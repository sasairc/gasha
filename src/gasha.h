/*
 * gasha.h
 *
 * Copyright (c) 2019 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef GASHA_H
#define GASHA_H
#ifdef  __cplusplus
extern "C" {
/* __cplusplus */
#endif

#include <stdint.h> /* uint32_t */
#include <stdio.h>  /* size_t */

/*
 * レアリティの定義
 */
#define RARITY_R    3
#define RARITY_SR   4
#define RARITY_SSR  5

/*
 * レアリティに対する確率の定義
 */
#define DEFAULT_WEIGHT_R    0.85
#define DEFAULT_WEIGHT_SR   0.12
#define DEFAULT_WEIGHT_SSR  0.03

/*
 * カード（各キャラクター）
 *
 * name は暫定的 ( id と rarity だけ持たせたい )
 */
typedef struct GASHA_CARD {
    uint32_t    id;
    char*       name;
    uint32_t    rarity;
} GASHA_CARD;

/*
 * 各種設定
 */
typedef struct GASHA_PROB {
    uint32_t        id;
    float           weight;
} GASHA_PROB;

typedef struct GASHA GASHA;
typedef struct GASHA_CONF {
    float           weights[6];
    GASHA_PROB***   probs;
    int             (*change_weight_of_rarity)(GASHA** gasha, uint32_t rarity, float weight);
    void            (*normalize_weight_of_rarity)(GASHA** gasha);
    int             (*config_pickups)(GASHA** gasha, GASHA_PROB pickups[]);
    void            (*release)(GASHA** gasha);
} GASHA_CONF;

/*
 * このライブラリの要
 */
typedef struct GASHA {
    GASHA_CONF*     conf;
    GASHA_CARD**    card;
    size_t          cardc;
    int             (*join_cards)(GASHA** gasha, GASHA_CARD cards[]);
    int             (*is_ready)(GASHA* gasha);
    uint32_t        (*roll)(GASHA* gasha);
    uint32_t        (*roll10)(GASHA* gasha);
    uint32_t        (*roll100)(GASHA* gasha);
    void            (*release)(GASHA* gasha);
} GASHA;

int init_gasha(GASHA** gasha);
GASHA_CARD* id2card(GASHA* gasha, uint32_t id);

size_t count_by_rarity(GASHA* gasha, uint32_t rarity);
GASHA_CARD** filter_by_rarity(GASHA* gasha, uint32_t rarity);

#ifdef  __cplusplus
}
/* __cplusplus */
#endif
/* GASHA_H */
#endif
