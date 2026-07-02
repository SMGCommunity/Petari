#include "Game/AudioLib/AudBgmKeeper.hpp"
#include "Game/AudioLib/AudBgm.hpp"

AudBgmKeeper::AudBgmKeeper() : mSingleBgmActiveFlags(0), mMultiBgmActiveFlags(0) {
}

AudBgm* AudBgmKeeper::get(BgmType type) {
    AudBgm* bgm;
    switch (type) {
    case BgmType_Single:
        bgm = getValidSingleBgm();
        if (bgm != nullptr) {
            return bgm;
        }
        break;
    case BgmType_Multi:
        bgm = getValidMultiBgm();
        if (bgm != nullptr) {
            return bgm;
        }
        break;
    }

    return nullptr;
}

void AudBgmKeeper::release(AudBgm* pBgm) {
    u32 bgmMask = 1;
    for (int idx = 0; idx < 2; idx++) {
        if (&mSingleBgm[idx] == pBgm) {
            mSingleBgm[idx].init();
            mSingleBgmActiveFlags &= ~bgmMask;
            return;
        }
        bgmMask = (bgmMask & 0x7F) << 1;
    }

    bgmMask = 1;
    for (int idx = 0; idx < 2; idx++) {
        if (&mMultiBgm[idx] == pBgm) {
            mMultiBgm[idx].init();
            mMultiBgmActiveFlags &= ~bgmMask;
            return;
        }
        bgmMask = (bgmMask & 0x7F) << 1;
    }
}

AudSingleBgm* AudBgmKeeper::getValidSingleBgm() {
    u32 bgmMask = 1;
    for (int idx = 0; idx < 2; idx++) {
        if ((mSingleBgmActiveFlags & bgmMask) == 0) {
            mSingleBgmActiveFlags |= bgmMask;
            return &mSingleBgm[idx];
        }
        bgmMask = (bgmMask & 0x7F) << 1;
    }
    return nullptr;
}

AudMultiBgm* AudBgmKeeper::getValidMultiBgm() {
    u32 bgmMask = 1;
    for (int idx = 0; idx < 2; idx++) {
        if ((mMultiBgmActiveFlags & bgmMask) == 0) {
            mMultiBgmActiveFlags |= bgmMask;
            return &mMultiBgm[idx];
        }
        bgmMask = (bgmMask & 0x7F) << 1;
    }
    return nullptr;
}
