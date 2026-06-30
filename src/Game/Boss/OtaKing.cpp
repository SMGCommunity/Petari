#include "Game/Boss/OtaKing.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Boss/OtaKingLongFoot.hpp"
#include "Game/Boss/OtaKingMagma.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/CocoNutBall.hpp"
#include "Game/Enemy/FireBall.hpp"
#include "Game/Enemy/FireBubble.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/CocoNut.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

#include <cstdio>

namespace NrvOtaKing {
    NEW_NERVE(OtaKingNrvWaitOnSwitch, OtaKing, WaitOnSwitch);
    NEW_NERVE(OtaKingNrvAppearDemo, OtaKing, AppearDemo);
    NEW_NERVE(OtaKingNrvWait, OtaKing, Wait);
    NEW_NERVE(OtaKingNrvThrowCocoNutWait, OtaKing, ThrowCocoNutWait);
    NEW_NERVE(OtaKingNrvThrowCocoNut, OtaKing, ThrowCocoNut);
    NEW_NERVE(OtaKingNrvThrowFireBallWait, OtaKing, ThrowFireBallWait);
    NEW_NERVE(OtaKingNrvThrowFireBall, OtaKing, ThrowFireBall);
    NEW_NERVE(OtaKingNrvDamage, OtaKing, Damage);
    NEW_NERVE(OtaKingNrvPowerUp, OtaKing, PowerUp);
    NEW_NERVE(OtaKingNrvDown, OtaKing, Down);
    NEW_NERVE(OtaKingNrvDownDemo, OtaKing, DownDemo);
    NEW_NERVE(OtaKingNrvAppearStar, OtaKing, AppearStar);
    NEW_NERVE(OtaKingNrvDead, OtaKing, Dead);
    NEW_NERVE(OtaKingNrvHitBackStart, OtaKing, HitBackStart);
    NEW_NERVE(OtaKingNrvHitBack, OtaKing, HitBack);
    NEW_NERVE(OtaKingNrvNearAttack, OtaKing, NearAttack);
    NEW_NERVE(OtaKingNrvBubbleAttack, OtaKing, BubbleAttack);
    NEW_NERVE(OtaKingNrvWaitStartDemo, OtaKing, WaitStartDemo);
};  // namespace NrvOtaKing

namespace {
    const s32 cHP = 3;
    const f32 cThrowDistance = 4000.0f;
    const s32 cWaitFrame = 75;
    const f32 cValidThrowCocoNutDistance = 500.0f;
    const s32 cThrowCocoNutStep = 70;
    const s32 cCocoNutThrowNum = 1;
    const f32 cThrowAngleMin = 2.5f;
    const f32 cThrowAngleMax = 5.0f;
    const s32 cThrowCocoNutWaitFrame = 45;
    const s32 cThrowFireBallStep = 94;
    const f32 cFireBallThrowNum = 3.0f;
    const f32 cFireBallThrowSpeed = 15.0f;
    const f32 cFireBallThrowAngle = 30.0f;
    const s32 cThrowFireBallWaitFrame = 60;
    const s32 cBubbleAppearStep = 100;
    const s32 cBubbleAppearNum = 6;
    const f32 cBubbleAppearRandomAngle = 30.0f;
    const s32 cAppearDemoRippleStepS = 40;
    const s32 cAppearDemoRippleStepL = 190;
    const s32 cCocoNutRippleStep = 30;
    const s32 cFireBallRippleStep = 30;
    const s32 cDownRippleStep = 90;
    const s32 cNearAttackRippleStep = 20;
    const s32 cBubbleAttackRippleStep = 85;
    const s32 cDownSinkStartStep = 80;
    const f32 cBodySensorRadius = 400.0f;
    const f32 cEyeSensorRadius = 1500.0f;
    const f32 cTurnSpeedMax = 1.0f;
    const f32 cTurnSpeedAccel = 0.03f;
    const f32 cPowerStarAppearOffsetY = 100.0f;
    const f32 cStarPieceAppearOffsetY = 300.0f;
    const s32 cStarPieceAppearNum = 8;
    const f32 cStarPieceAppearRange = 60.0f;
    const f32 cStarPieceAppearVelocity = 50.0f;
    const s32 cDamageCrownFallFrame = 73;
    // const s32 cDamageAngryRumbleFrame;
    const f32 cDamageFrontFootClipingHeight = 600.0f;
    const s32 cDownDemoFrame = 300;
    const f32 cDownDemoCocoNutScale = 1.5f;
    const s32 cDownDemoRumbleFrame1 = 105;
    const s32 cDownDemoRumbleFrame2 = 150;
    const s32 cPowerStarExistBgmStartStep = 200;
    const s32 cAppearSeStep0 = 10;
    const s32 cAppearSeStep1 = 205;
    const s32 cAppearSeStep2 = 230;
    const s32 cAppearSeStep3 = 100;
    const s32 cAppearDemoRumblePad01 = 50;
    const s32 cAppearDemoRumblePad02 = 130;
    const s32 cAppearDemoRumblePad03 = 160;
    const s32 cAppearDemoRumblePad04 = 200;
    const s32 cAppearDemoRumblePad05 = 250;
    const s32 cAppearDemoRumblePad06 = 310;
    const s32 cBlurStartStep = 175;
    const s32 cBlurTotalFrame = 70;
    const Vec cLongFootScale = {1.0f, 0.9f, 0.8f};
    const Vec cBubbleAppearVelocity = {0.0f, 37.5f, 15.0f};
    const Vec cSensorOffset = {0.0f, 150.0f, 0.0f};
    const u32 cLongFootDemoBckStep[] = {3, 8, 14, 18};
    const Vec cAppearDemoRotate = {0.0f, 30.0f, 0.0f};
    const Vec cDownDemoRotate = {0.0f, 43.0f, 0.0f};
    const Vec cDownDemoCocoNutPos = {0.0f, 630.0f, 250.0f};
};  // namespace

