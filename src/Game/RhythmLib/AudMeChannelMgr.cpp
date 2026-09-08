#include "Game/RhythmLib/AudMeChannelMgr.hpp"
#include "JSystem/JAudio2/JASChannel.hpp"

AudMeChannelMgr::AudMeChannelMgr(AudMeTrack* pTrack) : mTrack(pTrack) {
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
