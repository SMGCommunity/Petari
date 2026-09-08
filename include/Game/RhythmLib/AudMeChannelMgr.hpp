#pragma once

#include <JSystem/JAudio2/JASChannel.hpp>
#include <JSystem/JAudio2/JASSoundParams.hpp>

class AudMeTrack;
class JASChannel;

class AudMeChannelMgr {
public:
    AudMeChannelMgr(AudMeTrack*);

    bool noteOff(u32, u16);
    void setPauseFlag(bool);

    const JASChannelParams& getChannelParams() const {
        return mChannelParams;
    }

    /* 0x00 */ JASChannel* mChannels[8];
    /* 0x20 */ JASChannelParams mChannelParams;
    /* 0x38 */ u16 _38[8];
    /* 0x48 */ JASSoundParams mParams;
    /* 0x5C */ AudMeTrack* mTrack;
};
