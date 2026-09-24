#include "Game/Boss/DinoPackunTailRoot.hpp"
#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"

typedef JointControlDelegator< DinoPackunTailRoot > Delegator;

typedef bool (DinoPackunTailRoot::*func)(TPos3f*, const JointControllerInfo&);

DinoPackunTailRoot::DinoPackunTailRoot(const char* name, DinoPackun* pPackun) : DinoPackunTailNode(name, pPackun) {
}

void DinoPackunTailRoot::init(const JMapInfoIter& rIter) {
    MR::invalidateClipping(this);
    makeActorAppeared();
}

JointController* DinoPackunTailRoot::createJointControllerOwn(LiveActor* pActor, const char* pJointName) {
    return MR::createJointDelegator(this, pActor, &DinoPackunTailRoot::calcJointDirection, (func) nullptr, pJointName);
}

bool DinoPackunTailRoot::calcJointDirection(TPos3f* pOut, const JointControllerInfo& rInfo) {
    TPos3f stack_20 = pOut;
    MR::orthogonalize(&stack_20);
    TVec3f stack_14;
    stack_20.getZDir(stack_14);
    _A8.set(stack_14);
    TVec3f v12;
    pOut->getTrans(v12);
    mPosition.set(v12);
    return turnJointLocalXDir(pOut, rInfo);
}

void DinoPackunTailRoot::addNodeVelocity(const TVec3f& rVel) {
    return;
}
