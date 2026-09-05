#pragma once

// TODO : these need to be included together in this order to match sinit
// in multiple files. Where should this actually go?
#include "Game/RhythmLib/AudMeTrack.hpp"

#include "Game/RhythmLib/AudMePlayer.hpp"

namespace AudRhythmWrap {
    void rhythmProc();

    inline void newMeTrackMemPool(int n) {
        AudMeTrack::newMemPool(n);
    }
    inline void newMeMemPool(int n) {
        AudMe::newMemPool(n);
    }

};  // namespace AudRhythmWrap
