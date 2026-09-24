#include "Game/Enemy/Snakehead.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

void Snakehead_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

namespace {
    struct SnakeheadData {
        const s32* _0;
        const s32* _4;
        const f32* _8;
        const char* _C;
        const char* _10;
        const char* _14;
        const char* _18;
        const char* _1C;
    };

    static const f32 sDefaultForwardSpeed = 15.0f;
    static const f32 sDefaultBackSpeed = 10.0f;
    static const f32 sGoHomeSpeedNormal = 20.0f;
    static const f32 sGoHomeSpeedRace = 50.0f;
    static const s32 sStepForWaitBig = 30;
    static const s32 sStepForRestBig = 100;
    static const s32 sStepForWaitSmall = 60;
    static const s32 sStepForRestSmall = 120;
    static const s32 sStepForWaitBigRace = 30;
    static const s32 sStepForRestBigRace = 100;
    static const s32 sStepForWaitSmallRace = 60;
    static const s32 sStepForRestSmallRace = 120;
    static const f32 sBodySensorRadiusBig = 0.0f;
    static const f32 sBodySensorRadiusSmall = 0.0f;
    static const f32 sDefaultMoveStartRadius = 0.0f;
    static const f32 sStartCoord = 300.0f;
    // static const f32 sStepForShockStay = 0.0f;
    // static const f32 sCoordToGoalForSlowMove = 0.0f;

    static const SnakeheadData sSnakeheadDataTable[] = {
        {&::sStepForWaitBig, &::sStepForRestBig, &::sGoHomeSpeedNormal, "StraightAppear", "StraightWait", "StraightForward", "StraightForwardSmoke",
         "StraightBack"},
        {&::sStepForWaitSmall, &::sStepForRestSmall, &::sGoHomeSpeedNormal, "StraightAppear", "Wait", "StraightForward", nullptr, "StraightBack"},
        {&::sStepForWaitBigRace, &::sStepForRestBigRace, &::sGoHomeSpeedRace, nullptr, "Wait", "Forward", "ForwardSmoke", "Back"},
        {&::sStepForWaitSmallRace, &::sStepForRestSmallRace, &::sGoHomeSpeedRace, nullptr, "Wait", "Forward", nullptr, "Back"}};

    const SnakeheadData* getParam(s32 index) {
        return &::sSnakeheadDataTable[index];
    }
};  // namespace

namespace NrvSnakehead {
    NEW_NERVE(SnakeheadNrvWaylay, Snakehead, Waylay);
    NEW_NERVE(SnakeheadNrvWait, Snakehead, Wait);
    NEW_NERVE(SnakeheadNrvMoveForward, Snakehead, MoveForward);
    NEW_NERVE(SnakeheadNrvRest, Snakehead, Rest);
    NEW_NERVE(SnakeheadNrvMoveBack, Snakehead, MoveBack);
    NEW_NERVE(SnakeheadNrvTurtleDown, Snakehead, TurtleDown);
    NEW_NERVE_ONEND(SnakeheadNrvDPDSwoon, Snakehead, DPDSwoon, DPDSwoon);
};  // namespace NrvSnakehead

Snakehead::Snakehead(const char* pName)
    : LiveActor(pName), mController(), mStarPointer(), _C4(0.0f, 0.0f, 0.0f), mForwardSpeed(::sDefaultForwardSpeed),
      mBackwardSpeed(::sDefaultBackSpeed), mActivationRadius(1000.0f), mRailCenter(0.0f, 0.0f, 0.0f), _E8(), mItem() {
    mBodyMtx.identity();
}

