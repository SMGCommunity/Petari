#include "Game/Enemy/JumpGuarder.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    static const f32 sRotateDegree = 3.0f;
    static const s32 sHopStep = 286;
    static const s32 sOpenInt = 74;
    static const s32 sAttack = 360;
    // static const ??? sSensorRes = ???;
    static const s32 sBabyNum = 4;
    static const f32 sAppearHeight = 500.0f;
    static const f32 sAppearDistance = 2000.0f;
    static const f32 sDisappearDistance = 2200.0f;
    static const f32 sHeadOffset = -100.0f;
    static const f32 sBabyVelocity = 16.0f;
    static const f32 sBabyOffset = 64.0f;
    static const f32 sShadowRadius = 140.0f;
    static const f32 sHideShadowRadius = 110.0f;
    static const s32 sHitInt = 60;
    static const s32 sCoinDefault = 4;
    static const f32 sCameraLimitLength = 6250000.0f;
    static const s32 sOpen = 70;
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

JumpEmitter::JumpEmitter(const char* pName)
    : LiveActor(pName), mBodyJointMtx(), mHeadModel(), _C4(0.0f, 0.0f, 0.0f), mIsEventCamera(), mCameraInfo(), mMultEventCamera() {
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
    if (mMultEventCamera == nullptr) {
        return;
    }

    mMultEventCamera->start(0);
    mIsEventCamera = true;
    _C4.set(mPosition);
}

void JumpEmitter::updateEventCamera() {
    if (!mIsEventCamera) {
        return;
    }

    mMultEventCamera->update();

    if (mMultEventCamera->isActive(0) && !MR::isPlayerJumpRising()) {
        mMultEventCamera->nextForce();
    }

    if (::sCameraLimitLength < _C4.squared(*MR::getPlayerCenterPos()) || MR::isOnGroundPlayer() || MR::isPlayerInRush()) {
        endEventCamera();
    }
}

void JumpEmitter::endEventCamera() {
    if (mMultEventCamera == nullptr) {
        return;
    }

    if (mIsEventCamera) {
        mIsEventCamera = false;
        mMultEventCamera->endForceSoon();
    }
}

void JumpEmitter::updateRotate() {
    PartsModel* head = mHeadModel;

    TRot3f mtx;
    mtx.identity();
    mtx.set(getBaseMtx());
    mtx.invert(mtx);

    TVec3f playerPos(*MR::getPlayerPos());
    mtx.mult(playerPos, playerPos);

    playerPos.y = 0.0f;
    if (MR::isNearZero(playerPos)) {
        return;
    }

    MR::normalize(&playerPos);

    // Possible inline.
    f32 newAngle;
    if (playerPos.z == 0.0f) {
        if (playerPos.x >= 0.0f) {
            newAngle = 90.0f;
        } else {
            newAngle = -90.0f;
        }
    } else if (playerPos.z >= 0.0f) {
        newAngle = MR::toDegree(MR::atan2(playerPos.x, playerPos.z));
    } else {
        newAngle = 180.0f - MR::toDegree(MR::atan2(playerPos.x, -playerPos.z));
    }

    f32 angleDiff = MR::repeat(newAngle - head->mRotation.y, -180.0f, 360.0f);

    if (MR::abs(angleDiff) < ::sRotateDegree) {
        head->mRotation.y = newAngle;
    } else {
        head->mRotation.y += ::sRotateDegree * MR::sign(angleDiff);
    }

    head->mRotation.y = MR::repeat(head->mRotation.y, -180.0f, 360.0f);
}

bool JumpGuarder::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (pReceiver->isType(ATYPE_PLAYER_AUTO_JUMP)) {
            TVec3f upVec;
            MR::calcUpVec(&upVec, this);
            MR::setPlayerJumpVec(upVec);
            startEventCamera();
            setNerve(&NrvJumpGuarder::JumpGuarderNrvHopJump::sInstance);
        } else {
            setNerve(&NrvJumpGuarder::JumpGuarderNrvHopStart::sInstance);
        }

        return true;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::forceJumpPlayer(-mGravity);
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg)) {
        if (!isNerve(&NrvJumpGuarder::JumpGuarderNrvHopStart::sInstance)) {
            setNerve(&NrvJumpGuarder::JumpGuarderNrvHopStart::sInstance);
        }

        return true;
    }

    return MR::isMsgStarPieceReflect(msg);
}

bool JumpGuarder::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_GROUP_ATTACK) {
        MR::invalidateClipping(this);
        setNerve(&NrvJumpGuarder::JumpGuarderNrvUp::sInstance);
        return true;
    }

    if (msg == ACTMES_GROUP_HIDE) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvDown::sInstance);
        return true;
    }

    return false;
}

