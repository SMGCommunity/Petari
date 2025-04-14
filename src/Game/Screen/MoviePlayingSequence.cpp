#include "Game/Screen/MoviePlayingSequence.hpp"

namespace {
    enum WipeType {
        WipeType_ForceOpenWipeFade = 1,
        WipeType_OpenWipeFade = 2,
        WipeType_SystemWipeWhiteFade = 3,
        WipeType_OpenWipeWhiteFade = 4,
        WipeType_WipeWhiteFade = 5
    };

    struct MoviePlayingInfo {
        const char* _0;
        u32 _4;
        const char* _8;
        u32 _C;
        s32 _10;
        s32 _14;
        WipeType _18;
        s32 _1C;
        WipeType _20;
        s32 _24;
        WipeType _28;
        f32 mVolume;                // 0x2C
    };
};
