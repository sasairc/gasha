/*
 * gcc -lgasha kirara.c -o kirara
 */
#include <gasha.h>
#include <stdio.h>
#include <unistd.h>

#define VERBOSE
//#define PICKUP

int main(void)
{
    size_t      i           = 0;

    GASHA*      gasha       = NULL;

    GASHA_CARD* result      = NULL;

    GASHA_CARD  cards[]     = {
        /*
         * ★★★
         */
        {1000000,   "ゆの",                             3},
        {1000001,   "ゆの",                             3},
        {1001000,   "宮子",                             3},
        {1001001,   "宮子",                             3},
        {1002000,   "ヒロ",                             3},
        {1003000,   "沙英",                             3},
        {1004000,   "乃莉",                             3},
        {1005000,   "なずな",                           3},
        {1100000,   "野々原 ゆずこ",                    3},
        {1100001,   "野々原 ゆずこ",                    3},
        {1101000,   "櫟井 唯",                          3},
        {1101001,   "櫟井 唯",                          3},
        {1102000,   "日向 縁",                          3},
        {1102001,   "日向 縁",                          3},
        {1103000,   "松本 頼子",                        3},
        {1104000,   "相川 千穂",                        3},
        {1200000,   "丈槍 由紀",                        3},
        {1200001,   "丈槍 由紀",                        3},
        {1201000,   "恵飛須沢 胡桃",                    3},
        {1201001,   "恵飛須沢 胡桃",                    3},
        {1202000,   "若狭 悠里",                        3},
        {1202001,   "若狭 悠里",                        3},
        {1203000,   "直樹 美紀",                        3},
        {1203001,   "直樹 美紀",                        3},
        {1204000,   "佐倉 慈",                          3},
        {1300000,   "トオル",                           3},
        {1300001,   "トオル",                           3},
        {1301000,   "るん",                             3},
        {1301001,   "るん",                             3},
        {1302000,   "ユー子",                           3},
        {1302001,   "ユー子",                           3},
        {1303000,   "ナギ",                             3},
        {1303001,   "ナギ",                             3},
        {1400000,   "九条 カレン",                      3},
        {1400001,   "九条 カレン",                      3},
        {1401000,   "アリス・カータレット",             3},
        {1401001,   "アリス・カータレット",             3},
        {1402000,   "大宮 忍",                          3},
        {1403000,   "小路 綾",                          3},
        {1404000,   "猪熊 陽子",                        3},
        {1405000,   "松原 穂乃花",                      3},
        {1500000,   "涼風 青葉",                        3},
        {1500001,   "涼風 青葉",                        3},
        {1501000,   "滝本 ひふみ",                      3},
        {1501001,   "滝本 ひふみ",                      3},
        {1502000,   "篠田 はじめ",                      3},
        {1503000,   "飯島 ゆん",                        3},
        {1504000,   "八神 コウ",                        3},
        {1505000,   "遠山 りん",                        3},
        {1507000,   "阿波根 うみこ",                    3},
        {1600000,   "本田 珠輝",                        3},
        {1600001,   "本田 珠輝",                        3},
        {1601000,   "藤川 歌夜",                        3},
        {1602000,   "村上 椎奈",                        3},
        {1602001,   "村上 椎奈",                        3},
        {1603000,   "関 あやめ",                        3},
        {1604000,   "布田 裕美音",                      3},
        {1700000,   "千矢",                             3},
        {1700001,   "千矢",                             3},
        {1701000,   "巽 紺",                            3},
        {1701001,   "巽 紺",                            3},
        {1702000,   "雪見 小梅",                        3},
        {1702001,   "雪見 小梅",                        3},
        {1703000,   "棗 ノノ",                          3},
        {1703001,   "棗 ノノ",                          3},
        {1800000,   "折部 やすな",                      3},
        {1801000,   "ソーニャ",                         3},
        {1802000,   "呉織 あぎり",                      3},
        {1900000,   "高山 春香",                        3},
        {1901000,   "園田 優",                          3},
        {1902000,   "南 しずく",                        3},
        {1903000,   "野田 コトネ",                      3},
        {1904000,   "池野 楓",                          3},
        {1905000,   "飯塚 ゆず",                        3},
        {2000000,   "桜ノ宮 苺香",                      3},
        {2001000,   "日向 夏帆",                        3},
        {2002000,   "星川 麻冬",                        3},
        {2003000,   "天野 美雨",                        3},
        {2004000,   "神崎 ひでり",                      3},
        {2100000,   "一之瀬 花名",                      3},
        {2101000,   "千石 冠",                          3},
        {2102000,   "十倉 栄依子",                      3},
        {2103000,   "百地 たまて",                      3},
        {2200000,   "平沢 唯",                          3},
        {2201000,   "秋山 澪",                          3},
        {2202000,   "田井中 律",                        3},
        {2203000,   "琴吹 紬",                          3},
        {2204000,   "中野 梓",                          3},
        {2600000,   "メリー・ナイトメア",               3},

        /*
         * ★★★★
         */
        {1000100,   "ゆの",                             4},
        {1001100,   "宮子",                             4},
        {1002100,   "ヒロ",                             4},
        {1003100,   "沙英",                             4},
        {1004100,   "乃莉",                             4},
        {1005100,   "なずな",                           4},
        {1100100,   "野々原 ゆずこ",                    4},
//      {1100101,   "ゆずこ【お正月】",                 4}, // 配布
        {1101100,   "櫟井 唯",                          4},
//      {1101101,   "唯【ひな祭り】",                   4}, // 配布
        {1102100,   "日向 縁",                          4},
        {1103100,   "松本 頼子",                        4},
        {1104100,   "相川 千穂",                        4},
//      {1105100,   "岡野 佳",                          4}, // 配布
        {1106100,   "長谷川 ふみ",                      4},
        {1200100,   "丈槍 由紀",                        4},
        {1201100,   "恵飛須沢 胡桃",                    4},
        {1202100,   "若狭 悠里",                        4},
        {1203100,   "直樹 美紀",                        4},
        {1204100,   "佐倉 慈",                          4},
        {1300100,   "トオル",                           4},
//      {1300101,   "トオル【水着】",                   4},
        {1301100,   "るん",                             4},
//      {1301101,   "るん【クリスマス】",               4},
        {1302100,   "ユー子",                           4},
        {1303100,   "ナギ",                             4},
        {1304100,   "ミホ",                             4},
        {1400100,   "九条 カレン",                      4},
//      {1400101,   "カレン【水着】",                   4}, // 配布
        {1401100,   "アリス・カータレット",             4},
//      {1401101,   "アリス・カータレット",             4}, // 配布 (まほうつかい)
        {1402100,   "大宮 忍",                          4},
//      {1402101,   "忍【お正月】",                     4}, // 配布
        {1403100,   "小路 綾",                          4},
        {1404100,   "猪熊 陽子",                        4},
        {1405100,   "松原 穂乃花",                      4},
        {1406100,   "大宮 勇",                          4},
        {1407100,   "烏丸 さくら",                      4},
        {1408100,   "久世橋 朱里",                      4},
        {1500100,   "涼風 青葉",                        4},
//      {1500101,   "涼風 青葉",                        4}, // 配布 (シロクマver)
        {1501100,   "滝本 ひふみ",                      4},
        {1502100,   "篠田 はじめ",                      4},
        {1503100,   "飯島 ゆん",                        4},
        {1504100,   "八神 コウ",                        4},
        {1505100,   "遠山 りん",                        4},
//      {1506100,   "桜 ねね",                          4}, // 配布 ペラゴン
        {1507100,   "阿波根 うみこ",                    4},
//      {1508100,   "紅葉【お正月】",                   4},
//      {1509100,   "つばめ【バレンタイン】",           4},
        {1511100,   "星川 ほたる",                      4},
        {1600100,   "本田 珠輝",                        4},
//      {1600101,   "珠輝【クリスマス】",               4},
        {1601100,   "藤川 歌夜",                        4},
        {1602100,   "村上 椎奈",                        4},
        {1603100,   "関 あやめ",                        4},
        {1604100,   "布田 裕美音",                      4},
//      {1604101,   "裕美音【水着】",                   4},
//      {1605100,   "百武 照",                          4}, // 配布
        {1606100,   "飯野 水葉",                        4},
        {1607100,   "池谷 乃々",                        4},
        {1700100,   "千矢",                             4},
//      {1700101,   "千矢【バレンタイン】",             4}, // 配布
        {1701100,   "巽 紺",                            4},
        {1702100,   "雪見 小梅",                        4},
        {1703100,   "棗 ノノ",                          4},
        {1704100,   "色井 佐久",                        4},
        {1705100,   "棗 ニナ",                          4},
        {1706100,   "二条 臣",                          4},
        {1800100,   "折部 やすな",                      4},
//      {1800101,   "やすな【水着】",                   4}, // 配布
        {1801100,   "ソーニャ",                         4},
//      {1801101,   "ソーニャ【クリスマス】",           4}, // 配布
        {1802100,   "呉織 あぎり",                      4},
        {1900100,   "高山 春香",                        4},
        {1901100,   "園田 優",                          4},
        {1902100,   "南 しずく",                        4},
        {1903100,   "野田 コトネ",                      4},
//      {1903101,   "コトネ【水着】",                   4},
        {1904100,   "池野 楓",                          4},
        {1905100,   "飯塚 ゆず",                        4},
        {1906100,   "乙川 澄",                          4},
        {2000100,   "桜ノ宮 苺香",                      4},
//      {2000101,   "苺香【ひな祭り】",                 4},
        {2001100,   "日向 夏帆",                        4},
        {2002100,   "星川 麻冬",                        4},
        {2003100,   "天野 美雨",                        4},
        {2004100,   "神崎 ひでり",                      4},
//      {2005100,   "桜ノ宮 愛香",                      4}, // 配布
//      {2100100,   "一之瀬 花名",                      4}, // 配布
        {2101100,   "千石 冠",                          4},
        {2102100,   "十倉 栄依子",                      4},
//      {2102101,   "栄依子【水着】",                   4},
        {2103100,   "百地 たまて",                      4},
//      {2200100,   "平沢 唯",                          4}, // 配布
        {2201100,   "秋山 澪",                          4},
        {2202100,   "田井中 律",                        4},
//      {2202101,   "律【バレンタイン】",               4}, // 配布
        {2203100,   "琴吹 紬",                          4},
        {2204100,   "中野 梓",                          4},
//      {2205100,   "平沢 憂",                          4}, // 配布
//      {2300100,   "志摩 リン",                        4}, // 配布
        {2301100,   "各務原 なでしこ",                  4},
        {2302100,   "大垣 千明",                        4},
        {2303100,   "犬山 あおい",                      4},
        {2304100,   "斉藤 恵那",                        4},
//      {2400100,   "萌田 薫子",                        4}, // 配布
        {2401100,   "恋塚 小夢",                        4},
        {2402100,   "色川 琉姫",                        4},
//      {2402101,   "琉姫【クリスマス】",               4}, // 配布
        {2403100,   "勝木 翼",                          4},
        {2600100,   "メリー・ナイトメア",               4},
        {2601100,   "橘勇魚",                           4},
//      {2700100,   "関谷 なる",                        4}, // 配布
        {2701100,   "ハナ・N・フォンテーンスタンド",    4},
        {2702100,   "笹目 ヤヤ",                        4},
        {2800100,   "花小泉 杏",                        4},
//      {2801100,   "雲雀丘 瑠璃",                      4}, // 配布
        {2802100,   "久米川 牡丹",                      4},
//      {2900100,   "大空 遥",                          4}, // 配布
        {2901100,   "比嘉 かなた",                      4},
        {2902100,   "トーマス・紅愛",                   4},
        {2903100,   "トーマス・恵美理",                 4},
//      {3000100,   "ココア",                           4}, // 配布
        {3001100,   "チノ",                             4},
//      {3002100,   "リゼ",                             4}, // 配布
//      {3100100,   "鳩谷 こはね",                      4}, // 配布
        {3101100,   "有馬 ひづめ",                      4},
        {3102100,   "猿渡 宇希",                        4},
        {3103100,   "舘島 虎徹",                        4},
        {3104100,   "牛久 花和",                        4},

        /*
         * ★★★★★
         */
        {1000200,   "ゆの",                             5},
//      {1000201,   "ゆの【クリスマス】",               5},
        {1001200,   "宮子",                             5},
        {1100200,   "野々原 ゆずこ",                    5},
//      {1100201,   "ゆずこ【水着】",                   5},
        {1101200,   "櫟井 唯",                          5},
        {1103200,   "松本 頼子",                        5},
        {1200200,   "丈槍 由紀",                        5},
//      {1200201,   "由紀【水着】",                     5},
        {1201200,   "恵飛須沢 胡桃",                    5},
        {1202200,   "若狭 悠里",                        5},
        {1204200,   "佐倉 慈",                          5},
        {1300200,   "トオル",                           5},
//      {1300201,   "トオル【クリスマス】",             5},
        {1301200,   "るん",                             5},
//      {1302200,   "ユー子【クリスマス】",             5},
        {1302201,   "ユー子",                           5},
        {1303200,   "ナギ",                             5},
        {1400200,   "九条 カレン",                      5},
//      {1401200,   "アリス【クリスマス】",             5},
        {1402200,   "大宮 忍",                          5},
//      {1403200,   "綾【バレンタイン】",               5},
//      {1403201,   "綾【水着】",                       5},
//      {1406200,   "勇【お正月】",                     5},
        {1500200,   "涼風 青葉",                        5},
        {1501200,   "滝本 ひふみ",                      5},
//      {1501201,   "ひふみ【クリスマス】",             5},
//      {1502200,   "はじめ【クリスマス】",             5},
//      {1503200,   "ゆん【ひな祭り】",                 5},
//      {1504200,   "コウ【水着】",                     5},
        {1600200,   "本田 珠輝",                        5},
        {1601200,   "藤川 歌夜",                        5},
//      {1602200,   "椎奈【バレンタイン】",             5},
        {1700200,   "千矢",                             5},
//      {1700201,   "千矢【クリスマス】",               5},
        {1701200,   "巽 紺",                            5},
//      {1701201,   "紺【水着】",                       5},
        {1702200,   "雪見 小梅",                        5},
//      {1702201,   "小梅【バレンタイン】",             5},
        {1703200,   "棗 ノノ",                          5},
        {1800200,   "折部 やすな",                      5},
//      {1800201,   "やすな【お正月】",                 5},
        {1801200,   "ソーニャ",                         5},
        {1900200,   "高山 春香",                        5},
        {1901200,   "園田 優",                          5},
//      {1902200,   "しずく【水着】",                   5},
        {2000200,   "桜ノ宮 苺香",                      5},
//      {2001200,   "夏帆【水着】",                     5},
//      {2001201,   "夏帆【お正月】",                   5},
        {2002200,   "星川 麻冬",                        5},
        {2100200,   "一之瀬 花名",                      5},
        {2101200,   "千石 冠",                          5},
        {2103200,   "百地 たまて",                      5},
        {2200200,   "平沢 唯",                          5},
//      {2201200,   "澪【バレンタイン】",               5},
        {2204200,   "中野 梓",                          5},
        {2300200,   "志摩 リン",                        5},
//      {2301200,   "なでしこ【水着】",                 5},
        {2301201,   "各務原 なでしこ",                  5},
        {2400200,   "萌田 薫子",                        5},
        {2600200,   "メリー・ナイトメア",               5},
//      {2600201,   "メリー【クリスマス】",             5},
//      {2601200,   "勇魚【ひな祭り】",                 5},
        {2700200,   "関谷 なる",                        5},
        {2800200,   "花小泉 杏",                        5},
        {2801200,   "雲雀丘 瑠璃",                      5},
        {2900200,   "大空 遥",                          5},
        {3000200,   "ココア",                           5},
        {3001200,   "チノ",                             5},
        {3002200,   "リゼ",                             5},
        {3100200,   "鳩谷 こはね",                      5},
        {3201200,   "ランプ",                           5},
        {3203200,   "ポルカ",                           5},
        {3204200,   "コルク",                           5},
        {3208200,   "アルシーヴ",                       5},
        {3209200,   "シュガー",                         5},
        {0, NULL, 0},
    };

#ifdef  PICKUP
    GASHA_PROB  pickups[]   = {
        {1800000,   0.2},
        {1801000,   0.2},
        {1802000,   0.2},
        {1800100,   0.2},
        {1801100,   0.2},
        {1802100,   0.2},
        {1800200,   0.1},
        {1800201,   0.1},
        {1801200,   0.1},
        {0, 0.0},
    };
/* PICKUP */
#endif

    init_gasha(&gasha);
    gasha->join_cards(&gasha, cards);
    gasha->conf->change_weight_of_rarity(&gasha, RARITY_R, 0.86);
    gasha->conf->change_weight_of_rarity(&gasha, RARITY_SR, 0.12);
    gasha->conf->change_weight_of_rarity(&gasha, RARITY_SSR, 0.02);

#ifdef  PICKUP
    gasha->conf->config_pickups(&gasha, pickups);
/* PICKUP */
#endif

#ifdef  VERBOSE
    size_t  j   = 0;

    fprintf(stderr, "**** cards ****\n");
    for (i = 0; i < gasha->cardc; i++) {
        fprintf(stderr, "id = %d, name = %s, rarity = %d\n",
                gasha->card[i]->id, gasha->card[i]->name, gasha->card[i]->rarity);
    }
    fprintf(stderr, "\n**** probabilities 1 ****\n");
    fprintf(stderr, "\
rarity = 3, weight = %f %%\n\
rarity = 4, weight = %f %%\n\
rarity = 5, weight = %f %%\n",
           gasha->conf->weights[RARITY_R],
           gasha->conf->weights[RARITY_SR],
           gasha->conf->weights[RARITY_SSR]);
    fprintf(stderr, "\n**** probabilities 2 ****\n");
    for (i = RARITY_R; i <= RARITY_SSR; i++) {
        for (j = 0; j < count_by_rarity(gasha, i); j++) {
            fprintf(stderr, "id = %d, weight = %f %%\n",
                    gasha->conf->probs[i][j]->id, gasha->conf->probs[i][j]->weight);
        }
    }
/* VERBOSE */
#endif

    if (gasha->is_ready(gasha)) {
        for (i = 1; i <= 10; i++) {
            if (i % 10 == 0)
                result = id2card(gasha, gasha->roll10(gasha));
            else
                result = id2card(gasha, gasha->roll(gasha));
            if (result->rarity == 5)
                fprintf(stdout, "%zu 回目の試行で 星%d の %s が当たりました。やったね！\n",
                        i, result->rarity, result->name);
            else
                fprintf(stdout, "%zu 回目の試行で 星%d の %s が当たりました。\n",
                        i, result->rarity, result->name);
            usleep(10000);
        }
    }
    gasha->release(gasha);

    return 0;
}
