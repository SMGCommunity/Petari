#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MatrixControl.hpp"

static MatrixMap morph_bitmap[] = {{"ジャンプ系バインド", 0x1111100F},
                                   {"スピンアタック", 0x11231041},
                                   {"コイン引っ張り", 0x11110001},
                                   {"カメ持ち", 0x1111000F},
                                   {"水解除", 0x00001110},
                                   {"ダメージ解除", 0x00001100},
                                   {"ノーダメージ解除", 0x00000100},
                                   {"アッパーパンチ", 0x11110101},
                                   {"スピン回復エフェクト", 0x10002000},
                                   {""}};

static MatrixSelectList select_list[] = {{8, "ノーマル", "メタル", "ファイア", "アイス", "ハチ", "ホッパー", "テレサ", "フー"},
                                         {2, "×NG", "OK○"},
                                         {6, "×NG", "OK○", "FIRE", "ICE", "消える", "回転のみ"},
                                         {2, "×NG", "OK○"},
                                         {2, "×NG", "OK○"},
                                         {2, "×NG", "OK○"},
                                         {2, "×NG", "OK○"},
                                         {2, "×NG", "OK○"},
                                         {3, "×NG", "OK○", "頭突き"},
                                         {3, "×NG", "エフェクト＋音", "音のみ"}};

static MatrixMap auto_bind_bitmap[] = {{"ウォータープレッシャーの弾", 0x10112222},
                                       {"ポール", 0x11111000},
                                       {"グリーンスーパースピンドライバー", 0x11111222},
                                       {"スーパースピンドライバー", 0x11111222},
                                       {"スピンドライバ", 0x11111222},
                                       {"Gキャプチャー", 0x11111222},
                                       {"移動用砲台", 0x11111222},
                                       {"ウォーターロード", 0x11112222},
                                       {"土管", 0x33333103},
                                       {"噴水（大）", 0x00002000},
                                       {"オオアワ[共有]", 0x11112222},
                                       {"スイングロープ", 0x11111000},
                                       {"空中ブランコ", 0x11111000},
                                       {""}};

static MatrixSelectList auto_bind_selecterlist[] = {{8, "ノーマル", "メタル", "ファイア", "アイス", "ハチ", "ホッパー", "テレサ", "フー"},
                                                    {4, "×スルー", "OK○", "変身解除", "移動中不可"}};

static MatrixMap auto_efx_bitmap[] = {
    {"わたげ発生源", 0x20000000},
    {"杭スイッチ", 0x10000000},
    {"ダミー敵"},
    {"ザコカメムシ", 0x18000400},
    {"子連れカメムシ", 0x18000400},
    {"細野用マップパーツ75", 0x04000000},
    {"幽霊船たいまつ", 0x01000000},
    {"燭台", 0x01000000},
    {"燭台(テレサマンション)", 0x01000000},
    {"燭台(アイスボルケーノ)"},
    {"バブル", 0x01000000},
    {"カボクリ", 0x11000000},
    {"エイ", 0x10800000},
    {"ポール", 0x025A0000},
    {"ポール（鉄骨）", 0x025A0000},
    {"オニマス(ピボット)", 0x00200000},
    {"オニマス", 0x00200000},
    {"移動用砲台", 0x00040000},
    {"スーパースピンドライバー", 0x00040000},
    {"スピンドライバ", 0x00040000},
    {"伸び植物", 0x00090000},
    {"空中ブランコ", 0x00080000},
    {"つる花", 0x00080000},
    {"土管", 0x0040A800},
    {"タイマーピースブロック", 0x00004000},
    {"ゴム星", 0x00001000},
    {"電撃レール"},
    {"電撃レール点"},
    {"ビリキュー"},
    {"宝箱"},
    {"宝箱(空っぽ)"},
    {"宝箱ゴールド(空っぽ)"},
    {"宝箱(ライフＵＰキノコ)"},
    {"ひび割れ宝箱"},
    {"アイス床"},
    {"草"},
    {"花"},
    {"青い花"},
    {"トルネード小石"},
    {"サークルシェル"},
    {"サークルストロベリー"},
    {"タマコロチュートリアル"},
    {"コイン花"},
    {"隠れアイテム"},
    {"スターピーススター"},
    {"レバースイッチ"},
    {"クリスタルケージ[小]"},
    {"クリスタルケージ[中]"},
    {"クリスタルケージ[大]"},
    {"ファイアバー"},
    {"電撃ビリビリレール"},
    {"移動電撃ビリビリレール"},
    {"スターピース"},
    {"アイテムバブルピース"},
    {"スターピースディレクターピース"},
    {"フォロースターピース"},
    {"グループスターピース"},
    {"スターピースマザーピース"},
    {"トゲ植物"},
    {"トゲ植物(空中)"},
    {"陸ウニゾー"},
    {"ビリビリボール"},
    {"飾り足"},
    {"モグ"},
    {"蝶"},
    {"タコヘイ墨"},
    {"マグナムキラー", 0x10000000},
    {"雪だるま", 0x10000000},
    {"よわブロック", 0x10000000},
    {"ジャンプ台", 0x10000000},
    {"スピニングボックス", 0x10000000},
    {"杭スイッチビッグ", 0x10000000},
    {"ヒビ石", 0x10000000},
    {"メカクッパパーツローラーA", 0x00004000},
    {""},
};

