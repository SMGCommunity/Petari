#include "Game/Enemy/SearchBeamer.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/Enemy/AnimScaleController.hpp"

namespace NrvSearchBeamer {
    NEW_NERVE(SearchBeamerNrvNonActive, SearchBeamer, NonActive);
    NEW_NERVE(SearchBeamerNrvCloseWaitFar, SearchBeamer, CloseWaitFar);
    NEW_NERVE(SearchBeamerNrvCloseSearch, SearchBeamer, CloseSearch);
    NEW_NERVE(SearchBeamerNrvCloseWaitNear, SearchBeamer, CloseWaitNear);
    NEW_NERVE(SearchBeamerNrvOpenMouth, SearchBeamer, OpenMouth);
    NEW_NERVE(SearchBeamerNrvBeamPrepare, SearchBeamer, BeamPrepare);
    NEW_NERVE(SearchBeamerNrvBeamStart, SearchBeamer, BeamStart);
    NEW_NERVE(SearchBeamerNrvBeamAim, SearchBeamer, BeamAim);
    NEW_NERVE(SearchBeamerNrvCloseMouth, SearchBeamer, CloseMouth);
    NEW_NERVE(SearchBeamerNrvStopStart, SearchBeamer, StopStart);
    NEW_NERVE(SearchBeamerNrvStop, SearchBeamer, Stop);
    NEW_NERVE(SearchBeamerNrvStopForPlayerOff, SearchBeamer, StopForPlayerOff);
    NEW_NERVE(SearchBeamerNrvRecover, SearchBeamer, Recover);
    NEW_NERVE_ONEND(SearchBeamerNrvDPDSwoon, SearchBeamer, DPDSwoon, DPDSwoon);
};

void SearchBeamer::exeOnEndDPDSwoon() {
    mBindStarPointer->kill();
}

void SearchBeamer::exeStopForPlayerOff() {
    reformDirection(true);
    if (MR::isStep(this, 5)) {
        setNerve(&NrvSearchBeamer::SearchBeamerNrvRecover::sInstance);
    }
}
