#include "Game/Enemy/JumpGuarder.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    // static const ??? sRotateDegree = ???;
    // static const ??? sHopStep = ???;
    // static const ??? sOpenInt = ???;
    // static const ??? sAttack = ???;
    // static const ??? sSensorRes = ???;
    // static const ??? sBabyNum = ???;
    // static const ??? sAppearHeight = ???;
    // static const ??? sAppearDistance = ???;
    // static const ??? sDisappearDistance = ???;
    // static const ??? sHeadOffset = ???;
    // static const ??? sBabyVelocity = ???;
    // static const ??? sBabyOffset = ???;
    // static const ??? sShadowRadius = ???;
    // static const ??? sHideShadowRadius = ???;
    // static const ??? sHitInt = ???;
    // static const ??? sCoinDefault = ???;
    // static const ??? sCameraLimitLength = ???;
    // static const ??? sOpen = ???;
};

namespace NrvJumpGuarder {
    NEW_NERVE(JumpGuarderNrvHide, JumpGuarder, Hide);
    NEW_NERVE(JumpGuarderNrvUp, JumpGuarder, Up);
    NEW_NERVE(JumpGuarderNrvWait, JumpGuarder, Wait);
    NEW_NERVE(JumpGuarderNrvDown, JumpGuarder, Down);
    NEW_NERVE(JumpGuarderNrvHopStart, JumpGuarder, HopStart);
    NEW_NERVE(JumpGuarderNrvHopWait, JumpGuarder, HopWait);
    NEW_NERVE(JumpGuarderNrvHopJump, JumpGuarder, HopJump);
    NEW_NERVE(JumpGuarderNrvHopEnd, JumpGuarder, HopEnd);
    NEW_NERVE(JumpGuarderNrvPreOpen, JumpGuarder, PreOpen);
    NEW_NERVE(JumpGuarderNrvOpen, JumpGuarder, Open);
    NEW_NERVE(JumpGuarderNrvClose, JumpGuarder, Close);
    NEW_NERVE(JumpGuarderNrvInter, JumpGuarder, Inter);
};  // namespace NrvJumpGuarder

JumpEmitter::JumpEmitter(const char* pName) : LiveActor(pName) {
    _8C = 0;
    mHeadModel = nullptr;
    _C4.x = 0.0f;
    _C4.y = 0.0f;
    _C4.z = 0.0f;
    _D0 = 0;
    mCameraInfo = nullptr;
    mMultEventCamera = nullptr;
    _90.identity();
}

void JumpEmitter::kill() {
    LiveActor::kill();
    endEventCamera();
}

void JumpEmitter::initEventCamera(const JMapInfoIter& rIter) {
    mCameraInfo = new ActorCameraInfo(rIter);

    if (mCameraInfo->mCameraSetID != -1) {
        mMultEventCamera = new MultiEventCamera();
        mMultEventCamera->setUp(mName, mCameraInfo, 2);
        mMultEventCamera->setEndCameraTypeSoon();
    }
}

void JumpEmitter::startEventCamera() {
    if (mMultEventCamera != nullptr) {
        mMultEventCamera->start(0);
        _D0 = 1;
        _C4.set< f32 >(mPosition);
    }
}

void JumpEmitter::updateEventCamera() {
    if (_D0) {
        mMultEventCamera->update();

        if (mMultEventCamera->isActive(0) && !MR::isPlayerJumpRising()) {
            mMultEventCamera->nextForce();
        }

        if (6250000.0f < _C4.squared(*MR::getPlayerCenterPos()) || MR::isOnGroundPlayer() || MR::isPlayerInRush()) {
            endEventCamera();
        }
    }
}

void JumpEmitter::endEventCamera() {
    if (mMultEventCamera != nullptr) {
        if (_D0) {
            _D0 = 0;
            mMultEventCamera->endForceSoon();
        }
    }
}

void null() {
    TRot3f mtx;
    JMath::gekko_ps_copy12(mtx, mtx);
    TVec3f playerPos(*MR::getPlayerPos());
}

void JumpEmitter::updateRotate() {
    PartsModel* head = mHeadModel;
    TRot3f mtx;
    mtx.identity();
    JMath::gekko_ps_copy12(mtx, getBaseMtx());
    mtx.invert(mtx);
    TVec3f playerPos(*MR::getPlayerPos());
    mtx.mult(playerPos, playerPos);

    playerPos.y = 0.0f;
    if (MR::isNearZero(playerPos)) {
        return;
    }

    MR::normalize(&playerPos);
    mtx.getEuler(head->mRotation);

    // mtx.getEuler(playerPos);
    head->mRotation.y = MR::repeatDegree(head->mRotation.y);
}

JumpGuarder::JumpGuarder(const char* pName) : JumpEmitter(pName), mBabies(), mNumBabies(4), _E4(), _FC(4), _100(16.0f) {
    _F8 = 0;
}

// "Baby Begoman"
JumpGuarderBaby::JumpGuarderBaby() : BegomanBaby("ベビーベーゴマン") {
}

void JumpGuarder::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pReceiver)) {
        if (isHit(pReceiver->mHost)) {
            MR::sendMsgEnemyAttack(pReceiver, pSender);
            if (isNerve(&NrvJumpGuarder::JumpGuarderNrvWait::sInstance) && !MR::isOnGround(pReceiver->mHost)) {
                MR::startBck(this, "Hit", nullptr);
                MR::startSound(this, "SE_EM_JGUARDER_HIT");
            }
        }
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
        if ((isNerve(&NrvJumpGuarder::JumpGuarderNrvWait::sInstance) || isNerve(&NrvJumpGuarder::JumpGuarderNrvHopWait::sInstance)) &&
            MR::isPlayerStaggering() && _E4 == 0) {
            _E4 = 60;
            MR::startBck(this, "Hit", nullptr);
            MR::startSound(this, "SE_EM_JGUARDER_HIT");
        }
    }
}

bool JumpGuarder::enableAttack() {
    for (int i = 0; i < mNumBabies; i++) {
        if (!MR::isDead(&mBabies[i])) {
            continue;
        }

        return getNerveStep() % 0x168 == 0;
    }

    return false;
}

bool JumpGuarder::isHit(const LiveActor* pActor) const {
    for (int i = 0; i < _F8; i++) {
        if (_E8[i] == pActor) {
            return false;
        }
    }

    return true;
}
