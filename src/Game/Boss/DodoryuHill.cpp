#include "Game/Boss/DodoryuHill.hpp"
#include "Game/Boss/Dodoryu.hpp"

namespace {
    static const char* sDodoryuHillModel[] = {
        "DodoryuHillA",
        "DodoryuHillB",
        "DodoryuHillC",
    };
};  // namespace

DodoryuHill::DodoryuHill(Dodoryu* pHost) : MogucchiHill(pHost, 50, "ドドリュウ塚"), mHost(pHost) {
    _D8 = 3;
    mModelNameTable = ::sDodoryuHillModel;
    mJointName = "MogucchiHill";
    mAutoEffectName = "DodoryuHill";
    mHasLightCtrl = true;

    initWithoutIter();
}

bool DodoryuHill::notifyAppearAttackToPlayer(HitSensor*, HitSensor*) {
    return false;
}
