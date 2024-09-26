#include "Game/Map/SphereSelector.hpp"

inline void SphereSelector::exeConfirm() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(0xE4);
        MR::stopStageBGM(0x5A);
    }
}

inline void SphereSelector::exeSelectStart() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(0xE0);
    }

    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance, 0x3C);
}

namespace NrvSphereSelector {
    INIT_NERVE(SphereSelectorNrvSelectStart);
    INIT_NERVE(SphereSelectorNrvSelectWait);
    INIT_NERVE(SphereSelectorNrvSelectCancel);
    INIT_NERVE(SphereSelectorNrvConfirmStart);
    INIT_NERVE(SphereSelectorNrvConfirmWait);
    INIT_NERVE(SphereSelectorNrvConfirmCancel);
    INIT_NERVE(SphereSelectorNrvConfirmed);
}; 