void JumpGuarder::control() {
    PartsModel* head = mHeadModel;
    TMtx34f mtx;
    mtx.identity();

    MR::makeMtxTRS(mtx.mMtx, TVec3f(0.0f, 44.0f, 0.0f), head->mRotation, head->mScale);

    _90.set(mBodyJointMtx);

    TMtx34f mtx2;
    mtx2.concat(_90, mtx);
    _90.set(mtx2);

    mBumpCooldown = MR::max(mBumpCooldown - 1, 0);

    updateEventCamera();
}

void JumpGuarder::exeHide() {
    updateRotate();

    if (MR::isFirstStep(this)) {
        MR::startBck(mHeadModel, "Wait", nullptr);
        MR::startBrk(mHeadModel, "Green");
        MR::validateClipping(this);
        MR::setShadowVolumeSphereRadius(this, nullptr, ::sHideShadowRadius);
    }

    if (MR::enableGroupAttack(this, ::sAppearDistance, ::sAppearHeight)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_ATTACK, this, getSensor("Body"), "Body");
    }
}

void JumpGuarder::exeUp() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Up", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_APPEAR");
    }

    f32 ratio = MR::getBckFrame(this) / MR::getBckCtrl(this)->getEnd();
    MR::setShadowVolumeSphereRadius(this, nullptr, (::sShadowRadius - ::sHideShadowRadius) * ratio + ::sHideShadowRadius);

    if (MR::isBckStopped(this)) {
        MR::setShadowVolumeSphereRadius(this, nullptr, ::sHideShadowRadius);
        setNerve(&NrvJumpGuarder::JumpGuarderNrvWait::sInstance);
    }
}

void JumpGuarder::exeWait() {
    updateRotate();

    if (!MR::enableGroupAttack(this, ::sAppearDistance, ::sAppearHeight)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");
    } else if (enableAttack()) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvPreOpen::sInstance);
    }
}

void JumpGuarder::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_HIDE");
    }

    f32 ratio = 1.0f - MR::getBckFrame(this) / MR::getBckCtrl(this)->getEnd();
    MR::setShadowVolumeSphereRadius(this, nullptr, (::sShadowRadius - ::sHideShadowRadius) * ratio + ::sHideShadowRadius);

    if (MR::isBckStopped(this)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvHide::sInstance);
    }
}

void JumpGuarder::exeHopStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHeadModel, "HopStart", nullptr);
        MR::startBrk(mHeadModel, "OnAndOff");
        MR::startBck(this, "Damage", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_HIT");
        HitSensor* body = getSensor("Body");
        body->mRadius = 120.0f;
    }

    MR::startLevelSound(this, "SE_EM_LV_JGUARDER_SHAKE");

    if (MR::isStep(this, 10)) {
        for (u32 i = 0; i < mNumActiveBabies; i++) {
            mActiveBabies[i]->makeActorDead();
        }

        mNumActiveBabies = 0;
    }

    if (MR::isBckStopped(mHeadModel)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvHopWait::sInstance);
    }
}

void JumpGuarder::exeHopWait() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        MR::startBck(mHeadModel, "HopWait", nullptr);
        getSensor("Jump")->validate();
    }

    MR::startLevelSound(this, "SE_EM_LV_JGUARDER_SHAKE");

    if (MR::isStep(this, ::sHopStep)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvHopEnd::sInstance);
    }
}

void JumpGuarder::exeHopJump() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mHeadModel, "HopJump", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_TRAMPLE");
    }

    if (MR::isBckStopped(mHeadModel)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvHopWait::sInstance);
    }
}

void JumpGuarder::exeHopEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "HopEnd", nullptr);
        MR::startBck(mHeadModel, "HopEnd", nullptr);
        MR::startBrk(mHeadModel, "Green");
        MR::startSound(this, "SE_EM_JGUARDER_CLOSE_SPRING");
    }

    if (MR::isBckStopped(this) && MR::isBckStopped(mHeadModel)) {
        HitSensor* body = getSensor("Body");
        body->mRadius = 145.0f;
        getSensor("Jump")->invalidate();
        setNerve(&NrvJumpGuarder::JumpGuarderNrvWait::sInstance);
    }
}

void JumpGuarder::exePreOpen() {
    updateRotate();

    if (!MR::enableGroupAttack(this, ::sAppearDistance, ::sAppearHeight)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");
    } else if (MR::isStep(this, 0)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvOpen::sInstance);
    }
}

inline void JumpGuarder::addBaby(JumpGuarderBaby* pBaby) {
    s32 idx = mNumActiveBabies++;
    mActiveBabies[idx] = pBaby;
}