void Snakehead::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    const char* objectName;
    MR::getObjectName(&objectName, rIter);
    initModelManagerWithAnm(objectName, nullptr, false);

    MR::getJMapInfoArg0NoInit(rIter, &mForwardSpeed);
    MR::getJMapInfoArg1NoInit(rIter, &mBackwardSpeed);
    MR::getJMapInfoArg2NoInit(rIter, &mActivationRadius);

    s32 arg3 = 0;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);

    _E8 = static_cast< SnakeheadType >(MR::isEqualString(objectName, "SnakeheadSmall") + arg3 * 2);

    MR::connectToSceneEnemy(this);

    if (isSmall()) {
        initHitSensor(2);
        MR::addHitSensorAtJointEnemy(this, "head", "Head", 8, 100.0f, TVec3f(140.0f, -70.0f, 0.0f));
        MR::addHitSensorAtJointEnemy(this, "body", "Body03", 8, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));
    } else {
        initHitSensor(3);
        MR::addHitSensorAtJointEnemy(this, "upper_jaw", "UpperJaw", 8, 250.0f, TVec3f(100.0f, 0.0f, 0.0f));
        MR::addHitSensorAtJointEnemy(this, "lower_jaw", "LowerJaw", 8, 200.0f, TVec3f(250.0f, 0.0f, 0.0f));
        MR::addHitSensorAtJointEnemy(this, "body", "Body03", 8, 600.0f, TVec3f(0.0f, 0.0f, 0.0f));
    }

    initEffectKeeper(1, nullptr, false);

    const char* smokeName = ::getParam(_E8)->_18;
    if (smokeName != nullptr) {
        _C4 = mPosition;
        MR::setEffectHostSRT(this, smokeName, nullptr, nullptr, nullptr);
    }

    initSound(4, false);

    if (isSmall()) {
        MR::initStarPointerTarget(this, 250.0f, TVec3f(0.0f, 0.0f, 150.0f));
    } else {
        MR::initStarPointerTarget(this, 500.0f, TVec3f(0.0f, 0.0f, 200.0f));
    }

    mController = new AnimScaleController(nullptr);
    mController->setParamTight();

    mStarPointer = new WalkerStateBindStarPointer(this, mController);

    mBodyMtx.set(MR::getJointMtx(this, "Body04"));

    if (MR::isEqualString(objectName, "Snakehead")) {
        MR::initShadowController(this, 2);
        MR::addShadowVolumeFlatModel(this, "Head", "SnakeheadShadow", mBodyMtx);
        MR::addShadowVolumeBox(this, "Body", TVec3f(480.0f, 300.0f, 680.0f));
    } else {
        MR::initShadowVolumeFlatModel(this, "SnakeheadSmallShadow", mBodyMtx);
        MR::setShadowDropLength(this, "SnakeheadSmallShadow", 500.0f);
    }

    MR::initLightCtrl(this);

    initRailRider(rIter);
    MR::initAndSetRailClipping(&mRailCenter, this, 100.0f, 500.0f);
    MR::setRailCoord(this, ::sStartCoord);
    MR::moveTransToCurrentRailPos(this);

    MR::addToAttributeGroupSearchTurtle(this);

    if (isSmall()) {
        MR::declareStarPiece(this, 9);
    } else {
        mItem = MR::createKinokoOneUp();
    }

    if (isMove()) {
        initNerve(GET_NERVE(Snakehead, SnakeheadNrvWait));
    } else {
        initNerve(GET_NERVE(Snakehead, SnakeheadNrvWaylay));

        MR::hideModel(this);

        MR::invalidateShadow(this, nullptr);
    }

    makeActorAppeared();
}

void Snakehead::initAfterPlacement() {
    TVec3f headPos;
    MR::copyJointPos(this, "Head", &headPos);

    headPos -= MR::getRailPointPosStart(this);
    headPos += (MR::getRailPointPosEnd(this));

    TVec3f v7;
    v7.scaleAdd(-50.0f, mGravity, headPos);
    MR::getFirstPolyOnLineToMapExceptSensor(&_C4, nullptr, v7, mGravity * 1000.0f, getSensor("body"));

    mBodyMtx.set(MR::getJointMtx(this, "Body04"));
}

void Snakehead::kill() {
    MR::startSound(this, "SE_EM_EXPLODE_UNDER_WATER");

    MR::emitEffect(this, "Death");

    if (isSmall()) {
        TVec3f headPos;
        MR::copyJointPos(this, "Head", &headPos);
        MR::appearStarPiece(this, headPos, 9, 10.0f, 40.0f, 0);

        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST_W");
    } else {
        TVec3f attPos;
        MR::copyJointPos(this, "Antenna", &attPos);

        TPos3f itemMtx;
        itemMtx.set(getBaseMtx());
        itemMtx.setTrans(attPos);
        MR::appearKinokoOneUpPop(reinterpret_cast< BenefitItemObj* >(mItem), itemMtx, 15.0f);
    }

    LiveActor::kill();
}