OtaKing::OtaKing(const char* pName)
    : LiveActor(pName), mMagma(nullptr), mCocoNutBallArray(nullptr), mFireBallArray(nullptr), mFireBubbleArray(nullptr), mThrowPos(nullptr), mHits(0),
      _EC(0), mTurnSpeed(0.0f), mSoundObj(nullptr), mSoundObjPos(gZeroVec), mBubbleAppearPos(nullptr), mDownCocoNut(nullptr), mAnimCamera(nullptr),
      mIsLv2(false), mScaleController(nullptr) {
    MR::zeroMemory(mFeet, sizeof(mFeet));
    MR::zeroMemory(mLongFeet, sizeof(mLongFeet));
    mLongFootMtx.identity();
    MR::zeroMemory(mBckLBuffer, sizeof(mBckLBuffer) + sizeof(mBckRBuffer));
}

void OtaKing::init(const JMapInfoIter& rIter) {
    initMapToolInfo(rIter);
    initModel(rIter);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSensor();
    initEffectKeeper(0, "OtaKing", false);
    MR::setClippingTypeSphere(this, 3000.0f);
    initSound(8, false);
    mSoundObj = new AudAnmSoundObject(&mSoundObjPos, 4, MR::getCurrentHeap());
    mAnimCamera = MR::createActorCameraInfo(rIter);
    MR::initAnimCamera(this, mAnimCamera, "Appear");
    MR::initAnimCamera(this, mAnimCamera, "Down");
    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 24);
    MR::createCenterScreenBlur();

    if (MR::isValidSwitchA(this)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &OtaKing::startAppearDemo));
    }

    MR::tryRegisterDemoCast(this, rIter);
    mScaleController = new AnimScaleController(nullptr);
    initNerve(&NrvOtaKing::OtaKingNrvWait::sInstance);
    makeActorAppeared();
}

void OtaKing::movement() {
    LiveActor::movement();
    MR::copyJointPos(this, "Crown", &mSoundObjPos);
    mSoundObj->process();
}

void OtaKing::makeActorAppeared() {
    mHits = 0;
    LiveActor::makeActorAppeared();

    MR::startBrk(this, "Wait");
    MR::startBpk(this, "Wait");

    if (MR::isValidSwitchA(this)) {
        setNerve(&NrvOtaKing::OtaKingNrvWaitOnSwitch::sInstance);
    } else {
        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
    }
}

void OtaKing::startAppearDemo() {
    if (isNerve(&NrvOtaKing::OtaKingNrvWaitOnSwitch::sInstance)) {
        invalidateClippingAndStartDemo("出現", &NrvOtaKing::OtaKingNrvAppearDemo::sInstance, &NrvOtaKing::OtaKingNrvWaitStartDemo::sInstance);
    }
}

void OtaKing::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool isLv2 = false;
    MR::getJMapInfoArg1NoInit(rIter, &isLv2);
    bool isLv2Flag = isLv2;

    pArchiveList->addArchive(CocoNut::getModelName());
    pArchiveList->addArchive("FireBall");
    pArchiveList->addArchive("FireBubble");
    pArchiveList->addArchive("OtaKingMagma");
    pArchiveList->addArchive("OtaKingMagmaBloom");

    if (!isLv2Flag) {
        pArchiveList->addArchive("OtaKing");
        pArchiveList->addArchive("OtaKingFoot");
        pArchiveList->addArchive("OtaKingLongFoot");
    } else {
        pArchiveList->addArchive("OtaKingLv2");
        pArchiveList->addArchive("OtaKingFootLv2");
    }
}

void OtaKing::control() {
    if (isNerve(&NrvOtaKing::OtaKingNrvDown::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvAppearStar::sInstance) ||
        isNerve(&NrvOtaKing::OtaKingNrvDead::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvAppearDemo::sInstance) ||
        isNerve(&NrvOtaKing::OtaKingNrvDownDemo::sInstance)) {
        mTurnSpeed = 0.0f;
    } else {
        dirToPlayer();
    }
    mScaleController->updateNerve();
}

void OtaKing::calcAndSetBaseMtx() {
    LiveActor::calcAndSetBaseMtx();
    MR::setBaseScale(this, mScaleController->_C.mult(mScale));
}

