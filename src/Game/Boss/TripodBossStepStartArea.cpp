#include "Game/Boss/TripodBossStepStartArea.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Util.hpp"
#include "Game/Util/PlayerUtil.hpp"

TripodBossStepStartArea::TripodBossStepStartArea(int formType, const char* pName) : AreaObj(formType, pName) {
    _3C = -1;
}

void TripodBossStepStartArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::connectToSceneAreaObj(this);
    _3C = mObjArg0;
}

void TripodBossStepStartArea::movement() {
    if (isInVolume(*MR::getPlayerPos())) {
        MR::requestStartTripodBossStepSequence(_3C);
    }
}

TripodBossStepStartArea::~TripodBossStepStartArea() {
}

const char* TripodBossStepStartArea::getManagerName() const {
    return "TripodBossStepStart";
}
