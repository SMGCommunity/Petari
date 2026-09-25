#include "Game/Boss/DodoryuHill.hpp"
#include "Game/Boss/Dodoryu.hpp"

namespace {
    inline void setHillTable(MogucchiHill* pHill, const char** pModelNames, u32 count) {
        pHill->_D8 = count;
        pHill->mModelNameTable = pModelNames;
    }
    inline void setHillNames(MogucchiHill* pHill, const char* pJointName, const char* pEffectName) {
        pHill->mJointName = pJointName;
        pHill->mAutoEffectName = pEffectName;
    }

    static const char* sDodoryuHillModel[] = {
        "DodoryuHillA",
        "DodoryuHillB",
        "DodoryuHillC",
    };
};  // namespace

DodoryuHill::DodoryuHill(Dodoryu* pHost) : MogucchiHill(pHost, 50, "ドドリュウ塚"), mHost(pHost) {
    setHillTable(this, ::sDodoryuHillModel, 3);
    const char* pJoint = "MogucchiHill";
    const char* pEffect = "DodoryuHill";
    setHillNames(this, pJoint, pEffect);
    mHasLightCtrl = true;

    initWithoutIter();
}

bool DodoryuHill::notifyAppearAttackToPlayer(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}