bool OtaKing::isSensorBodyOrFace(HitSensor* pSensor) {
    return pSensor == getSensor("body") || pSensor == getSensor("face");
}

void OtaKing::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver) && isSensorBodyOrFace(pSender)) {
        if (isNerveValidNearAttack() && MR::sendMsgEnemyAttackMaximum(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
            setNerve(&NrvOtaKing::OtaKingNrvNearAttack::sInstance);
        } else {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool OtaKing::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isSensorBodyOrFace(pReceiver) && MR::isMsgStarPieceReflect(msg)) {
        mScaleController->startHitReaction();
        return true;
    }

    return false;
}

bool OtaKing::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!pSender->isType(ATYPE_COCO_NUT)) {
        return false;
    }

    CocoNutBall* rallyBall = static_cast< CocoNutBall* >(pSender->mHost);

    s32 a1 = mHits;

    if (mIsLv2) {
        a1 = MR::max(mHits, 1);
    }
    bool b1 = rallyBall->_9C <= a1;

    bool isReceiverBody = pReceiver == getSensor("body");

    if (isReceiverBody) {
        if (b1) {
            if (!isNerve(&NrvOtaKing::OtaKingNrvHitBack::sInstance)) {
                setNerve(&NrvOtaKing::OtaKingNrvHitBack::sInstance);
            }

            rallyBall->hitBackToPlayer();
        } else {
            if (MR::isPlayingStageBgm()) {
                MR::startSystemME("ME_RALLY_COMBO_LAST");
            }

            damage();
            return true;
        }
    } else {
        if (b1 && !isNerve(&NrvOtaKing::OtaKingNrvHitBackStart::sInstance)) {
            setNerve(&NrvOtaKing::OtaKingNrvHitBackStart::sInstance);
        }
    }

    return false;
}

bool OtaKing::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    switch (msg)
    case ACTMES_RUSH_END:
        if (pSender->isType(ATYPE_COCO_NUT)) {
            if (!isDamageNerve()) {
                if (isNerve(&NrvOtaKing::OtaKingNrvWait::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance) ||
                    isNerve(&NrvOtaKing::OtaKingNrvHitBackStart::sInstance)) {
                    if (isValidBubbleAttack()) {
                        setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
                    } else {
                        mHits > 0 && isValidThrowFireBall() ? setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance) :
                                                              setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
                    }
                    return true;
                } else if (isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvHitBack::sInstance)) {
                    _EC = 2;
                    return true;
                }
            }
        }
    return false;
}

void OtaKing::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    bool arg1 = false;

    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    mIsLv2 = arg1;
}

void OtaKing::initModel(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(mIsLv2 ? "OtaKing" : "OtaKingLv2", nullptr, false);

    for (int i = 0; i < ARRAY_SIZE(mFeet); i++) {
        mFeet[i] = new PartsModel(this, "前足モデル", mIsLv2 ? "OtaKingFoot" : "OtaKingFootLv2", nullptr, 18, false);
        mFeet[i]->mPosition.set(mPosition);
        mFeet[i]->initWithoutIter();
        MR::initLightCtrl(mFeet[i]);
    }

    mFeet[0]->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);
    mFeet[1]->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);

    initLongFoot(rIter);

    mCocoNutBallArray = new CocoNutBall[3];
    for (int i = 0; i < 3; i++) {
        mCocoNutBallArray[i].initWithoutIter();
        mCocoNutBallArray[i]._8C = this;
        mCocoNutBallArray[i]._BE = true;
        mCocoNutBallArray[i]._D8 = true;
        mCocoNutBallArray[i]._D4 = 5000.0f;

        float arg0;
        if (MR::getJMapInfoArg0NoInit(rIter, &arg0)) {
            mCocoNutBallArray[i]._C0 = arg0;
        }

        float arg2;
        if (MR::getJMapInfoArg2NoInit(rIter, &arg2)) {
            mCocoNutBallArray[i]._C4 = arg2;
        }
    }

    mFireBallArray = new FireBall[6];

    for (int i = 0; i < 6; i++) {
        mFireBallArray[i].initWithoutIter();
        mFireBallArray[i]._8C = this;
    }

    mFireBubbleArray = new FireBubble[6];
    for (int i = 0; i < 6; i++) {
        mFireBubbleArray[i].initWithoutIter();
    }

    mThrowPos = new FixedPosition(this, "Lip", TVec3f(-50.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    mBubbleAppearPos = new FixedPosition(this, TVec3f(0.0f, 10.0f, 500.0f), TVec3f(0.0f, 0.0f, 0.0f));

    mMagma = new OtaKingMagma(this, -1);
    mMagma->initWithoutIter();
    mDownCocoNut = new CocoNutBall;
    mDownCocoNut->mScale.set(::cDownDemoCocoNutScale);
    mDownCocoNut->initWithoutIter();
}

void OtaKing::initSensor() {
    initHitSensor(3);
    MR::addHitSensorEnemy(this, "body", 16, ::cBodySensorRadius, ::cSensorOffset);
    MR::addHitSensorAtJointEnemy(this, "face", "Face", 16, ::cBodySensorRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEnemy(this, "search", 16, ::cEyeSensorRadius, ::cSensorOffset);
}

void OtaKing::dirToPlayer() {
    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    f32 angle = JMAATan2(vec.x, vec.z);

    angle = MR::repeat(MR::toDegree(angle), mRotation.y - 180.0f, 360.0f);

    f32 angleMinRotY = angle - mRotation.y;

    if ((0.0f < angleMinRotY && mTurnSpeed < angleMinRotY) || (angleMinRotY < 0.0f && angleMinRotY < mTurnSpeed)) {
        if (angleMinRotY * mTurnSpeed < 0.0f) {
            mTurnSpeed = 0.0f;
        }
        mRotation.y += mTurnSpeed;

        if (0.0f < angleMinRotY) {
            mTurnSpeed = MR::max(mTurnSpeed + ::cTurnSpeedAccel, ::cTurnSpeedMax);
        } else {
            mTurnSpeed = MR::min(mTurnSpeed - ::cTurnSpeedAccel, -::cTurnSpeedMax);
        }

    } else {
        mRotation.y = angle;
        mTurnSpeed = 0.0f;
    }

    MR::repeatDegree(&mRotation.y);
}

CocoNutBall* OtaKing::getDisappearedCocoNut() {
    for (int i = 0; i < 3; i++) {
        if (MR::isDead(&mCocoNutBallArray[i])) {
            return &mCocoNutBallArray[i];
        }
    }
    return nullptr;
}

FireBall* OtaKing::getDisappearedFireBall() {
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&mFireBallArray[i])) {
            return &mFireBallArray[i];
        }
    }
    return nullptr;
}

