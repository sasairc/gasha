# Gasha

Gasha is a "Gacha System" emulation library written in C.
This library does not have any dependencies and can be compiled using any C compiler.

## Installation

On UNIX, GNU/Linux and similar systems, you can simply type

```
% make
# make [PREFIX=/prefix] install
```

By default, `make install` installs the shared library under */usr/local/lib*, include files under */usr/local/include*. You can specify an installation prefix other than */usr/local* by giving `make` the option `PREFIX=/prefix`.


## API

#### GASHA* new_gasha(void);
#### int init_gasha(GASHA** gasha);

#### int gasha->join_cards(GASHA** gasha, GASHA_CARD cards[]);
#### int gasha->is_ready(GASHA* gasha);
#### uint32_t gasha->roll(GASHA* gasha);
#### uint32_t gasha->roll10(GASHA* gasha);
#### uint32_t gasha->roll100(GASHA* gasha);
#### void gasha->release(GASHA* gasha);

#### int gasha->conf->change_weight_of_rarity(GASHA** gasha, uint32_t rarity, float weight);
#### int gasha->conf->change_random_number_generator(GASHA** gasha, float (*fn)(void));
#### void gasha->conf->normalize_weight_of_rarity(GASHA** gasha);
#### int gasha->conf->config_pickups(GASHA** gasha, GASHA_PROB pickups[]);
#### void gasha->conf->release(GASHA** gasha);

#### GASHA_CARD* id2card(GASHA* gasha, uint32_t id);
#### size_t count_by_rarity(GASHA* gasha, uint32_t rarity);
#### GASHA_CARD** filter_by_rarity(GASHA* gasha, uint32_t rarity);


## Example

```c
#include <stdio.h>
#include <gasha.h>

int main(void)
{
    size_t      i       = 0;

    GASHA*      gasha   = new_gasha();
    GASHA_CARD* result  = NULL;
    GASHA_CARD  cards[] = {
        {100,   "chara01",  3}, /* {id, "name", rarity} */
        {200,   "chara02",  3},
        {300,   "chara03",  3},
        {400,   "chara04",  3},
        {500,   "chara05",  3},
        {600,   "chara06",  4},
        {700,   "chara07",  4},
        {800,   "chara08",  4},
        {900,   "chara09",  4},
        {100,   "chara10",  4},
        {110,   "chara11",  5},
        {120,   "chara12",  5},
        {130,   "chara13",  5},
        {140,   "chara14",  5},
        {150,   "chara15",  5},
        {0, NULL, 0},
    };
    gasha->join_cards(&gasha, cards);
    gasha->conf->change_weight_of_rarity(&gasha, RARITY_R, 0.85);
    gasha->conf->change_weight_of_rarity(&gasha, RARITY_SR, 0.12);
    gasha->conf->change_weight_of_rarity(&gasha, RARITY_SSR, 0.03);

#ifdef PICKUP
    /* pickup */
    GASHA_PROB  pickups[]   = {
        {200,   0.4},   /* {id, probability} */
        {400,   0.4},
        {600,   0.4},
        {800,   0.4},
        {100,   0.4},
        {120,   0.6},
        {0, 0.0},
    };
    gasha->conf->config_pickups(&gasha, pickups);
/* PICKUP */
#endif

    if (gasha->is_ready(gasha)) {
        for (i = 1; i <= 10; i++) {
            if (i % 10)
                result = id2card(gasha, gasha->roll(gasha));
            else
                result = id2card(gasha, gasha->roll10(gasha)); /* rarity >= 4 */
            fprintf(stdout, "id = %zu, name = %s, rarity = %zu\n",
                    result->id, result->name, result->rarity);
        }
    }
    gasha->release(gasha);

    return 0;
}
```

## Kirara

[kirara.c](https://raw.githubusercontent.com/sasairc/gasha/wip/kirara/kirara.c) -  Kirara Fantasia "Gacha" Simulator
