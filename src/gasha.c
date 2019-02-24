/*
 * gasha.c
 *
 * Copyright (c) 2019 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./gasha.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>

#ifdef  LIBRARY_VERBOSE
#define print_error()   fprintf(stderr, "%s: %d: %s\n",\
        __FILE__, __LINE__, strerror(errno))
/* LIBRARY_VERBOSE */
#endif

static int create_config(GASHA** gasha);
static int config_probs(GASHA** gasha);
static int change_weight_of_rarity(GASHA** gasha, uint32_t rarity, float weight);
static void normalize_weight_of_rarity(GASHA** gasha);
static int config_pickups(GASHA** gasha, GASHA_PROB pickups[]);
static int sort_probs(GASHA** gasha, uint32_t rarity);
static int normalize_probs(GASHA** gasha, uint32_t rarity);
static int join_cards(GASHA** gasha, GASHA_CARD cards[]);
static int is_ready(GASHA* gasha);
static uint32_t roll(GASHA* gasha);
static uint32_t roll10(GASHA* gasha);
static uint32_t roll100(GASHA* gasha);
static uint32_t select_card(GASHA* gasha, uint32_t rarity);
static void release_conf(GASHA** gasha);
static void release_conf_probs(GASHA** gasha);
static void release_card(GASHA** gasha);
static void release(GASHA* gasha);
static float gen_rval(void);

int init_gasha(GASHA** gasha)
{
    GASHA*  gs  = NULL;

    if ((gs = (GASHA*)
                malloc(sizeof(GASHA))) == NULL) {
#ifdef  LIBRARY_VERBOSE
        print_error();
/* LIBRARY_VERBOSE */
#endif
        goto ERR;
    } else {
        gs->conf        = NULL;
        gs->card        = NULL;
        gs->cardc       = 0;
        gs->join_cards  = join_cards;
        gs->is_ready    = is_ready;
        gs->roll        = roll;
        gs->roll10      = roll10;
        gs->roll100     = roll100;
        gs->release     = release;
    }
    if (create_config(&gs) < 0)
        goto ERR;

    *gasha = gs;

    return 0;

ERR:
    release(gs);
    
    return -1;
}

/*
 * id2card()
 *
 * roll() から返却された ID からカードの情報を得る
 */
GASHA_CARD*
id2card(GASHA* gasha, uint32_t id)
{
    size_t  i   = 0;

    for (i = 0; i < gasha->cardc; i++) {
        if (gasha->card[i]->id == id)
            return gasha->card[i];
    }

    return NULL;
}

/*
 * count_by_rarity()
 * filter_by_rarity()
 *
 * レアリティ別のリストを返す
 */
size_t
count_by_rarity(GASHA* gasha, uint32_t rarity)
{
    size_t  i   = 0,
            n   = 0;

    for (i = 0; i < gasha->cardc; i++)
        if (gasha->card[i]->rarity == rarity)
            n++;

    return n;
}

GASHA_CARD**
filter_by_rarity(GASHA* gasha, uint32_t rarity)
{
    size_t          i       = 0,
                    n       = 0;

    GASHA_CARD**    dest    = NULL;

    if ((dest = (GASHA_CARD**)
                malloc(sizeof(GASHA_CARD*) * (count_by_rarity(gasha, rarity) + 1))) == NULL) {
#ifdef  LIBRARY_VERBOSE
        print_error();
/* LIBRARY_VERBOSE */
#endif
        return NULL;
    }
    for (i = 0; i < gasha->cardc; i++) {
        if (gasha->card[i]->rarity == rarity) {
            dest[n] = gasha->card[i];
            n++;
        }
    }
    dest[n] = NULL;

    return dest;
}

/*
 * create_config()
 *
 * レアリティに対する既定の確率を設定する
 */
