#pragma once

#include "Game/AudioLib/AudBgm.hpp"

class AudBgmKeeper {
public:
    enum BgmType {};

    /// @brief Creates a new `AudBgmKeeper`.
    AudBgmKeeper();

    AudBgm* get(BgmType);
    void release(AudBgm*);
    AudSingleBgm* getValidSingleBgm();
    AudMultiBgm* getValidMultiBgm();

    /* 0x000 */ AudSingleBgm mSingleBgm[2];
    /* 0x3B8 */ AudMultiBgm mMultiBgm[2];
    /* 0x7B8 */ u8 _7B8;
    /* 0x7B9 */ u8 _7B9;
};
