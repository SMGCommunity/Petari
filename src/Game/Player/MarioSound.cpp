#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioWait.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JAudio2/JAISound.hpp>
#include <cstring>

struct SoundList {
    union SoundFlags {
        u32 _0;
        u8 _4[4];
    };

    const char* name;
    u32 _4;

    SoundFlags _8;
    const char* _C;

    u32 _10;
    u32 _14;
};

SoundList soundlist[] = {
    {
        "声小ジャンプ",  // name
        SE_PV_JUMP_S,    // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声中ジャンプ",  // name
        SE_PV_JUMP_M,    // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声大ジャンプ",  // name
        SE_PV_JUMP_L,    // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声後ジャンプ",   // name
        SE_PV_JUMP_TURN,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "声幅ジャンプ",   // name
        SE_PV_JUMP_LONG,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "声物ジャンプ",  // name
        SE_PV_JUMP_JOY,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声高飛び込み",   // name
        SE_PV_HIGH_DIVE,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "声尻ドロップ",  // name
        SE_PV_HIP_DROP,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声尻ドロップ着地",   // name
        SE_PV_HIP_DROP_LAND,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "声着地停止",  // name
        SE_PV_LAND,    // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "声ステージイン着地",  // name
        SE_PV_LAND_COOL,       // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声小ダメージ",  // name
        SE_PV_DAMAGE_S,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声大ダメージ",  // name
        SE_PV_DAMAGE_L,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声最終ダメージ",   // name
        SE_PV_LAST_DAMAGE,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "声炎ダメージ",  // name
        SE_PV_BURN,      // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声電気ダメージ",   // name
        SE_PV_ELEC_DAMAGE,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "声電気ダメージ終了",       // name
        SE_PV_ELEC_DAMAGE_RECOVER,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "声倒れ",    // name
        SE_PV_DOWN,  // 0x4
        0,           // 0x8
        0,           // 0xC
        0,           // 0x10
        0,           // 0x14
    },
    {
        "声落下死亡",    // name
        SE_PV_FALL_DIE,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声しびれ",      // name
        SE_PV_DAMAGE_S,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声氷ダメージ",  // name
        SE_PV_FREEZE,    // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声つぶれダメージ",    // name
        SE_PV_DIE_ROCK_CRASH,  // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声スピン",  // name
        SE_PV_SPIN,  // 0x4
        0,           // 0x8
        0,           // 0xC
        0,           // 0x10
        0,           // 0x14
    },
    {
        "声スピンキャンセル",  // name
        SE_PV_SPIN_CANCEL,     // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声パンチ",   // name
        SE_PV_PUNCH,  // 0x4
        0,            // 0x8
        0,            // 0xC
        0,            // 0x10
        0,            // 0x14
    },
    {
        "声踏み",     // name
        SE_PV_STOMP,  // 0x4
        0,            // 0x8
        0,            // 0xC
        0,            // 0x10
        0,            // 0x14
    },
    {
        "声蹴り",    // name
        SE_PV_KICK,  // 0x4
        0,           // 0x8
        0,           // 0xC
        0,           // 0x10
        0,           // 0x14
    },
    {
        "声トルネード",     // name
        SE_PV_TWIST_START,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "声壁反射",   // name
        SE_PV_GUARD,  // 0x4
        0,            // 0x8
        0,            // 0xC
        0,            // 0x10
        0,            // 0x14
    },
    {
        "声投げ",     // name
        SE_PV_THROW,  // 0x4
        0,            // 0x8
        0,            // 0xC
        0,            // 0x10
        0,            // 0x14
    },
    {
        "声壁押し",     // name
        SE_PV_LIFT_UP,  // 0x4
        0,              // 0x8
        0,              // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "声壁体当たり",       // name
        SE_PV_WALL_HIT_BODY,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "声スピン尻ドロップ",  // name
        SE_PV_PUNCH,           // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声軽い力み",  // name
        SE_PV_CATCH,   // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "声崖つかまり",         // name
        SE_PV_CLIFF_FALL_HANG,  // 0x4
        0,                      // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "声崖つかまり終了",  // name
        SE_PV_CLIFF_CLIMB,   // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "声炎ダメージ中",  // name
        SE_PV_BURN_RUN,    // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "声炎ダメージ終了",  // name
        SE_PV_BURN_RECOVER,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "声氷ダメージ終了",    // name
        SE_PV_FREEZE_RECOVER,  // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声針ダメージ",       // name
        SE_PV_NEEDLE_DAMAGE,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "声針ダメージ中",         // name
        SE_PV_NEEDLE_DAMAGE_RUN,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "声針ダメージ終了",           // name
        SE_PV_NEEDLE_DAMAGE_RECOVER,  // 0x4
        0,                            // 0x8
        0,                            // 0xC
        0,                            // 0x10
        0,                            // 0x14
    },
    {
        "声水中ダメージ",      // name
        SE_PV_DAMAGE_S_WATER,  // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声水中最終ダメージ",     // name
        SE_PV_LAST_DAMAGE_WATER,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "声冷水ダメージ",         // name
        SE_PV_COLD_WATER_DAMAGE,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "声スケキヨ開始",  // name
        SE_PV_BURY_HEAD,   // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "声スケキヨ終了",         // name
        SE_PV_BURY_HEAD_RECOVER,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "声足埋まり開始",  // name
        SE_PV_BURY_FOOT,   // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "声足埋まり終了",         // name
        SE_PV_BURY_FOOT_RECOVER,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "声あくび",  // name
        SE_PV_YAWN,  // 0x4
        0,           // 0x8
        0,           // 0xC
        0,           // 0x10
        0,           // 0x14
    },
    {
        "声いびき１",   // name
        SE_PV_SLEEP_1,  // 0x4
        0,              // 0x8
        0,              // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "声いびき２",   // name
        SE_PV_SLEEP_2,  // 0x4
        0,              // 0x8
        0,              // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "声噴水ジャンプ",  // name
        SE_PV_UPSET,       // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "声ランニングキック",  // name
        SE_PV_KICK,            // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "声砂脱出",     // name
        SE_PV_LIFT_UP,  // 0x4
        0,              // 0x8
        0,              // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "声投げられ",  // name
        SE_PV_THROWN,  // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "声慌て",     // name
        SE_PV_UPSET,  // 0x4
        0,            // 0x8
        0,            // 0xC
        0,            // 0x10
        0,            // 0x14
    },
    {
        "声しゃがむ",  // name
        SE_PV_SQUAT,   // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "声砂沈み",     // name
        SE_PV_DIE_MUD,  // 0x4
        0,              // 0x8
        0,              // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "声砂沈み死亡",  // name
        SE_PV_MUD_SINK,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "声沼沈み",     // name
        SE_PV_DIE_MUD,  // 0x4
        0,              // 0x8
        0,              // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "声沼沈み死亡",  // name
        SE_PV_MUD_SINK,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "足音左",          // name
        SE_PM_FOOTNOTE_L,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "足音右",          // name
        SE_PM_FOOTNOTE_R,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "ジャンプ踏切",    // name
        SE_PM_JUMP,        // 0x4
        0x4000000,         // 0x8
        "水跳ねジャンプ",  // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "着地",        // name
        SE_PM_LAND,    // 0x4
        0x4000000,     // 0x8
        "水跳ね着地",  // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "重い着地",        // name
        SE_PM_LAND_HEAVY,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "掴み",          // name
        SE_PM_GRAB_OBJ,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "ぶらさがり",  // name
        SE_PM_HAND,    // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "尻ドロップ回転",   // name
        SE_PM_PRE_HIPDROP,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "尻ドロップ着地",  // name
        SE_PM_HIPDROP,     // 0x4
        0x4000000,         // 0x8
        "水跳ね着地",      // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "スピン尻ドロップ回転",    // name
        SE_PM_SPIN_HIP_DROP_TURN,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "スピン尻ドロップ落下",       // name
        SE_PM_LV_SPIN_HIP_DROP_FALL,  // 0x4
        0x1000000,                    // 0x8
        0,                            // 0xC
        0,                            // 0x10
        0,                            // 0x14
    },
    {
        "中ジャンプ",  // name
        SE_PM_JUMP_M,  // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "大ジャンプ",  // name
        SE_PM_JUMP_L,  // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "後ジャンプ",     // name
        SE_PM_JUMP_TURN,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "幅ジャンプ",     // name
        SE_PM_JUMP_LONG,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "トルネードジャンプ",  // name
        SE_PM_TWIST_JUMP,      // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "スピンジャンプ",   // name
        SE_PM_SPIN_ATTACK,  // 0x4
        0x4000000,          // 0x8
        "水跳ねジャンプ",   // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "トランポリンジャンプ小",  // name
        SE_OJ_TRAMPOLINE_BOUND_S,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "トランポリンジャンプ大",  // name
        SE_OJ_TRAMPOLINE_BOUND_L,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "スリップアップ",  // name
        SE_PM_SLIP_UP,     // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "ダメージ",      // name
        SE_PM_DAMAGE_S,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "炎ダメージ",  // name
        SE_PM_BURN,    // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "針ダメージ",         // name
        SE_PM_NEEDLE_DAMAGE,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "電気ダメージ",     // name
        SE_PM_ELEC_DAMAGE,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "壁反射",             // name
        SE_PM_SPIN_HIT_WALL,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "プレスダメージ",      // name
        SE_PM_DAMAGE_STOMPED,  // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "氷ダメージ",      // name
        SE_PM_ICE_DAMAGE,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "倒れ",            // name
        SE_PM_FALLDOWN_S,  // 0x4
        0x4000000,         // 0x8
        "水跳ね左足",      // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "吹っ飛び倒れ",    // name
        SE_PM_FALLDOWN_M,  // 0x4
        0x4000000,         // 0x8
        "水跳ね着地",      // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "坂滑り",                 // name
        SE_PM_LV_SLIP_SLIP_CODE,  // 0x4
        0x9000000,                // 0x8
        "水面滑り",               // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "ルイージ滑り",            // name
        SE_PM_LV_LUIGI_WALK_SLIP,  // 0x4
        0x9000000,                 // 0x8
        "水面滑り",                // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "パンチ風切り",     // name
        SE_PM_PUNCH_SHOOT,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "炎ダメージ復帰バウンド",  // name
        SE_PM_BURN_JUMP,           // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "炎ダメージ炎上中",  // name
        SE_PM_LV_BURNING,    // 0x4
        0x1000000,           // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "氷ダメージ終了",          // name
        SE_PM_ICE_DAMAGE_RECOVER,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "引き戻し基本",           // name
        SE_PM_LV_PULL_BACK_BASE,  // 0x4
        0x1000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "引き戻し浮遊",          // name
        SE_PM_LV_PULL_BACK_FLY,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "引き戻し泡破裂",        // name
        SE_OJ_GCAPTURE_RELEASE,  // 0x4
        0,                       // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "はねとばされ",             // name
        SE_PM_LV_FLIP_DAMAGE_TURN,  // 0x4
        0x1000000,                  // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "結界ヒット",                // name
        SE_PM_KAMECK_BARRIER_BOUND,  // 0x4
        0,                           // 0x8
        0,                           // 0xC
        0,                           // 0x10
        0,                           // 0x14
    },
    {
        "壁衝突",             // name
        SE_PM_WALL_HIT_BODY,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "投げられ",       // name
        SE_PM_FLIP_AWAY,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "ヘリコプタージャンプ",  // name
        SE_PM_HELI_JUMP,         // 0x4
        0,                       // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "滞空開始",                 // name
        SE_PM_HELI_JUMP_AIR_START,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "滞空中",                // name
        SE_PM_LV_HELI_JUMP_AIR,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "ホッパー跳ね返り",  // name
        SE_PM_HOPPER_BOUND,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "ホッパージャンプ溜め",    // name
        SE_PM_LV_HOPPER_PRE_JUMP,  // 0x4
        0x1000000,                 // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "ホッパージャンプ",  // name
        SE_PM_HOPPER_JUMP,   // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "テレサ浮遊",               // name
        SE_PM_LV_TERESA_MARIO_FLY,  // 0x4
        0x1000000,                  // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "テレサ踏ん張り",   // name
        SE_PM_LV_AIR_WALK,  // 0x4
        0x1000000,          // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "テレサ壁反射",            // name
        SE_PM_TERESA_MARIO_BOUND,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "テレサ消える",             // name
        SE_PM_TERESA_MARIO_VANISH,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "テレサ現れる",             // name
        SE_PM_TERESA_MARIO_APPEAR,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "テレサ風に乗る",            // name
        SE_PM_LV_T_MARIO_RIDE_WIND,  // 0x4
        0x1000000,                   // 0x8
        0,                           // 0xC
        0,                           // 0x10
        0,                           // 0x14
    },
    {
        "ハチ飛行中",            // name
        SE_PM_LV_BEE_MARIO_FLY,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "スケキヨ開始",    // name
        SE_PM_BURY_START,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "スケキヨ終了",  // name
        SE_PM_BURY_END,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "スケキヨ終了スピン",  // name
        SE_PM_BURY_END_SPIN,   // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "スケート足",  // name
        SE_PM_SKATE,   // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "スケート滑り",       // name
        SE_PM_LV_SKATE_SLIP,  // 0x4
        0x1000000,            // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "スケートジャンプ",  // name
        SE_PM_SKATE_JUMP,    // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "スケート着地",    // name
        SE_PM_SKATE_LAND,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "スケートスピン",  // name
        SE_PM_SKATE_SPIN,  // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "ハチ壁くっつき",     // name
        SE_PM_BEE_WALL_LAND,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "ハチ壁歩き左",         // name
        SE_PM_BEE_WALL_WALK_L,  // 0x4
        0,                      // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "ハチ壁歩き右",         // name
        SE_PM_BEE_WALL_WALK_R,  // 0x4
        0,                      // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "ハチ壁ジャンプ",     // name
        SE_PM_BEE_WALL_JUMP,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "砂沈み",            // name
        SE_PM_LV_SAND_SINK,  // 0x4
        0x1000000,           // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "砂脱出",        // name
        SE_PM_SAND_OUT,  // 0x4
        0,               // 0x8
        0,               // 0xC
        0,               // 0x10
        0,               // 0x14
    },
    {
        "砂強制沈み",              // name
        SE_PM_LV_SAND_SINK_FORCE,  // 0x4
        0x1000000,                 // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "砂死亡",           // name
        SE_PM_LV_SAND_DIE,  // 0x4
        0x1000000,          // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "沼強制沈み",             // name
        SE_PM_LV_MUD_SINK_FORCE,  // 0x4
        0x1000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "沼死亡",          // name
        SE_PM_LV_MUD_DIE,  // 0x4
        0x1000000,         // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "毒沼脱出",            // name
        SE_PM_POISON_MUD_OUT,  // 0x4
        0,                     // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "毒沼ダメージ",           // name
        SE_PM_POISON_MUD_DAMAGE,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "ダークマター沈み",       // name
        SE_PM_LV_DARK_MATTER_IN,  // 0x4
        0x1000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "惑星貫通中",            // name
        SE_PM_LV_WARP_STRAIGHT,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "惑星貫通終了",           // name
        SE_PM_WARP_STRAIGHT_END,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "フー滞空中",           // name
        SE_PM_LV_FOO_FLY_WAIT,  // 0x4
        0x1000000,              // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "フー加速",       // name
        SE_PM_FOO_ACCEL,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "フー飛行中",         // name
        SE_PM_LV_FOO_FLYING,  // 0x4
        0x1000000,            // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "フーブレーキ",   // name
        SE_PM_FOO_BRAKE,  // 0x4
        0,                // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "ワープポッド入り",  // name
        SE_PM_WARP_POD_IN,   // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "ワープポッド出",    // name
        SE_PM_WARP_POD_OUT,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "ワープポッド移動",      // name
        SE_PM_LV_WARP_POD_MOVE,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "スペシャルダッシュ強",   // name
        SE_PM_RACE_START_DASH_L,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "スペシャルダッシュ弱",   // name
        SE_PM_RACE_START_DASH_S,  // 0x4
        0,                        // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "ダッシュ加速強成功",  // name
        SE_SY_GET_DASH_RING,   // 0x4
        0x2000000,             // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "ブラックホール吸い込まれ",  // name
        SE_PM_BLACK_HOLE_IN,         // 0x4
        0,                           // 0x8
        0,                           // 0xC
        0,                           // 0x10
        0,                           // 0x14
    },
    {
        "水歩行突入",        // name
        SE_PM_WALK_TO_SWIM,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "水歩行脱出",        // name
        SE_PM_SWIM_TO_WALK,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "水面ウエイト",       // name
        SE_PM_WAIT_ON_WATER,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "水中ウエイト",          // name
        SE_PM_WAIT_UNDER_WATER,  // 0x4
        0,                       // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "水落下突入",         // name
        SE_PM_DIVE_TO_WATER,  // 0x4
        0,                    // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "水ジャンプ脱出",       // name
        SE_PM_JUMP_FROM_WATER,  // 0x4
        0,                      // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "水面一掻き",               // name
        SE_PM_SWIM_ACCEL_ON_WATER,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "水中一掻き",               // name
        SE_PM_SWIM_ACCEL_IN_WATER,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "水面バタ足",            // name
        SE_PM_LV_SWIM_ON_WATER,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "水中バタ足",            // name
        SE_PM_LV_SWIM_IN_WATER,  // 0x4
        0x1000000,               // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "水面潜り",                // name
        SE_PM_DIVE_WATER_ROLLING,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "水中潜り",                // name
        SE_PM_DIVE_FAST_IN_WATER,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "水面スピン開始",           // name
        SE_PM_TORNADE_ON_WATER_ST,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "水面スピン",               // name
        SE_PM_LV_TORNADE_ON_WATER,  // 0x4
        0x1000000,                  // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "水中スピン開始",           // name
        SE_PM_TORNADE_IN_WATER_ST,  // 0x4
        0,                          // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "水中スピン",               // name
        SE_PM_LV_TORNADE_IN_WATER,  // 0x4
        0x1000000,                  // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "水面ダメージ",         // name
        SE_PM_DAMAGE_ON_WATER,  // 0x4
        0,                      // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "水中ダメージ",         // name
        SE_PM_DAMAGE_IN_WATER,  // 0x4
        0,                      // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "亀ジェット泳ぎ",          // name
        SE_OJ_LV_TURTLE_JET_SWIM,  // 0x4
        0x1000000,                 // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "水底接触",                 // name
        SE_PM_LV_TURTLE_SEA_SMOKE,  // 0x4
        0x1000000,                  // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "亀壁ヒット",              // name
        SE_OJ_TURTLE_JET_BOUND_W,  // 0x4
        0,                         // 0x8
        0,                         // 0xC
        0,                         // 0x10
        0,                         // 0x14
    },
    {
        "亀ブレーキ",        // name
        SE_PM_TURTLE_BRAKE,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "亀加速",            // name
        SE_PM_TURTLE_ACCEL,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "水跳ね左足",        // name
        SE_PM_FOOTNOTE_L_W,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "水跳ね右足",        // name
        SE_PM_FOOTNOTE_R_W,  // 0x4
        0,                   // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "水跳ね左足小",          // name
        SE_PM_FOOTNOTE_SUB_L_W,  // 0x4
        0,                       // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "水跳ね右足小",          // name
        SE_PM_FOOTNOTE_SUB_R_W,  // 0x4
        0,                       // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "水跳ねジャンプ",  // name
        SE_PM_JUMP_W,      // 0x4
        0,                 // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "水跳ね着地",  // name
        SE_PM_LAND_W,  // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "水跳ね手",    // name
        SE_PM_HAND_W,  // 0x4
        0,             // 0x8
        0,             // 0xC
        0,             // 0x10
        0,             // 0x14
    },
    {
        "水弾かれ",         // name
        SE_PM_WATER_BOUND,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "水面滑り",           // name
        SE_PM_LV_SLIP_WATER,  // 0x4
        0x1000000,            // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "トルネード風",       // name
        SE_PM_LV_TWIST_WIND,  // 0x4
        0x1000000,            // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "スリップ",     // name
        SE_PM_LV_SLIP,  // 0x4
        0x9000000,      // 0x8
        "水面滑り",     // 0xC
        0,              // 0x10
        0,              // 0x14
    },
    {
        "最後の一撃",       // name
        SE_PM_LAST_DAMAGE,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "空中ふんばり",     // name
        SE_PM_LV_AIR_WALK,  // 0x4
        0x1000000,          // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "スピン許可",       // name
        SE_PM_SPIN_ENABLE,  // 0x4
        0,                  // 0x8
        0,                  // 0xC
        0,                  // 0x10
        0,                  // 0x14
    },
    {
        "スピン回復終了",        // name
        SE_PM_SPIN_RECOVER_END,  // 0x4
        0,                       // 0x8
        0,                       // 0xC
        0,                       // 0x10
        0,                       // 0x14
    },
    {
        "トルネード飛行",         // name
        SE_PM_LV_TORNADE_FLYING,  // 0x4
        0x1000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "ライフ警告",         // name
        SE_SY_LV_LIFE_ALERT,  // 0x4
        0x3000000,            // 0x8
        0,                    // 0xC
        0,                    // 0x10
        0,                    // 0x14
    },
    {
        "ライフ回復",        // name
        SE_SY_LIFE_RECOVER,  // 0x4
        0x2000000,           // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "水中ライフ減少",      // name
        SE_SY_WATER_LIFE_DEC,  // 0x4
        0x2000000,             // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "酸素減少警告",      // name
        SE_SY_OXYGEN_ALERT,  // 0x4
        0x2000000,           // 0x8
        0,                   // 0xC
        0,                   // 0x10
        0,                   // 0x14
    },
    {
        "無酸素警告",             // name
        SE_SY_OXYGEN_ZERO_ALERT,  // 0x4
        0x2000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "酸素回復",            // name
        SE_SY_INC_OXYGEN_ONE,  // 0x4
        0x2000000,             // 0x8
        0,                     // 0xC
        0,                     // 0x10
        0,                     // 0x14
    },
    {
        "酸素完全回復",         // name
        SE_SY_INC_OXYGEN_FULL,  // 0x4
        0x2000000,              // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "水面酸素回復",           // name
        SE_SY_LV_RECOVER_OXYGEN,  // 0x4
        0x3000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "水面酸素完全回復",         // name
        SE_SY_RECOVER_OXYGEN_FULL,  // 0x4
        0x2000000,                  // 0x8
        0,                          // 0xC
        0,                          // 0x10
        0,                          // 0x14
    },
    {
        "無酸素ダメージ",  // name
        SE_SY_NO_OXYGEN,   // 0x4
        0x2000000,         // 0x8
        0,                 // 0xC
        0,                 // 0x10
        0,                 // 0x14
    },
    {
        "ハチ体力回復",               // name
        SE_SY_LV_INC_AIR_WALK_TIMER,  // 0x4
        0x3000000,                    // 0x8
        0,                            // 0xC
        0,                            // 0x10
        0,                            // 0x14
    },
    {
        "ハチ体力完全回復",          // name
        SE_SY_INC_AIR_WALK_TIMER_F,  // 0x4
        0x2000000,                   // 0x8
        0,                           // 0xC
        0,                           // 0x10
        0,                           // 0x14
    },
    {
        "ハチ体力切れ",           // name
        SE_SY_NO_AIR_WALK_TIMER,  // 0x4
        0x2000000,                // 0x8
        0,                        // 0xC
        0,                        // 0x10
        0,                        // 0x14
    },
    {
        "変身解除",       // name
        SE_SY_MORPH_END,  // 0x4
        0x2000000,        // 0x8
        0,                // 0xC
        0,                // 0x10
        0,                // 0x14
    },
    {
        "呪い解除",             // name
        SE_SY_MORPH_TO_NORMAL,  // 0x4
        0x2000000,              // 0x8
        0,                      // 0xC
        0,                      // 0x10
        0,                      // 0x14
    },
    {
        "",  // name
        0,   // 0x4
        0,   // 0x8
        0,   // 0xC
        0,   // 0x10
        0,   // 0x14
    },
};