FireBubble* OtaKing::getDisappearedBubble() {
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&mFireBubbleArray[i])) {
            return &mFireBubbleArray[i];
        }
    }
    return nullptr;
}

s32 OtaKing::getDisappearedCocoNutNum() const {
    s32 dissappearedNum = 0;
    for (int i = 0; i < 3; i++) {
        if (MR::isDead(&mCocoNutBallArray[i])) {
            dissappearedNum++;
        }
    }
    return dissappearedNum;
}

s32 OtaKing::getDisappearedFireBallNum() const {
    s32 dissappearedNum = 0;
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&mFireBallArray[i])) {
            dissappearedNum++;
        }
    }
    return dissappearedNum;
}

s32 OtaKing::getDisappearedFireBubbleNum() const {
    s32 dissappearedNum = 0;
    for (int i = 0; i < 6; i++) {
        if (MR::isDead(&mFireBubbleArray[i])) {
            dissappearedNum++;
        }
    }
    return dissappearedNum;
}

bool OtaKing::isValidThrowCocoNut() const {
    if (getDisappearedCocoNutNum() == 0) {
        return false;
    }

    f32 distToPlayer = MR::calcDistanceToPlayer(this);
    if (::cThrowDistance < distToPlayer) {
        return false;
    }

    for (int i = 0; i < 3; i++) {
        CocoNutBall* currentCocoNut = &mCocoNutBallArray[i];
        if (!MR::isDead(currentCocoNut) && MR::isNear(this, currentCocoNut, distToPlayer + ::cValidThrowCocoNutDistance)) {
            return false;
        }
    }

    return true;
}

bool OtaKing::isValidThrowFireBall() const {
    return ::cFireBallThrowNum <= getDisappearedFireBallNum() && MR::calcDistanceToPlayer(this) < ::cThrowDistance &&
           (!mIsLv2 || getDisappearedCocoNutNum() == 3);
}

bool OtaKing::isOneHP() const {
    return mHits + 1 >= ::cHP;
}

bool OtaKing::isValidBubbleAttack() const {
    if (getDisappearedFireBubbleNum() != ::cBubbleAppearNum) {
        return false;
    }

    if (mIsLv2) {
        return mHits > 0;
    } else {
        return isOneHP();
    }
}

// fabricated inline used in multiple functions
bool OtaKing::isDamageNerve() {
    return isNerve(&NrvOtaKing::OtaKingNrvDamage::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvPowerUp::sInstance) ||
           isNerve(&NrvOtaKing::OtaKingNrvDown::sInstance);
}

void OtaKing::damage() {
    if (isDamageNerve()) {
        return;
    }
    MR::deleteEffectAll(this);

    mHits++;

    if (mHits < ::cHP) {
        MR::startSound(this, "SE_BV_OTAKING_DAMAGE");
        MR::startSound(this, "SE_BM_OTAKING_DAMAGE");
        switch (mHits % ::cHP) {
        case 0:
            MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_3");
            break;
        case 1:
            MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_1");
            break;
        case 2:
            MR::startSystemSE("SE_SY_VS_BOSS_DAMAGE_2");
        }

        if (isOneHP()) {
            setNerve(&NrvOtaKing::OtaKingNrvPowerUp::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvDamage::sInstance);
        }
    } else {
        setNerve(&NrvOtaKing::OtaKingNrvDown::sInstance);
    }
}

