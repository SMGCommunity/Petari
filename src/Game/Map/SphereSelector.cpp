#include "Game/Map/SphereSelector.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"

namespace NrvSphereSelector {
    NEW_NERVE(SphereSelectorNrvSelectStart, SphereSelector, SelectStart);
    NEW_NERVE(SphereSelectorNrvSelectWait, SphereSelector, SelectWait);
    NEW_NERVE(SphereSelectorNrvSelectCancel, SphereSelector, SelectCancel);
    NEW_NERVE(SphereSelectorNrvConfirmStart, SphereSelector, ConfirmStart);
    NEW_NERVE(SphereSelectorNrvConfirmWait, SphereSelector, ConfirmWait);
    NEW_NERVE(SphereSelectorNrvConfirmCancel, SphereSelector, ConfirmCancel);
    NEW_NERVE(SphereSelectorNrvConfirmed, SphereSelector, Confirm);
};  // namespace NrvSphereSelector

void SphereSelector::exeSelectStart() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_SELECT_START);
    }

    MR::setNerveAtStep(this, &NrvSphereSelector::SphereSelectorNrvSelectWait::sInstance, 60);
}

void SphereSelector::exeConfirmWait() {}

void SphereSelector::exeConfirm() {
    if (MR::isFirstStep(this)) {
        sendMsgToAllActor(ACTMES_SPHERE_SELECTOR_CONFIRMED);
        MR::stopStageBGM(90);
    }
}