struct SoundSwapList {
    const char* name;

    u32 offset1;
    u32 offset2;
    u32 offset3;
};

SoundSwapList soundswaplist[] = {{"", 0, 0, 0}};

u32 Mario::initSoundTable(SoundList* list, u32 globalTablePosition) {
    u32* pSwapOffset = reinterpret_cast< u32* >(soundswaplist) + globalTablePosition;
    u32 count = 0;
    s32 i = 0;
    while (true) {
        SoundList* pEntry = list + i;
        if (pEntry->name[0] == '\0') {
            break;
        }

        pEntry->_10 = 0;
        pEntry->_14 = pEntry->_4;

        if (globalTablePosition != 0) {
            s32 swapIndex = 0;
            while (true) {
                const SoundSwapList* pSwapEntry = soundswaplist + swapIndex;
                if (pSwapEntry->name[0] == '\0') {
                    break;
                }

                if (strcmp(pEntry->name, pSwapEntry->name) == 0) {
                    u32 soundID = pSwapOffset[swapIndex * 4];
                    if (soundID != 0) {
                        pEntry->_14 = soundID;
                    }
                    break;
                }

                swapIndex++;
            }
        }

        count++;
        i++;
    }

    return count;
}

void Mario::initSound() {
    u32 count = initSoundTable(soundlist, 0);
    _96C = new HashSortTable(count);
    for (u32 i = 0; i < count; i++) {
        _96C->add(soundlist[i].name, i, false);
    }
    _96C->sort();
    _970 = nullptr;
}

