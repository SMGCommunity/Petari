#pragma once

#include "JSystem/JAudio2/JASSoundParams.hpp"

class AudMeTrack;
class JASChannel;

class AudMeChannelMgr {
public:
    AudMeChannelMgr(AudMeTrack*);

    bool noteOff(u32, u16);
    void setPauseFlag(bool);

    /* 0x00 */ JASChannel* mChannels[8];
    /* 0x20 */ f32 _20;
    /* 0x24 */ f32 _24;
    /* 0x28 */ f32 _28;
    /* 0x2C */ f32 _2C;
    /* 0x30 */ f32 _30;
    /* 0x34 */ f32 _34;
    /* 0x38 */ u16 _38[8];
    /* 0x48 */ JASSoundParams mParams;
    /* 0x5C */ AudMeTrack* mTrack;
};