void OtaKing::throwCocoNut() {
    CocoNutBall* cocoNut = getDisappearedCocoNut();

    mThrowPos->calc();
    TVec3f trans;
    mThrowPos->copyTrans(&trans);

    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    f32 angle = JMAATan2(vec.x, vec.z);

    angle = mRotation.y - MR::repeat(MR::toDegree(angle), mRotation.y - 180.0f, 360.0f);
    f32 random = MR::getRandom(::cThrowAngleMin, ::cThrowAngleMax);

    cocoNut->appearAndThrow(trans, angle + (_EC % 2 == mHits % 2 ? random : -random));
    _EC++;
}

void OtaKing::throwFireBall() {
    mThrowPos->calc();

    TVec3f trans;
    mThrowPos->copyTrans(&trans);

    TVec3f vec;
    vec.sub(*MR::getPlayerPos(), mPosition);

    f32 angle = JMAATan2(vec.x, vec.z);
    angle = mRotation.y - MR::repeat(MR::toDegree(angle), mRotation.y - 180.0f, 360.0f);

    for (int i = 0; i < ::cFireBallThrowNum; i++) {
        FireBall* currentFireBall = getDisappearedFireBall();

        f32 throwAngle = angle;
        if (i != 0) {
            f32 angleOffset;
            if (i % 2 == 0) {
                angleOffset = ::cFireBallThrowAngle * ((i + 1) / 2);
            } else {
                angleOffset = -::cFireBallThrowAngle * ((i + 1) / 2);
            }
            throwAngle += angleOffset;
        }

        currentFireBall->appearAndThrow(trans, ::cFireBallThrowSpeed, throwAngle);
    }
}

void OtaKing::startBckWaitIfNotPlaying() {
    MR::tryStartBck(this, "Wait", nullptr);

    for (int i = 0; i < ARRAY_SIZE(mFeet); i++) {
        MR::tryStartBck(mFeet[i], i == 0 ? "WaitR" : "WaitL", nullptr);
    }
}

void OtaKing::startBckWithFrontFoot(const char* bckName) {
    MR::startBck(this, bckName, nullptr);
    snprintf(mBckLBuffer, sizeof(mBckLBuffer), "%sR", bckName);
    MR::startBck(mFeet[0], mBckLBuffer, nullptr);
    snprintf(mBckRBuffer, sizeof(mBckRBuffer), "%sL", bckName);
    MR::startBck(mFeet[1], mBckRBuffer, nullptr);
}

bool OtaKing::isNerveValidNearAttack() const {
    return isNerve(&NrvOtaKing::OtaKingNrvWait::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance) ||
           isNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance) ||
           isNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance) || isNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
}

void OtaKing::appearBubble() {
    mBubbleAppearPos->calc();

    TVec3f trans;

    mBubbleAppearPos->copyTrans(&trans);

    for (int i = 0; i < ::cBubbleAppearNum; i++) {
        FireBubble* currentBubble = getDisappearedBubble();

        if (currentBubble != nullptr) {
            TVec3f vec1;
            vec1.sub(trans, mPosition);

            if (MR::normalizeOrZero(&vec1)) {
                vec1.set(0.0f, 0.0f, 1.0f);
            }

            TPos3f rotate;

            rotate.makeRotate(TVec3f(0.0f, 1.0f, 0.0f),
                              MR::toRadian(((i + 0.5f) * 60.0f) + MR::getRandom(-::cBubbleAppearRandomAngle, ::cBubbleAppearRandomAngle)));

            rotate.mult33Inline(vec1, vec1);
            currentBubble->appear(trans, vec1, ::cBubbleAppearVelocity);
        }
    }
}

void OtaKing::startDemo() {
    for (int i = 0; i < ARRAY_SIZE(mFeet); i++) {
        MR::requestMovementOn(mFeet[i]);
    }

    if (!mIsLv2) {
        for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
            MR::requestMovementOn(mLongFeet[i]);
        }
    }

    MR::pauseOffEffectAll(this);
    MR::pauseOffEffectAll(mMagma);
}

void OtaKing::appearStarPiece() {
    TVec3f appearPos(mPosition);
    appearPos.y += ::cStarPieceAppearOffsetY;
    MR::appearStarPiece(this, appearPos, mHits * ::cStarPieceAppearNum, ::cStarPieceAppearRange, ::cStarPieceAppearVelocity, false);
    MR::startSoundObject(mSoundObj, "SE_OJ_STAR_PIECE_BURST");
}

void OtaKing::initLongFoot(const JMapInfoIter& rIter) {
    if (!mIsLv2) {
        for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
            mLongFeet[i] = new OtaKingLongFoot(this, ::cLongFootDemoBckStep[i], "飾り足");
            mLongFeet[i]->init(rIter);
            mLongFeet[i]->mScale.set(::cLongFootScale);
        }

        mLongFootMtx.identity33();
        mLongFootMtx.setTrans(mPosition);
        mLongFeet[0]->initFixedPosition(mLongFootMtx, TVec3f(735.0f, 80.0f, -55.0f), TVec3f(-9.0f, 266.0f, 0.0f));
        mLongFeet[1]->initFixedPosition(mLongFootMtx, TVec3f(-959.0f, 130.0f, 0.0f), TVec3f(0.0f, 107.0f, 14.0f));
        mLongFeet[2]->initFixedPosition(mLongFootMtx, TVec3f(0.0f, 43.0f, 884.0f), TVec3f(-8.0f, 159.0f, 0.0f));
        mLongFeet[3]->initFixedPosition(mLongFootMtx, TVec3f(55.0f, 55.0f, -896.0f), TVec3f(-9.0f, -14.0f, 0.0f));
    }
}