static
int create_config(GASHA** gasha)
{
    if ((*gasha)->conf != NULL)
        release_conf(gasha);

    if (((*gasha)->conf = (GASHA_CONF*)
                malloc(sizeof(GASHA_CONF))) == NULL) {
#ifdef  LIBRARY_VERBOSE
        print_error();
/* LIBRARY_VERBOSE */
#endif
        return -1;
    } else {
        (*gasha)->conf->weights[RARITY_R]           = DEFAULT_WEIGHT_R;
        (*gasha)->conf->weights[RARITY_SR]          = DEFAULT_WEIGHT_SR;
        (*gasha)->conf->weights[RARITY_SSR]         = DEFAULT_WEIGHT_SSR;
        (*gasha)->conf->change_weight_of_rarity     = change_weight_of_rarity;
        (*gasha)->conf->normalize_weight_of_rarity  = normalize_weight_of_rarity;
        (*gasha)->conf->config_pickups              = config_pickups;
        (*gasha)->conf->release                     = release_conf;
    }

    return 0;
}

static
int config_probs(GASHA** gasha)
{
    size_t          i       = 0;

    uint32_t        rarity  = 0;

    GASHA_CARD**    cards   = NULL;

    if ((*gasha)->conf->probs != NULL)
        release_conf_probs(gasha);

    if (((*gasha)->conf->probs = (GASHA_PROB***)
                malloc(sizeof(GASHA_PROB**) * 6)) == NULL) {
#ifdef  LIBRARY_VERBOSE
        print_error();
/* LIBRARY_VERBOSE */
#endif
        return -1;
    }
    for (rarity = RARITY_R; rarity <= RARITY_SSR; rarity++) {
        if (((*gasha)->conf->probs[rarity] = (GASHA_PROB**)
                    malloc(sizeof(GASHA_PROB*) * (count_by_rarity(*gasha, rarity) + 1))) == NULL) {
#ifdef  LIBRARY_VERBOSE
            print_error();
/* LIBRARY_VERBOSE */
#endif
            goto ERR;
        }
        cards = filter_by_rarity(*gasha, rarity);
        for (i = 0; i < count_by_rarity(*gasha, rarity); i++) {
            if (((*gasha)->conf->probs[rarity][i] = (GASHA_PROB*)
                        malloc(sizeof(GASHA_PROB))) == NULL) {
#ifdef  LIBRARY_VERBOSE
                print_error();
/* LIBRARY_VERBOSE */
#endif
                goto ERR;
            }
            (*gasha)->conf->probs[rarity][i]->id = cards[i]->id;
            (*gasha)->conf->probs[rarity][i]->weight = 1.0 / (float)count_by_rarity(*gasha, rarity);
        }
        (*gasha)->conf->probs[rarity][i] = NULL;
    }

    return 0;

ERR:
    release_conf_probs(gasha);

    return -1;
}

/*
 * gasha->conf->config_pickups();
 *
 * ピックアップを設定する
 */
static
int config_pickups(GASHA** gasha, GASHA_PROB pickups[])
{
    size_t      i       = 0,
                j       = 0;

    GASHA_CARD* card    = NULL;

    for (i = 0; pickups[i].id != 0; i++) {
        if ((card = id2card(*gasha, pickups[i].id)) == NULL)
            return -1;
        for (j = 0; (*gasha)->conf->probs[card->rarity] != NULL &&
                pickups[i].id != (*gasha)->conf->probs[card->rarity][j]->id; j++);
        if ((*gasha)->conf->probs[card->rarity] == NULL)
            return -1;
        (*gasha)->conf->probs[card->rarity][j]->weight = pickups[i].weight;
    }
    for (i = RARITY_R; i <= RARITY_SSR; i++) {
        sort_probs(gasha, i);
        normalize_probs(gasha, i);
    }

    return 0;
}

/*
 * sort_probs()
 *
 * テーブルをピックアップ用にソートする
 */
static
int sort_probs(GASHA** gasha, uint32_t rarity)
{
    size_t      i       = 0,
                j       = 0;

    GASHA_PROB  p;

    for (i = 0; i < count_by_rarity(*gasha, rarity); i++) {
        for (j = count_by_rarity(*gasha, rarity) - 1; j > i; j--) {
            if ((*gasha)->conf->probs[rarity][j - 1]->weight <
                    (*gasha)->conf->probs[rarity][j]->weight) {
                p = *((*gasha)->conf->probs[rarity][j - 1]);
                *((*gasha)->conf->probs[rarity][j - 1]) = *((*gasha)->conf->probs[rarity][j]);
                *((*gasha)->conf->probs[rarity][j]) = p;
            }
        }
    }

    return 0;
}

