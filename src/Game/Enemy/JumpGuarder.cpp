#include "Game/Enemy/JumpGuarder.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    // static const ??? sRotateDegree = ???;
    // static const ??? sHopStep = ???;
    // static const ??? sOpenInt = ???;
    static const s32 sAttack = 360;
    // static const ??? sSensorRes = ???;
    static const s32 sBabyNum = 4;
    // static const ??? sAppearHeight = ???;
    // static const ??? sAppearDistance = ???;
    // static const ??? sDisappearDistance = ???;
    static const f32 sHeadOffset = -100.0f;
    // static const ??? sBabyVelocity = ???;
    // static const ??? sBabyOffset = ???;
    static const f32 sShadowRadius = 145.0f;
    // static const ??? sHideShadowRadius = ???;
    // static const ??? sHitInt = ???;
    // static const ??? sCoinDefault = ???;
    // static const ??? sCameraLimitLength = ???;
    // static const ??? sOpen = ???;
};  // namespace

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
    mBodyJointMtx = 0;
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

JumpGuarder::JumpGuarder(const char* pName) : JumpEmitter(pName), mBabies(), mNumBabies(::sBabyNum), _E4(), mNumCoins(::sBabyNum), _100(16.0f) {
    _F8 = 0;
}

void JumpGuarder::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("JumpGuarder", nullptr, false);
    // "Jump guarder head"
    mHeadModel = MR::createPartsModelNoSilhouettedMapObj(this, "ジャンプガーダー頭", "JumpGuarderHead", _90);
    MR::initLightCtrl(mHeadModel);
    MR::initDefaultPos(this, rIter);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initHitSensor(2);

    TVec3f offset(0.0f, ::sHeadOffset, 0.0f);
    MR::addHitSensorMtx(this, "Jump", 31, 8, ::sShadowRadius, MR::getJointMtx(mHeadModel, "SpringJoint3"), offset);
    MR::addHitSensorMtxEnemy(this, "SpringJoint3", 8, ::sShadowRadius, MR::getJointMtx(mHeadModel, "SpringJoint3"), offset);
    getSensor("SpringJoint3")->setType(31);
    getSensor("SpringJoint3")->validate();
    getSensor("Jump")->invalidate();
    MR::initShadowVolumeSphere(this, ::sShadowRadius);
    initEffectKeeper(1, nullptr, false);
    initSound(8, false);
    MR::invalidateClipping(this);
    initNerve(&NrvJumpGuarder::JumpGuarderNrvHide::sInstance);
    MR::startBckWithInterpole(this, "Down", 0);
    MR::setBckFrame(this, MR::getBckCtrl(this)->getEnd() - 1);
    MR::calcAnimDirect(this);
    mBodyJointMtx = MR::getJointMtx(this, "Body");
    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchReadB(this, rIter);
    initEventCamera(rIter);
    MR::joinToGroupArray(this, rIter, nullptr, 32);
    makeActorAppeared();
    s32 JMapArg2 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &mNumCoins);
    MR::getJMapInfoArg1NoInit(rIter, &JMapArg2);
    MR::getJMapInfoArg2NoInit(rIter, &mNumBabies);

    switch (JMapArg2) {
    case 0:
        _100 = 16.0f;
        break;
    case 1:
        _100 = 8.0f;
        break;
    case 2:
        _100 = 4.0f;
        break;
    }

    mBabies = new JumpGuarderBaby[::sBabyNum];
    MR::declareCoin(this, mNumCoins);

    for (int i = 0; i < ::sBabyNum; i++) {
        JumpGuarderBaby* baby = &mBabies[i];
        baby->mParent = this;
        baby->mPosition.set(mPosition);
        baby->initWithoutIter();
        baby->makeActorDead();
    }
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
    } else if (MR::isSensorPlayer(pReceiver)) {
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

        return getNerveStep() % ::sAttack == 0;
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

bool MR::enableGroupAttack(LiveActor* pActor, f32 arg2, f32 arg3) {
    LiveActorGroup* group = MR::getGroupFromArray(pActor);

    if (isValidSwitchA(pActor) && !isOnSwitchA(pActor)) {
        return false;
    }

    if (isValidSwitchB(pActor) && !isOnSwitchB(pActor)) {
        return false;
    }

    LiveActor* closest = pActor;
    if (group != nullptr) {
        for (int i = 0; i < group->getObjectCount(); i++) {
            LiveActor* actor = group->getActor(i);
            if (MR::calcDistanceToPlayer(actor) < MR::calcDistanceToPlayer(closest)) {
                closest = actor;
            }
        }
    }

    return isNearPlayerPose(closest, arg2, arg3);
}
