#include "Game/LiveActor/MirrorActor.h"
#include "JSystem/JMath/JMath.h"

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

    register TVec3f* butts = &stack_8;

    __asm {
        psq_l f1, 0x20(r1), 0, 0
        psq_l f0, 0(butts), 0, 0
        psq_l f2, 8(butts), 1, 0 
        ps_sub f0, f0, f1
        psq_l f3, 0x28(r1), 1, 0
        ps_sub f1, f2, f3
        psq_st f0, 0(butts), 0, 0
        psq_st f1, 8(butts), 1, 0
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

void MirrorActor::calcAnim() {
    MR::copyJointAnimation(this, _8C);
    MR::updateMaterial(this);
}

void MirrorActor::calcViewAndEntry() {
    mModelManager->calcView();
}