/*
 * normalize_probs()
 *
 * ピックアップ外の確率を設定する
 */
static
int normalize_probs(GASHA** gasha, uint32_t rarity)
{
    size_t  i       = 0;

    float   pw      = 0.0,
            npw     = 0.0;

    npw = (*gasha)->conf->probs[rarity][count_by_rarity(*gasha, rarity) - 1]->weight;
    while ((*gasha)->conf->probs[rarity][i]->weight != npw) {
        pw += (*gasha)->conf->probs[rarity][i]->weight;
        i++;
    }
    npw = (1.0 - pw) / ((float)count_by_rarity(*gasha, rarity) - i);
    while (i < count_by_rarity(*gasha, rarity)) {
        (*gasha)->conf->probs[rarity][i]->weight = npw;
        i++;
    }

    return 0;
}

/*
 * gasha->conf->change_weight_of_rarity()
 * 
 * レアリティ別の確率を設定する
 */
static
int change_weight_of_rarity(GASHA** gasha, uint32_t rarity, float weight)
{
    if ((*gasha)->conf == NULL) {
        if (create_config(gasha) < 0)
            return -1;
    }
    (*gasha)->conf->weights[rarity] = weight;

    return 0;
}

/*
 * gasha->conf->normalize_weight_of_rarity()
 *
 * レアリティ別の確率を正規化する
 */
static
void normalize_weight_of_rarity(GASHA** gasha)
{
    (*gasha)->conf->weights[RARITY_R] = 1.0 -
        ((*gasha)->conf->weights[RARITY_SR] + (*gasha)->conf->weights[RARITY_SSR]);

    return;
}

/*
 * gasha->join_cards()
 *
 * gasha にカードを登録する
 */
static
int join_cards(GASHA** gasha, GASHA_CARD cards[])
{
    size_t  i   = 0;

    if ((*gasha)->card != NULL)
        release_card(gasha);

    for ((*gasha)->cardc = 0;
            cards[(*gasha)->cardc].name != NULL; ((*gasha)->cardc)++);
    if (((*gasha)->card = (GASHA_CARD**)
                malloc(sizeof(GASHA_CARD) * (*gasha)->cardc)) == NULL) {
#ifdef  LIBRARY_VERBOSE
        print_error();
/* LIBRARY_VERBOSE */
#endif
        goto ERR;
    }
    for (i = 0; i < (*gasha)->cardc; i++) {
        if (((*gasha)->card[i] = (GASHA_CARD*)
                    malloc(sizeof(GASHA_CARD))) == NULL) {
#ifdef  LIBRARY_VERBOSE
            print_error();
/* LIBRARY_VERBOSE */
#endif
            goto ERR;
        } else {
            (*gasha)->card[i]->id = cards[i].id;
            (*gasha)->card[i]->rarity = cards[i].rarity;
            if (((*gasha)->card[i]->name = strdup(cards[i].name)) == NULL) {
#ifdef  LIBRARY_VERBOSE
                print_error();
/* LIBRARY_VERBOSE */
#endif
                goto ERR;
            }
        }
    }
    config_probs(gasha);

    return 0;

ERR:
    release_card(gasha);

    return -1;
}

/*
 * select_card()
 *
 * 確定したレアリティのカードを一枚抽選する
 *
 * 2019/02/24 - 全て重みベースで抽選するよう変更
 *
 * こんな感じ
 *
 * +---+---+-+-+-+-+
 * | 3 | 3 |1|1|1|1|
 * +---+---+-+-+-+-+
 * total weight = 1.0
 */
