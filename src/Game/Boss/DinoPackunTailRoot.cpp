#include "Game/Boss/DinoPackunTailRoot.hpp"
#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/Util/JointController.hpp"

typedef JointControlDelegator< DinoPackunTailRoot > Delegator;

typedef bool (DinoPackunTailRoot::*func)(TPos3f*, const JointControllerInfo&);

DinoPackunTailRoot::DinoPackunTailRoot(const char* name, DinoPackun* pPackun) : DinoPackunTailNode(name, pPackun) {}

void DinoPackunTailRoot::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    makeActorAppeared();
}

JointController* DinoPackunTailRoot::createJointControllerOwn(LiveActor* pActor, const char* pJointName) {
    return MR::createJointDelegator(this, pActor, &DinoPackunTailRoot::calcJointDirection, (func) nullptr, pJointName);
}

bool DinoPackunTailRoot::calcJointDirection(TPos3f* pOut, const JointControllerInfo& rInfo) {
    TPos3f stack_20;
    stack_20.setInline(*pOut);
    MR::orthogonalize(&stack_20);
    TVec3f stack_14;
    stack_14.set< f32 >(stack_20(0, 2), stack_20(1, 2), stack_20(2, 2));
    _A8.set< f32 >(stack_14);
    TVec3f v12;
    f32 z = pOut->mMtx[2][3];
    f32 y = pOut->mMtx[1][3];
    f32 x = pOut->mMtx[0][3];
    v12.set< f32 >(x, y, z);
    mPosition.set< f32 >(v12);
    return turnJointLocalXDir(pOut, rInfo);
}

void DinoPackunTailRoot::addNodeVelocity(const TVec3f& rVel) {
    return;
}

DinoPackunTailRoot::~DinoPackunTailRoot() {}
