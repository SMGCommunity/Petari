#include "Game/LiveActor/MirrorActor.hpp"
#include "Game/LiveActor/MirrorCamera.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "JSystem/JMath/JMath.hpp"

MirrorActor::~MirrorActor() {

}

MirrorActor::MirrorActor(LiveActor *pActor, const char *a2, const char *a3) : LiveActor(a2) {
    _8C = pActor;
    _A0 = 0;
    initModelManagerWithAnm(a3, 0, 0);
}

#ifdef NON_MATCHING
void MirrorActor::init(const JMapInfoIter &rIter) {
    MR::connectToSceneMirrorMapObj(this);
    MR::copyJointAnimation(this, _8C);
    MR::syncMaterialAnimation(this, _8C);
    MR::invalidateClipping(this);

    TBox3f modelBB;
    MR::calcModelBoundingBox(&modelBB, _8C);

    register TVec3f stack_14;
    JMAVECLerp((const Vec*)&modelBB.mMax, (const Vec*)&modelBB.mMin, (Vec*)&stack_14, 0.5f);
    register TVec3f stack_8(modelBB.mMax);

    register TVec3f* ptrStack = &stack_8;

    __asm {
        psq_l f1, 0x20(r1), 0, 0
        psq_l f0, 0(ptrStack), 0, 0
        psq_l f2, 8(ptrStack), 1, 0 
        ps_sub f0, f0, f1
        psq_l f3, 0x28(r1), 1, 0
        ps_sub f1, f2, f3
        psq_st f0, 0(ptrStack), 0, 0
        psq_st f1, 8(ptrStack), 1, 0
    };

    f32 mag = PSVECMag((const Vec*)&stack_8);
    _9C = 0.5f * mag;

    __asm {
        psq_l       f0, 0x14(r1), 0, 0
        lfs         f1, 0x1C(r1)
        psq_st      f0, 0x90(r31), 0, 0
        stfs        f1, 0x98(r31)
    };

    makeActorAppeared();
}
#endif

void MirrorActor::movement() {
    if (MR::isDead(this) || MR::isDead(_8C) || MR::isClipped(_8C) || MR::isHiddenModel(_8C) || isHostInTheOtherSideOfMirror()) {
        if (_A0) {
            _A0 = 1;

            if (!MR::isHiddenModel(this)) {
                MR::disconnectToDrawTemporarily(this);
            }
        }   
    }
    else {
        if (_A0) {
            _A0 = 0;

            if (!MR::isHiddenModel(this)) {
                MR::connectToDrawTemporarily(this);
            }
        }

        LiveActor::movement();
    }
}

void MirrorActor::calcAnim() { 
    MR::copyJointAnimation(this, _8C);
    MR::updateMaterial(this);
}

void MirrorActor::calcViewAndEntry() {
    mModelManager->calcView();
}

#ifdef NON_MATCHING
// weird function call to getHostCenterPos
bool MirrorActor::isHostInTheOtherSideOfMirror() const {
    if (MR::isExistMirrorCamera()) { 
        TVec3f centerPos;
        centerPos = getHostCenterPos();
        f32 dist = MR::getDistanceToMirror(centerPos);
        return dist < 0.0f;
    }

    return false;
}
#endif