static
uint32_t select_card(GASHA* gasha, uint32_t rarity)
{
    size_t      i       = 0;

    uint32_t    id      = 0;

    float       rval    = 0;

    /*
     * rval = 0.615385 はやすなちゃん
     * rval = 0.692308 はソーニャちゃん
     */
    rval = gen_rval();
    for (i = 0; i < count_by_rarity(gasha, rarity); i++) {
        if (rval <= gasha->conf->probs[rarity][i]->weight) {
            id = gasha->conf->probs[rarity][i]->id;
            break;
        }
        rval -= gasha->conf->probs[rarity][i]->weight;
    }

    return id;
}

/*
 * gasha->is_ready()
 *
 * roll()/roll10() できる状態であるかチェックする
 */
static
int is_ready(GASHA* gasha)
{
    if (gasha->conf != NULL     &&
            gasha->card != NULL &&
            gasha->cardc > 0)
        return 1;

    return 0;
}

/*
 * gasha->roll()
 *
 * 所謂「単発」
 */
static
uint32_t roll(GASHA* gasha)
{
    float   rval    = gen_rval();

    if (rval < gasha->conf->weights[RARITY_SSR])
        return select_card(gasha, RARITY_SSR);

    if (rval < gasha->conf->weights[RARITY_SR])
        return select_card(gasha, RARITY_SR);

    return select_card(gasha, RARITY_R);
}

/*
 * gasha->roll10()
 *
 * 10連ガシャにおいて SR 以上を確定させる
 * この関数が10連する訳ではなく、上記の roll() を組み合わせて使う
 */
static
uint32_t roll10(GASHA* gasha)
{
    float   rval    = gen_rval();

    if (rval < gasha->conf->weights[RARITY_SSR])
        return select_card(gasha, RARITY_SSR);

    return select_card(gasha, RARITY_SR);
}

/*
 * gasha->roll100()
 *
 * SSR 確定
 */
static
uint32_t roll100(GASHA* gasha)
{
    return select_card(gasha, RARITY_SSR);
}

/*
 * 以下はリソースの解放
 * release() で全て呼ばれる
 */
static
void release_conf(GASHA** gasha)
{
    if ((*gasha)->conf == NULL)
        return;

    free((*gasha)->conf);
    (*gasha)->conf = NULL;

    return;
}

static
void release_conf_probs(GASHA** gasha)
{
    size_t  i   = 0,
            j   = 0;

    if ((*gasha)->conf->probs == NULL)
        return;

    for (i = RARITY_R; i <= RARITY_SSR; i++) {
        if ((*gasha)->conf->probs[i] != NULL) {
            for (j = 0; (*gasha)->conf->probs[i][j] != NULL; j++) {
                free((*gasha)->conf->probs[i][j]);
                (*gasha)->conf->probs[i][j] = NULL;
            }
            free((*gasha)->conf->probs[i]);
            (*gasha)->conf->probs[i] = NULL;
        }
    }
    free((*gasha)->conf->probs);
    (*gasha)->conf->probs = NULL;

    return;
}

static
void release_card(GASHA** gasha)
{
    size_t  i   = 0;

    if ((*gasha)->card == NULL)
        return;

    for (i = 0; i < (*gasha)->cardc; i++) {
        if ((*gasha)->card[i] != NULL) {
            if ((*gasha)->card[i]->name != NULL) {
                free((*gasha)->card[i]->name);
                (*gasha)->card[i]->name = NULL;
            }
            free((*gasha)->card[i]);
            (*gasha)->card[i] = NULL;
        }
    }
    free((*gasha)->card);
    (*gasha)->card = NULL;
    (*gasha)->cardc = 0;

    return;
}

static
void release(GASHA* gasha)
{
    if (gasha != NULL) {
        gasha->conf->release(&gasha);
        release_conf(&gasha);
        release_card(&gasha);
        free(gasha);
    }

    return;
}

/*
 * gen_rval()
 *
 * 乱数生成
 */
static
float gen_rval(void)
{
    struct  timeval tv;

    gettimeofday(&tv, NULL);
    srand(tv.tv_usec);

    return (float)rand() / (float)RAND_MAX;
}
