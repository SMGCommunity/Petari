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
        TVec3f horizontalToTalkActor;
        TVec3f toTalkActor;
        TVec3f* pTrans = &getTrans();
        toTalkActor.setPS2(mTalkActor->mPosition);
        JMathInlineVEC::PSVECSubtract(&toTalkActor, pTrans, &toTalkActor);
        MR::vecKillElement(toTalkActor, getAirGravityVec(), &horizontalToTalkActor);
        if (!MR::normalizeOrZero(&horizontalToTalkActor)) {
            getPlayer()->setFrontVecKeepUp(horizontalToTalkActor, (u32)0x10);
            if (!getPlayer()->mMovementStates._37) {
                getPlayer()->setLastNonFixMoveVec(horizontalToTalkActor);
            }
            else if (isPlayerModeTeresa()) {
                TVec3f* pCamDirZ = &getCamDirZ();
                MR::vecKillElement(horizontalToTalkActor, *pCamDirZ, pCamDirZ);
                if (!MR::normalizeOrZero(&horizontalToTalkActor)) {
                    getPlayer()->setLastNonFixMoveVec(horizontalToTalkActor);
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