void Snakehead::control() {
    mController->updateNerve();

    tryDPDSwoon();

    TVec3f jointPos;
    MR::copyJointPos(this, "Body04", &jointPos);
    mBodyMtx.setTrans(jointPos);

    if (!isSmall()) {
        TVec3f body01Pos;
        MR::copyJointPos(this, "Body01", &body01Pos);

        TVec3f v6;
        f32 dist = jointPos.distance(body01Pos);

        v6.add(jointPos, body01Pos);
        v6.scale(0.5f);
        MR::setShadowDropPosition(this, "Body", v6);
        MR::setShadowVolumeBoxSize(this, "Body", TVec3f(480.0f, 300.0f, dist));
    }
}

void Snakehead::calcAndSetBaseMtx() {
    TVec3f endPntDir;
    MR::calcRailEndPointDirection(&endPntDir, this);

    TPos3f frontUp;
    MR::makeMtxFrontUpPos(&frontUp, endPntDir, -mGravity, mPosition);
    MR::setBaseTRMtx(this, frontUp);

    TVec3f scale = mController->_C * mScale;
    MR::setBaseScale(this, scale);
}

void Snakehead::exeWaylay() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Waylay");
        MR::setBckFrameAndStop(this, 0.0f);

        mBodyMtx.set(MR::getJointMtx(this, "Body04"));
    }

    if (isNearPlayerFromRail() && MR::getBckFrame(this) == 0.0f) {
        choiceAndStartBck("Waylay");
        MR::setBckFrame(this, 1.0f);

        MR::showModel(this);
    }

    if (MR::getBckFrame(this) == 2.0f) {
        MR::validateShadow(this, nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(Snakehead, SnakeheadNrvMoveForward));
    }
}

void Snakehead::exeWait() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Wait");
    }

    if (MR::isGreaterEqualStep(this, *::getParam(_E8)->_0) && (isMove() || isNearPlayerFromRail())) {
        setNerve(GET_NERVE(Snakehead, SnakeheadNrvMoveForward));
    }
}

void Snakehead::exeMoveForward() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Forward");

        MR::setRailDirectionToEnd(this);

        MR::startSound(this, "SE_EM_SNAKEHEAD_FORWARD");
    }

    if (!isMove() && MR::isRailReachedNearGoal(this, ::sStartCoord)) {
        MR::moveCoordAndFollowTrans(this, mForwardSpeed / 1.5f);
    } else {
        MR::moveCoordAndFollowTrans(this, mForwardSpeed);
    }

    if (MR::isRailReachedGoal(this)) {
        setNerve(GET_NERVE(Snakehead, SnakeheadNrvRest));
    }
}

void Snakehead::exeRest() {
    if (MR::isStep(this, *::getParam(_E8)->_4)) {
        setNerve(GET_NERVE(Snakehead, SnakeheadNrvMoveBack));
    }
}

void Snakehead::exeMoveBack() {
    if (MR::isFirstStep(this)) {
        choiceAndStartBck("Back");

        MR::setRailDirectionToStart(this);

        MR::startSound(this, "SE_EM_SNAKEHEAD_BACKWARD");
    }

    MR::moveCoordAndFollowTrans(this, mBackwardSpeed);

    if (MR::isRailReachedNearGoal(this, ::sStartCoord)) {
        MR::setRailCoord(this, ::sStartCoord);
        MR::moveTransToCurrentRailPos(this);

        setNerve(GET_NERVE(Snakehead, SnakeheadNrvWait));
    }
}

