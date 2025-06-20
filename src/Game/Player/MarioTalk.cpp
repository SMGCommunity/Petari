#include "Game/Player/MarioTalk.hpp"
#include "Game/LiveActor/HitSensorInfo.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

MarioTalk::MarioTalk(MarioActor *mActor) : MarioState(mActor, 0x22), mTalkActor(0), _18(0) {

}

//_468 not cooperating with the comparison
bool MarioTalk::start() {
    bool b90 = mActor->_B90;
    mActor->_B90 = false;
    if (&mActor->_468.x == nullptr) {
        stopAnimationUpper(nullptr, (char*)nullptr);
    }
    changeAnimationNonStop("見る");
    if (b90) {
        mActor->_B90 = true;
    }
    _18 = true;
    return true;
}

bool MarioTalk::close() {
    if (getPlayer()->mMovementStates._37) {
        getPlayer()->setFrontVecKeepUp(getPlayer()->mLastNonFixMoveVec);
    }
    return true;
}

// regswap
bool MarioTalk::update() {
    if (_18) {
        TVec3f stack_14;
        TVec3f stack_8;
        TVec3f* pTrans = &getTrans();
        stack_8.setPS2(mTalkActor->mPosition);
        JMathInlineVEC::PSVECSubtract(&stack_8, pTrans, &stack_8);
        MR::vecKillElement(stack_8, getAirGravityVec(), &stack_14);
        if (!MR::normalizeOrZero(&stack_14)) {
            getPlayer()->setFrontVecKeepUp(stack_14, (u32)0x10);
            if (!getPlayer()->mMovementStates._37) {
                getPlayer()->setLastNonFixMoveVec(stack_14);
            }
            else if (isPlayerModeTeresa()) {
                TVec3f* pCamDirZ = &getCamDirZ();
                MR::vecKillElement(stack_14, *pCamDirZ, pCamDirZ);
                if (!MR::normalizeOrZero(&stack_14)) {
                    getPlayer()->setLastNonFixMoveVec(stack_14);
                }
            }
        }
        _18 = false;
    }
    return true;
}

bool MarioTalk::notice() {
    return true;
}

void Mario::startTalk(const LiveActor *pActor) {
    stopWalk();
    mTalk->mTalkActor = pActor;
    changeStatus(mTalk);
}  

void Mario::endTalk() {
    closeStatus(mTalk);
}

void Mario::setLastNonFixMoveVec(const TVec3f &rSet) {
    JGeometry::setTVec3f((f32*)&rSet, (f32*)&mLastNonFixMoveVec);
}