bool OtaKing::tryThrowCocoNutOrFireBallIfWait(s32 a1) {
    if (MR::isGreaterStep(this, a1) && isValidThrowCocoNut()) {
        setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance);
        return true;
    }

    if (MR::isGreaterStep(this, a1 * 2) && isValidThrowFireBall()) {
        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
        return false;
    }

    return false;
}

void OtaKing::exeWaitOnSwitch() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);

        startBckWithFrontFoot("Appear");
        MR::setBckFrameAndStop(this, 0.0f);

        mScale.set(0.001f);

        MR::setBckFrameAndStop(mFeet[0], 0.0f);
        MR::hideModel(mFeet[0]);
        MR::setBckFrameAndStop(mFeet[1], 0.0f);
        MR::hideModel(mFeet[1]);

        if (!mIsLv2) {
            for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
                mLongFeet[i]->hide();
            }
        }
    }
}

void OtaKing::exeAppearDemo() {
    if (MR::isFirstStep(this)) {
        startDemo();
        startBckWithFrontFoot("Appear");
        mScale.set(1.0f);

        MR::showModel(mFeet[0]);
        MR::showModel(mFeet[1]);

        if (!mIsLv2) {
            for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
                mLongFeet[i]->startAppearDemo();
            }
        }

        MR::startAnimCameraTargetSelf(this, mAnimCamera, "Appear", 0, 1.0f);
        MR::stopStageBGM(0);
        MR::startBossBGM(0);
        mRotation.set(::cAppearDemoRotate);
        MR::overlayWithPreviousScreen(2);

        if (mIsLv2) {
            MR::forceToFrameCinemaFrame();
        }
    }

    if (MR::isStep(this, 1)) {
        MR::hidePlayer();
    }

    if (MR::isStep(this, ::cAppearSeStep0)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_HITBACK");
    }

    if (MR::isStep(this, ::cAppearSeStep1)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_DAMAGE");
    }

    if (MR::isStep(this, ::cAppearSeStep2)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_HITBACK");
    }

    if (MR::isStep(this, ::cAppearSeStep3)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_HITBACK");
    }

    if (MR::isStep(this, ::cAppearDemoRippleStepS)) {
        mMagma->attack();
    }

    if (MR::isStep(this, ::cAppearDemoRippleStepL)) {
        mMagma->appearDemo();
    }

    MR::tryRumblePadVeryWeak(this, 0);

    if (MR::isStep(this, ::cAppearDemoRumblePad01)) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, ::cAppearDemoRumblePad02)) {
        MR::tryRumblePadVeryStrong(this, 0);
        MR::shakeCameraWeak();
    }
    if (MR::isStep(this, ::cAppearDemoRumblePad03)) {
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }
    if (MR::isStep(this, ::cAppearDemoRumblePad04)) {
        MR::shakeCameraStrong();
    }
    if (MR::isGreaterStep(this, ::cAppearDemoRumblePad04) && MR::isLessStep(this, ::cAppearDemoRumblePad05)) {
        MR::tryRumblePadWeak(this, 0);
    }
    if (MR::isStep(this, ::cAppearDemoRumblePad06)) {
        MR::tryRumblePadMiddle(this, 0);
    }

    if (MR::isBckStopped(this)) {
        MR::validateHitSensors(this);
        validateClippingAndEndDemo("出現");
        MR::showPlayer();
        MR::endAnimCamera(this, mAnimCamera, "Appear", 0, true);

        if (!mIsLv2) {
            for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
                mLongFeet[i]->endDemo();
            }
        }

        TVec3f vec;
        vec.sub(*MR::getPlayerPos(), mPosition);

        f32 angle = JMAATan2(vec.x, vec.z);
        mRotation.y = MR::repeatDegree(MR::toDegree(angle));

        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
    }
}

void OtaKing::exeWait() {
    if (MR::isFirstStep(this)) {
        startBckWaitIfNotPlaying();
    }

    if (tryThrowCocoNutOrFireBallIfWait(::cWaitFrame)) {
        _EC = 0;
    }
}

void OtaKing::exeThrowCocoNutWait() {
    if (MR::isFirstStep(this)) {
        startBckWaitIfNotPlaying();
    }

    tryThrowCocoNutOrFireBallIfWait(::cThrowCocoNutWaitFrame);
}

