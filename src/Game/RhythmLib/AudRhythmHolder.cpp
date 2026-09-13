#include "Game/RhythmLib/AudRhythmHolder.hpp"

AudRhythmHolder::AudRhythmHolder() : mIsPaused(), mSeqParser(), _168() {
    init();
}

void AudRhythmHolder::movement() {
    for (s32 index = 0; index < NUM_RHYTHM_INFO; index++) {
        mInfos[index]._0 = false;
    }
}
