#pragma once

#include <JSystem/JAudio2/JAISound.hpp>

namespace AudBgmSetting {
    struct BgmSettingInfo {
        /* 0x0 */ s32 mMuteStateIndex;
        /* 0x4 */ s32 mExtraChordIndex;
    };

    struct MuteState {
        /* 0x0 */ u8 mMuteStates[8];  // 16 nibbles
        /* 0x8 */ u8 _8;              // rate 1
        /* 0x9 */ u8 _9;              // rate 2
    };

    struct MultiBgmSet {
        /* 0x00 */ u32 mSeqId;
        /* 0x04 */ u32 mStreamId;
        /* 0x08 */ f32 mBeatMul;
        /* 0x0C */ u32 mIntroBeats;
        /* 0x10 */ u32 mLoopBeats;
        /* 0x14 */ u32 mLoopStartSamples;  // time (sec) = val / 48000
        /* 0x18 */ u32 mLoopEndSamples;    // time (sec) = val / 48000
    };

    const MuteState* getMuteState(JAISoundID, s32);
    u16 getExtraChordNum(JAISoundID, s32);
    u32 getSeqIdForMultiBgm(u32);
    u32 getStreamIdForMultiBgm(u32);
    f32 getBeatMulForMultiBgm(u32);
    u32 getIntroBeatsForMultiBgm(u32);
    u32 getLoopBeatsForMultiBgm(u32);
    u32 getLoopStartSamplesForMultiBgm(u32);
    u32 getLoopEndSamplesForMultiBgm(u32);

};  // namespace AudBgmSetting