void OtaKing::exeThrowCocoNut() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("Attack");
        MR::startSound(this, "SE_BV_OTAKING_PRE_RALLYBALL");
    }

    if (MR::isStep(this, ::cThrowCocoNutStep)) {
        throwCocoNut();
        MR::emitEffect(this, "OtaKingCocoAttack");
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, ::cCocoNutRippleStep)) {
        mMagma->attack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
            return;
        }

        if (mHits > 0) {
            if (isValidThrowFireBall()) {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
            } else {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
            }
        } else if (_EC >= 2) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance);
        }
    }
}

void OtaKing::exeThrowFireBallWait() {
    if (MR::isFirstStep(this)) {
        startBckWaitIfNotPlaying();
    }
    if (MR::isGreaterStep(this, ::cThrowFireBallWaitFrame) && isValidThrowFireBall()) {
        setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
    }
}

void OtaKing::exeThrowFireBall() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("FireAttack");
        MR::startSound(this, "SE_BV_OTAKING_PRE_FIREBALL");
    }
    MR::startLevelSound(this, "SE_BM_LV_OTAKING_PRE_FIRE");

    if (MR::isStep(this, ::cThrowFireBallStep)) {
        throwFireBall();
        MR::emitEffect(this, "OtaKingFireAttack");
        MR::startSound(this, "SE_BM_OTAKING_FIRE_OUT");
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, ::cFireBallRippleStep)) {
        mMagma->fireAttack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
        } else if (mHits > 0 && isValidThrowCocoNut()) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNut::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvWait::sInstance);
        }
    }
}

void OtaKing::exeDamage() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("Damage");
        MR::tryRumblePadVeryStrong(this, 0);
        appearStarPiece();
        mMagma->damage();
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(mIsLv2 ? 6 : 16);
    }

    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormalStrong();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
        } else if (isValidThrowFireBall()) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
        }
    }
}

void OtaKing::exePowerUp() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("CrownDamage");
        MR::startBrk(this, "CrownDamage");
        MR::startBpk(this, "CrownDamage");
        MR::tryRumblePadVeryStrong(this, 0);
        appearStarPiece();
        mMagma->damage();
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(mIsLv2 ? 6 : 16);
    }

    if (MR::isStep(this, 3)) {
        MR::shakeCameraNormalStrong();
    }

    if (MR::isStep(this, ::cDamageCrownFallFrame)) {
        MR::startSoundObject(mSoundObj, "SE_BM_OTAKING_CROWN_FALL");
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
        mMagma->damage();
    }

    if (MR::isGreaterStep(this, ::cDamageCrownFallFrame)) {
        MR::tryRumblePadVeryWeak(this, 0);
    }

    if (MR::isStep(this, 83)) {
        MR::hideMaterial(this, "lambert141_v");
        MR::hideMaterial(this, "Daiya_v");
    }

    if (MR::isStep(this, ::cBlurStartStep)) {
        mMagma->damage();
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 195)) {
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 215)) {
        MR::shakeCameraNormal();
    }

    if (MR::isGreaterStep(this, ::cBlurStartStep) && MR::isLessStep(this, 215)) {
        MR::tryRumblePadWeak(this, 0);
    }

    if (MR::isStep(this, ::cBlurStartStep)) {
        MR::startCenterScreenBlur(::cBlurTotalFrame, 15.0f, 80, 5, 30);
    }

    if (MR::isBckStopped(this)) {
        MR::startBrk(this, "Wait");
        MR::startBpk(this, "Wait");

        MR::showModel(mFeet[0]);
        MR::showModel(mFeet[1]);

        if (MR::isValidSwitchB(this)) {
            MR::onSwitchB(this);
        }

        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
            return;
        } else if (isValidThrowFireBall()) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
            return;
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
            return;
        }
    }

    if (MR::getPlayerPos()->y < mPosition.y + ::cDamageFrontFootClipingHeight) {
        MR::hideModel(mFeet[0]);
        MR::hideModel(mFeet[1]);
    } else {
        // unneeded, as showmodel is already called unconditionally
        MR::showModel(mFeet[0]);
        MR::showModel(mFeet[1]);
    }
}

void OtaKing::exeDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Down", 0);
        MR::startSound(this, "SE_BM_OTAKING_LAST_DAMAGE");
        MR::startSound(this, "SE_BM_OTAKING_DIE");
        MR::startSound(this, "SE_BV_OTAKING_DIE");
        MR::startSystemSE("SE_SY_VS_BOSS_LAST_HIT");
        MR::tryRumblePadVeryStrong(this, 0);
        MR::invalidateHitSensors(this);
        MR::stopStageBGM(60);

        for (int i = 0; i < 3; i++) {
            if (!MR::isDead(&mCocoNutBallArray[i])) {
                mCocoNutBallArray[i].kill();
            }
        }

        for (int i = 0; i < 6; i++) {
            if (!MR::isDead(&mFireBallArray[i])) {
                mFireBallArray[i].kill();
            }
        }

        for (int i = 0; i < 6; i++) {
            if (!MR::isDead(&mFireBubbleArray[i])) {
                mFireBubbleArray[i].kill();
            }
        }

        if (!mIsLv2) {
            for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
                mLongFeet[i]->startDownDemo();
            }
        }
    }

    if (MR::isStep(this, 2)) {
        MR::stopScene(20);
    }

    if (MR::isStep(this, 3)) {
        invalidateClippingAndStartDemo("ダウン", &NrvOtaKing::OtaKingNrvDownDemo::sInstance, &NrvOtaKing::OtaKingNrvWaitStartDemo::sInstance);
    }
}