void Snakehead::exeTurtleDown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Damage");

        MR::setRailDirectionToStart(this);

        MR::startSound(this, "SE_EV_SNAKEHEAD_DAMAGE");
    }

    f32 coord = *::getParam(_E8)->_8;
    if (MR::isRailReachedNearGoal(this, ::sStartCoord + coord)) {
        MR::setRailCoord(this, ::sStartCoord);
        MR::moveTransToCurrentRailPos(this);
    } else {
        MR::moveCoordAndFollowTrans(this, coord);
    }

    if (MR::isBckStopped(this)) {
        kill();
    }
}

void Snakehead::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvTurtleDown))) {
        return;
    }

    if (!MR::isSensorEnemy(pSender)) {
        return;
    }

    if (!MR::isSensorPlayerOrRide(pReceiver) && !MR::isSensorEnemy(pReceiver)) {
        return;
    }

    if (pSender != getSensor("body")) {
        if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvDPDSwoon)) || !MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }

        return;
    }

    if (pSender != getSensor("body")) {
        return;
    }

    TVec3f vec40;
    MR::calcPerpendicFootToLineInside(&vec40, pReceiver->mPosition, MR::getRailPointPosStart(this), MR::getRailPointPosEnd(this));

    TVec3f vec4C;
    vec4C.sub(pReceiver->mPosition, vec40);

    f32 pushRadius = isSmall() ? 100.0f : ::sStartCoord;
    if (vec4C.length() > pushRadius) {
        return;
    }

    if (!isNerve(GET_NERVE(Snakehead, SnakeheadNrvDPDSwoon)) && MR::sendMsgEnemyAttackStrong(pReceiver, pSender)) {
        return;
    }

    TVec3f pos(getSensor("body")->mPosition);
    f32 radius = getSensor("body")->mRadius;

    getSensor("body")->mPosition.set(vec40);
    getSensor("body")->mRadius = pushRadius;

    MR::sendMsgPush(pReceiver, pSender);

    getSensor("body")->mPosition.set(pos);
    getSensor("body")->mRadius = radius;
}

bool Snakehead::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvWaylay))) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvTurtleDown))) {
        return false;
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        setNerve(GET_NERVE(Snakehead, SnakeheadNrvTurtleDown));
        return true;
    }

    return false;
}

void Snakehead::choiceAndStartBck(const char* pBck) {
    const char* v1 = nullptr;
    if (MR::isEqualString(pBck, "Forward")) {
        v1 = ::getParam(_E8)->_14;
    } else if (MR::isEqualString(pBck, "Back")) {
        v1 = ::getParam(_E8)->_1C;
    } else if (MR::isEqualString(pBck, "Waylay")) {
        v1 = ::getParam(_E8)->_C;
    } else if (MR::isEqualString(pBck, "Wait")) {
        v1 = ::getParam(_E8)->_10;
    }

    if (v1 != nullptr) {
        if (!MR::isEqualString(pBck, "Waylay")) {
            MR::startBck(this, v1);
        } else {
            MR::startBckNoInterpole(this, v1);
        }

        if (MR::isExistBtk(this, v1)) {
            MR::startBtk(this, v1);
        } else if (MR::isExistBtk(this, "Wait")) {
            MR::startBtk(this, "Wait");
            MR::setBtkFrameAndStop(this, 0.0f);
        }
    }
}

bool Snakehead::tryDPDSwoon() {
    if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvDPDSwoon))) {
        return false;
    }

    if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvWaylay))) {
        return false;
    }

    if (isNerve(GET_NERVE(Snakehead, SnakeheadNrvTurtleDown))) {
        return false;
    }

    if (!mStarPointer->tryStartPointBind()) {
        return false;
    }

    setNerve(GET_NERVE(Snakehead, SnakeheadNrvDPDSwoon));

    return true;
}

bool Snakehead::isNearPlayerFromRail() const {
    TVec3f nearestPos;
    MR::calcNearestRailPos(&nearestPos, this, *MR::getPlayerPos());

    return nearestPos.distance(*MR::getPlayerPos()) <= mActivationRadius;
}

void Snakehead::endDPDSwoon() {
    mStarPointer->kill();
}

void Snakehead::exeDPDSwoon() {
    MR::updateActorStateAndNextNerve(this, mStarPointer, GET_NERVE(Snakehead, SnakeheadNrvMoveBack));
}