void JumpGuarder::exeOpen() {
    if (MR::isLessStep(this, ::sOpen)) {
        updateRotate();
    }

    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Open", nullptr);
        MR::startSound(this, "SE_EM_JGUARDER_SHUTTER_OPEN");

        mNumActiveBabies = 0;
        for (int i = 0; i < mNumBabies; i++) {
            if (MR::isDead(&mBabies[i])) {
                addBaby(&mBabies[i]);
            }
        }

        TVec3f yDir;
        MR::extractMtxYDir(mHeadModel->getBaseMtx(), &yDir);
        TVec3f zDir;
        MR::extractMtxZDir(mHeadModel->getBaseMtx(), &zDir);

        for (u32 i = 0; i < mNumActiveBabies; i++) {
            JumpGuarderBaby* baby = mActiveBabies[i];
            MR::rotateVecDegree(&zDir, yDir, 360.0f / mNumActiveBabies);

            baby->mPosition.set(mPosition + zDir * ::sBabyOffset);
            baby->mVelocity.set(TVec3f(0.0f, 0.0f, 0.0f));

            if (mNumCoins > 0) {
                mNumCoins -= 1;
                baby->appearFromGuarder();
            } else {
                baby->appearFromGuarder();
            }
        }
    }

    if (MR::isLessStep(this, ::sOpen)) {
        TVec3f yDir;
        MR::extractMtxYDir(mHeadModel->getBaseMtx(), &yDir);
        TVec3f zDir;
        MR::extractMtxZDir(mHeadModel->getBaseMtx(), &zDir);

        for (u32 i = 0; i < mNumActiveBabies; i++) {
            JumpGuarderBaby* baby = mActiveBabies[i];
            MR::rotateVecDegree(&zDir, yDir, 360.0f / mNumActiveBabies);

            baby->mPosition.set(mPosition + zDir * ::sBabyOffset);
            baby->mVelocity.set(TVec3f(0.0f, 0.0f, 0.0f));
        }
    }

    if (MR::isStep(this, ::sOpen)) {
        TVec3f yDir;
        MR::extractMtxYDir(mHeadModel->getBaseMtx(), &yDir);
        TVec3f zDir;
        MR::extractMtxZDir(mHeadModel->getBaseMtx(), &zDir);

        for (u32 i = 0; i < mNumActiveBabies; i++) {
            JumpGuarderBaby* baby = mActiveBabies[i];
            MR::rotateVecDegree(&zDir, yDir, 360.0f / mNumActiveBabies);

            baby->mVelocity.set(zDir * mBabyVelocity);
        }

        MR::startSound(this, "SE_EM_JGUARDER_LAUNCH_BABY");
    }

    if (MR::isStep(this, ::sOpenInt)) {
        mNumActiveBabies = 0;
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvClose::sInstance);
    }
}

void JumpGuarder::exeClose() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Close", nullptr);
    }

    MR::startLevelSound(this, "SE_EM_LV_JGUARDER_SHUTTER_CLOSE");

    if (MR::isBckStopped(this)) {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvInter::sInstance);
    }
}

void JumpGuarder::exeInter() {
    updateRotate();

    if (!MR::enableGroupAttack(this, ::sDisappearDistance, ::sAppearHeight)) {
        MR::sendMsgToGroupMember(ACTMES_GROUP_HIDE, this, getSensor("Body"), "Body");
    } else {
        setNerve(&NrvJumpGuarder::JumpGuarderNrvWait::sInstance);
    }
}

JumpGuarder::JumpGuarder(const char* pName)
    : JumpEmitter(pName), mBabies(), mNumBabies(::sBabyNum), mBumpCooldown(), mNumActiveBabies(), mNumCoins(::sCoinDefault),
      mBabyVelocity(::sBabyVelocity) {
    mNumActiveBabies = 0;
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

    MR::addHitSensorMtx(this, "Jump", ATYPE_PLAYER_AUTO_JUMP, 8, ::sShadowRadius, MR::getJointMtx(mHeadModel, "SpringJoint3"),
                        TVec3f(0.0f, ::sHeadOffset, 0.0f));

    MR::addHitSensorMtxEnemy(this, "Body", 8, ::sShadowRadius, MR::getJointMtx(this, "Body"), TVec3f(0.0f, 35.0f, 0.0f));
    getSensor("Body")->setType(29);
    getSensor("Body")->validate();
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
    s32 babySpeedModifier = 0;
    MR::getJMapInfoArg0NoInit(rIter, &mNumCoins);
    MR::getJMapInfoArg1NoInit(rIter, &babySpeedModifier);
    MR::getJMapInfoArg2NoInit(rIter, &mNumBabies);

    switch (babySpeedModifier) {
    case 0:
        mBabyVelocity = ::sBabyVelocity;
        break;
    case 1:
        mBabyVelocity = ::sBabyVelocity / 2.0f;
        break;
    case 2:
        mBabyVelocity = ::sBabyVelocity / 4.0f;
        break;
    }

    mBabies = new JumpGuarderBaby[::sBabyNum];
    MR::declareCoin(this, mNumCoins);

    for (int i = 0; i < ::sBabyNum; i++) {
        JumpGuarderBaby* baby = &mBabies[i];
        baby->mHost = this;
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
            MR::isPlayerStaggering() && mBumpCooldown == 0) {
            mBumpCooldown = ::sHitInt;
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
    for (u32 i = 0; i < mNumActiveBabies; i++) {
        if (mActiveBabies[i] == pActor) {
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