void OtaKing::exeDownDemo() {
    TPos3f rotate;
    rotate.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(::cDownDemoRotate.y));
    rotate.setTransInline(mPosition);

    if (MR::isFirstStep(this)) {
        startDemo();
        startBckWithFrontFoot("Down");
        MR::startAnimCameraTargetSelf(this, mAnimCamera, "Down", 0, 1.0f);
        mRotation.set(::cDownDemoRotate);
        mMagma->damage();
        TVec3f demoBreakVec;
        rotate.mult(::cDownDemoCocoNutPos, demoBreakVec);
        mDownCocoNut->mScale.set(::cDownDemoCocoNutScale);
        mDownCocoNut->demoBreak(demoBreakVec);
        TVec3f starAppearPos(mPosition);
        starAppearPos.y += ::cPowerStarAppearOffsetY;
        MR::requestAppearPowerStar(this, starAppearPos);
    }

    if (MR::isStep(this, 1)) {
        MR::hidePlayer();
    }

    if (MR::isStep(this, ::cDownRippleStep)) {
        mMagma->down();
    }

    if (MR::isGreaterStep(this, ::cDownSinkStartStep)) {
        MR::startLevelSound(this, "SE_BM_LV_OTAKING_SINK");
    }

    mRotation.set(::cDownDemoRotate);
    MR::tryRumblePadWeak(this, 0);

    if (MR::isStep(this, 0)) {
        MR::shakeCameraStrong();
    }

    if (MR::isStep(this, ::cDownDemoRumbleFrame1)) {
        MR::shakeCameraNormal();
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, ::cDownDemoRumbleFrame2)) {
        MR::shakeCameraNormal();
        MR::tryRumblePadStrong(this, 0);
    }

    if (MR::isStep(this, ::cPowerStarExistBgmStartStep)) {
        MR::startAfterBossBGM();
    }

    if (MR::isStep(this, ::cDownDemoFrame)) {
        validateClippingAndEndDemo("ダウン");
        setNerve(&NrvOtaKing::OtaKingNrvAppearStar::sInstance);
    }
}

void OtaKing::exeAppearStar() {
    if (MR::isFirstStep(this)) {
        mScale.set(0.001f);
        for (int i = 0; i < ARRAY_SIZE(mFeet); i++) {
            mFeet[i]->kill();
        }

        if (!mIsLv2) {
            for (int i = 0; i < ARRAY_SIZE(mLongFeet); i++) {
                mLongFeet[i]->kill();
            }
        }

        getSensor("body")->invalidate();
        getSensor("search")->invalidate();

        MR::validateClipping(this);
        MR::pauseOffEffectAll(mMagma);
    } else if (MR::isEndPowerStarAppearDemo(this)) {
        MR::showPlayer();
        setNerve(&NrvOtaKing::OtaKingNrvDead::sInstance);
    }
}

void OtaKing::exeDead() {
    if (MR::isFirstStep(this) && MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void OtaKing::exeHitBackStart() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("HitBackStart");
        MR::deleteEffectAll(this);
    }
}

void OtaKing::exeHitBack() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("HitBack");
        MR::deleteEffectAll(this);
        MR::tryRumblePadStrong(this, 0);
        mMagma->hitBack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
            return;
        }

        if (mHits > 0) {
            if (isValidThrowFireBall()) {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBall::sInstance);
            } else {
                setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
            }
        } else if (_EC >= 2) {
            setNerve(&NrvOtaKing::OtaKingNrvThrowFireBallWait::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvThrowCocoNutWait::sInstance);
        }
    }
}

void OtaKing::exeNearAttack() {
    if (MR::isFirstStep(this)) {
        startBckWithFrontFoot("HitBack");
        MR::deleteEffectAll(this);
        mMagma->hitBack();
    }

    if (MR::isStep(this, ::cNearAttackRippleStep)) {
        mMagma->attack();
    }

    if (MR::isBckStopped(this)) {
        if (isValidBubbleAttack()) {
            setNerve(&NrvOtaKing::OtaKingNrvBubbleAttack::sInstance);
        } else {
            setNerve(&NrvOtaKing::OtaKingNrvWait::sInstance);
        }
    }
}

void OtaKing::exeBubbleAttack() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BV_OTAKING_PRE_FIREBALL");
        startBckWithFrontFoot("BubbleAttack");
    }

    if (MR::isStep(this, ::cBubbleAppearStep)) {
        appearBubble();
        MR::tryRumblePadStrong(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, ::cBubbleAttackRippleStep)) {
        MR::startSound(this, "SE_BM_OTAKING_MAGMA_BEAT");
        mMagma->attack();
    }

    MR::setNerveAtBckStopped(this, &NrvOtaKing::OtaKingNrvWait::sInstance);
}

void OtaKing::exeWaitStartDemo() {
}