bool Mario::playSoundJ(const char* pSoundName, s32 timing) {
    // FIXME: Keep this goto dispatch shape for match; revisit with a decomp.me scratch before restructuring.
    u32 index;
    if (_96C->search(pSoundName, &index)) {
        s32 type = soundlist[index]._8._4[0] & 0x3;

        if (type == 2) {
            goto systemSound;
        }

        if (type < 2) {
            if (type == 0) {
                goto actorSound;
            }

            if (type >= 0) {
                goto levelSound;
            }

            goto typeEnd;
        }

        if (type >= 4) {
            goto typeEnd;
        }

        goto systemLevelSound;

    actorSound: {
        JAISoundID soundID(soundlist[index]._14);
        MR::startSound(mActor, soundID, timing);
    }
        goto typeEnd;

    systemSound: {
        JAISoundID soundID(soundlist[index]._14);
        MR::startSystemSE(soundID, timing);
    }
        goto typeEnd;

    levelSound: {
        JAISoundID soundID(soundlist[index]._14);
        MR::startLevelSound(mActor, soundID, timing);
    }
        goto typeEnd;

    systemLevelSound: {
        JAISoundID soundID(soundlist[index]._14);
        MR::startSystemLevelSE(soundID, timing);
    }

    typeEnd:
        s32 recurType = soundlist[index]._8._4[0];
        recurType &= ~0x3;
        if (recurType == 0x8) {
            goto recurse;
        }

        if (recurType >= 0x8) {
            goto recurseEnd;
        }

        if (recurType == 0x4) {
            goto recurse;
        }

        goto recurseEnd;

    recurse:
        if (mDrawStates.mIsUnderwater || mDrawStates._13) {
            playSoundJ(soundlist[index]._C, -1);
        }

    recurseEnd:;
    }

    bool isFound = _96C->search("声", pSoundName, &index);
    if (isFound) {
        JAISoundID soundID(soundlist[index]._14);
        return MR::startSound(mActor, soundID, timing);
    }

    return isFound;
}

