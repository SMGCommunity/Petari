#include "Game/RhythmLib/AudMeChannelMgr.hpp"
#include "JSystem/JAudio2/JASChannel.hpp"

AudMeChannelMgr::AudMeChannelMgr(AudMeTrack* pTrack) : _20(1.0f), _24(1.0f), _28(0.0f), _2C(0.5f), _30(0.0f), _34(0.0f), mTrack(pTrack) {
    mParams.init();
    for (int idx = 0; idx < 8; idx++) {
        mChannels[idx] = nullptr;
    }

    for (int idx = 0; idx < 8; idx++) {
        _38[idx] = 0;
    }
}

bool AudMeChannelMgr::noteOff(u32 channelNo, u16 directRelease) {
    JASChannel* channel = mChannels[channelNo];
    if (channel == nullptr) {
        return false;
    }

    if (directRelease == 0) {
        channel->release(0);
    } else {
        channel->release(directRelease);
    }
    channel->free();
    mChannels[channelNo] = nullptr;
    return true;
}

void AudMeChannelMgr::setPauseFlag(bool pause) {
    for (int idx = 0; idx < 8; idx++) {
        if (mChannels[idx] != nullptr) {
            mChannels[idx]->setPauseFlag(pause);
        }
    }
}
