#include "Game/RhythmLib/AudRhythmHolder.hpp"
#include "Game/RhythmLib/AudRhythmMeSystem.hpp"

AudRhythmHolder::AudRhythmHolder() : _C(), mSeqParser(), _168() {
    init();
}

void AudRhythmHolder::movement() {
    for (s32 index = 0; index < NUM_RHYTHM_INFO; index++) {
        mInfos[index]._0 = false;
    }
}