void Mario::stopSoundJ(const char* pSoundName, u32 delay) {
    u32 index;
    if (_96C->search(pSoundName, &index)) {
        s32 stopType = soundlist[index]._8._4[0] & 0x3;

        if (stopType != 2) {
            if (stopType >= 2) {
            } else {
                if (stopType == 0) {
                    JAISoundID soundID(soundlist[index]._14);
                    MR::stopSound(mActor, soundID, delay);
                } else {
                }
            }
        } else {
            JAISoundID soundID(soundlist[index]._14);
            MR::stopSystemSE(soundID, delay);
        }
    }

    if (_96C->search("声", pSoundName, &index)) {
        JAISoundID soundID(soundlist[index]._14);
        MR::stopSound(mActor, soundID, delay);
    }
}

void Mario::startBas(const char* pAnimName, bool arg2, f32 startFrame, f32 speed) {
    if (mActor->mSoundObject) {
        ResourceHolder* pHolder = MR::getResourceHolder(mActor);
        const JAUSoundAnimation* pRes = nullptr;

        if (pAnimName && pHolder->mBasResTable->isExistRes(pAnimName)) {
            pRes = static_cast< JAUSoundAnimation* >(pHolder->mBasResTable->getRes(pAnimName));
        }

        if (pRes) {
            mActor->mSoundObject->startAnimation(pRes, arg2, startFrame, speed);
        } else {
            mActor->mSoundObject->removeAnimation();
        }

        _970 = pAnimName;
    }
}

bool Mario::isRunningBas(const char* pAnimName) const {
    if (!_970) {
        return false;
    }

    if (strcmp(_970, pAnimName) == 0) {
        return true;
    }

    return false;
}

void Mario::skipBas(f32 frame) {
    mActor->mSoundObject->skip(frame);
}

void Mario::playSoundTeresaFlying() {
    if (getPlayerMode() != 6) {
        return;
    }

    s32 timing = 100;
    if (getCurrentStatus() == 0x1C) {
        if (mWait->_14 == 0) {
            timing = 100 - static_cast< s32 >(mWait->_16);
            if (timing < 0) {
                timing = 0;
            }
        } else {
            timing = 0;
        }
    }

    playSound("テレサ浮遊", timing);
}

void Mario::playSoundTrampleCombo(u8 combo) {
    if (combo >= 7) {
        return;
    }
    MR::startSystemSE("SE_SY_TRAMPLE_COMBO", combo);
}

void Mario::setSeVersion(u32 version) {
    MR::setSeVersion(mActor, version);
}