static MatrixSelectList auto_efx_selecterlist[] = {{19,
                                                    "スピンヒット振動",
                                                    "スピーカー音",
                                                    "スピンキャッチ許可",
                                                    "SHDホーミング許可(地形)",
                                                    "地形移動無視",
                                                    "ハチ出立",
                                                    "乗換センサ",
                                                    "青い炎エフェクト",
                                                    "水柱エフェクト",
                                                    "ダメージ中RUSH",
                                                    "ナナメつぶし許可",
                                                    "カメを捨てない",
                                                    "ハチ回復",
                                                    "ハチメータ消去",
                                                    "テレサ時は無視",
                                                    "リバインド制限",
                                                    "HD中ラッシュ",
                                                    "食い込み除外",
                                                    "水陸移動特殊処理",
                                                    "着地エフェクト",
                                                    "影を消す",
                                                    "ダメージ中跳ねる"},
                                                   {2, "□マリオ側で立てる", "[v]オブジェ側で立てる"}};

static MatrixValueTable talking_head_height[] = {
    {"クッパJｒ", 300.0f}, {"ルイージ", 200.0f}, {"ピーチ姫", 180.0f},       {"ペンギンコーチ", 500.0f},       {"ペンギン仙人", 500.0f},
    {"チコ", 100.0f},      {"デブチコ", 200.0f}, {"キノピオ", 100.0f},       {"天文台用キノピオ", 100.0f},     {"ペンギン", 160.0f},
    {"ロゼッタ", 240.0f},  {"ウサギ", 100.0f},   {"いたずらウサギ", 100.0f}, {"シャッチー（会話用）", 220.0f}, {""}};

void MarioActor::initActionMatrix() {
    _FBC = new MatrixControl("変身中のマリオ行動表", morph_bitmap, select_list, -1);
    _FC0 = new MatrixControl("マリオ状態とオートバインド対応表", auto_bind_bitmap, auto_bind_selecterlist, 1);
    _FC4 = new MatrixControl("メッセージ送信応答時のエフェクト", auto_efx_bitmap, auto_efx_selecterlist, 1);

    _FC8 = new MatrixValueGetter("キャラの身長", talking_head_height);
}

bool MarioActor::isActionOk(const char* pName) const {
    return _FBC->getValue(pName, mPlayerMode);
}

u8 MarioActor::selectAction(const char* pName) const {
    return _FBC->getValue(pName, mPlayerMode);
}

bool MarioActor::selectAutoBind(const char* pName, u8* value) const {
    return _FC0->getValueOrNone(pName, mPlayerMode, value);
}

bool MarioActor::selectCustomEffectSpinHitSound(const char* pName) const {
    return _FC4->getBitOrNone(pName, 0);
}

bool MarioActor::selectSpinCatchInRush(const char* pName) const {
    return _FC4->getBitOrNone(pName, 2);
}

bool MarioActor::selectHomingInSuperHipDrop(const char* pName) const {
    return !_FC4->isExist(pName) ? false : _FC4->getBit(pName, 3);
}

bool MarioActor::selectNotHomingSensor(const HitSensor* pSensor) const {
    return !_FC4->isExist(pSensor->mHost->mName) ? false : !_FC4->getBit(pSensor->mHost->mName, 3);
}

bool MarioActor::selectInvalidMovingCollision(const char* pName) const {
    return !_FC4->isExist(pName) ? false : _FC4->getBit(pName, 4);
}

bool MarioActor::selectQuickResetBeeWallGravity(const char* pName) const {
    return !_FC4->isExist(pName) ? false : _FC4->getBit(pName, 5);
}

bool MarioActor::selectJumpRushSensor(const char* pName) const {
    return !_FC4->isExist(pName) ? false : _FC4->getBit(pName, 6);
}

bool MarioActor::selectDamageFireColor(const char* pName) const {
    return _FC4->getBitOrNone(pName, 7);
}

bool MarioActor::selectWaterInOutEffect(const char* pName) const {
    return _FC4->getBitOrNone(pName, 8);
}

bool MarioActor::selectOnDamageRush(const char* pName) const {
    return _FC4->getBitOrNone(pName, 9);
}

bool MarioActor::selectTiltPress(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 10);
}

bool MarioActor::selectHandyRush(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 11);
}

bool MarioActor::selectRecoverFlyMeter(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 12);
}

bool MarioActor::selectHideFlyMeter(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 13);
}

bool MarioActor::selectTeresaThru(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 14);
}

bool MarioActor::selectRebindTimer(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 15);
}

bool MarioActor::selectHipDropRush(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 16);
}

bool MarioActor::selectPushOff(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 17);
}

bool MarioActor::selectWaterInOutRush(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 18);
}

bool MarioActor::selectLandEffect(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 19);
}

bool MarioActor::selectNoShadow(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 20);
}

bool MarioActor::selectDamagePop(const HitSensor* pSensor) const {
    return _FC4->getBitOrNone(pSensor->mHost->mName, 21);
}

f32 MarioActor::getFaceLookHeight(const char* pName) const {
    f32 value;
    return _FC8->getValue(pName, &value) ? value : 150.0f;
}
