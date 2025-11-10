#include "Game/NPC/TrickRabbit.hpp"
#include "Game/Camera/CameraTargetArg.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/RailGraphIter.hpp"
#include "Game/NPC/RabbitStateCaught.hpp"
#include "Game/NPC/RabbitStateWaitStart.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "Game/Util/RailGraphUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "Game/Map/HitInfo.hpp"

TrickRabbit::TrickRabbit(const char* pName) : LiveActor(pName),
_8C(0.0f, 0.0f, 0.0f, 1.0f),
_9C(0, 0, 1),
mRabbitStateWaitStart(0),
mRabbitStateCaught(0),
mMultiEventCamera(0),
mTalkMessageCtrl(nullptr),
mFootPrint(nullptr),
mRailGraph(0),
mRailGraphIter(0),
mSpotMarkLight(nullptr),
_CC(0),
_D0(0.0f),
_D4(0),
_D8(0),
_DC(0),
_E0(0),
_E1(0),
mParabolicPath(new ParabolicPath) {}

void TrickRabbit::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    if (MR::isEqualString(mName, "いたずら幽霊ウサギ"))
        _E1 = true;
    else
        _E1 = false;

    initModel();
    initSound(4, 0);
    initSensor();
    initBind();
    initEffectKeeper(0, 0, 0);
    MR::onEmitEffectSyncClipping(this, "Light");
    MR::onForceDeleteEffectSyncClipping(this, "Light");
    f32 f = mScale.x;
    MR::initShadowVolumeSphere(this, f*45.0f);
    mFootPrint = TrickRabbitUtil::createRabbitFootPrint(this);
    initRoute(rIter);
    f32 zero = 0.0f;
    TVec3f stack_8(zero, 120.0f, zero);
    MR::createTalkCtrlDirect(this, rIter, "TrickRabbit", stack_8, 0);
    initDemoCamera(rIter);
    initState();
    initTimer();
    mSpotMarkLight = new SpotMarkLight(this, 100.0f, 1500.0f, 0.0f, 0);
    mSpotMarkLight->initWithoutIter();
    MR::declarePowerStar(this);
    MR::addToAttributeGroupSearchTurtle(this);
    MR::useStageSwitchWriteA(this, rIter);
    if (!mTalkMessageCtrl) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvWait::sInstance);
    }
    makeActorAppeared();
}

void TrickRabbit::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void TrickRabbit::initModel() {
    mScale.scale(1.0f);
    initModelManagerWithAnm("TrickRabbit", 0, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);
}

void TrickRabbit::initSensor() {
    initHitSensor(2);
    MR::addHitSensorAtJointEnemy(this, "Body", "Spine", 8, 70.0, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorAtJointEnemy(this, "Catch", "Spine", 8, 30.0, TVec3f(0.0f, 0.0f, 0.0f));
    MR::initStarPointerTargetAtJoint(this, "Spine", 70.0f, TVec3f(0, 0, 0));
}

void TrickRabbit::initBind() {
    f32 f = mScale.x;
    initBinder(f*60.0f, f*60.0f, 0);
    MR::onCalcGravity(this);
}

void TrickRabbit::initRoute(const JMapInfoIter& rIter) {
    mRailGraph = MR::createRailGraphFromJMap(rIter);
    mRailGraphIter = MR::createRailGraphIter(mRailGraph);
}

void TrickRabbit::initDemoCamera(const JMapInfoIter& rIter) {
    ActorCameraInfo stack_8 = ActorCameraInfo(rIter);
    
    if (stack_8.mCameraSetID == -1)
        return;

    mMultiEventCamera = new MultiEventCamera();
    mMultiEventCamera->setUp(mName, new ActorCameraInfo(rIter), 1);
}

void TrickRabbit::initState() {
    initNerve(&NrvTrickRabbit::TrickRabbitNrvWaitStart::sInstance);
    mRabbitStateCaught = new RabbitStateCaught(this, mTalkMessageCtrl);
    mRabbitStateCaught->init();

    mRabbitStateWaitStart = new RabbitStateWaitStart(this, &_9C, mTalkMessageCtrl);
    mRabbitStateWaitStart->init();
    mRabbitStateWaitStart->setTalkActionName("StartEvent");
    mRabbitStateWaitStart->_1C = true;
}

void TrickRabbit::control() {
    TVec3f stack_8(-mGravity);
    MR::blendQuatUpFront(&_8C, stack_8, _9C, 0.1, 0.2);
    controlRouteLevel();
    updateTime();
}

void TrickRabbit::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

void TrickRabbit::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isCaughtable()) {
            if (pSender == getSensor("Catch")) 
                requestCaught();
        }
        else
            MR::sendMsgPush(pReceiver, pSender);
    }
}

bool TrickRabbit::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender) && !isCaughtable())
        return true;

    return false;
}

bool TrickRabbit::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("Body"))
        return 0;

    if (isNerve(&NrvTrickRabbit::TrickRabbitNrvWaitStart::sInstance) || isNerve(&NrvTrickRabbit::TrickRabbitNrvGiveUp::sInstance)) {
        return mRabbitStateWaitStart->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }
    
    if (MR::isMsgStarPieceAttack(msg)) {
        MR::limitedStarPieceHitSound();
        MR::startSound(this, "SE_SM_RABBIT_STAR_PIECE_HIT", -1, -1);
        MR::startSound(this, "SE_SV_RABBIT_STAR_PIECE_HIT", -1, -1);
        return receiveMsgTrample();
    }

    if (MR::isMsgPlayerSpinAttack(msg))
        return requestCaught();

    if (MR::isMsgPlayerTrample(msg)) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_RABBIT_STAR_PIECE_HIT", -1, -1);
        return receiveMsgTrample();
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return receiveMsgTrample();
    }
    return false;
}

bool TrickRabbit::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvTrickRabbit::TrickRabbitNrvWaitStart::sInstance) || isNerve(&NrvTrickRabbit::TrickRabbitNrvGiveUp::sInstance)) {
        return mRabbitStateWaitStart->receiveOtherMsg(msg, pSender, pReceiver);
    }
    return false;
}

bool TrickRabbit::receiveMsgTrample() {
    if (!isTumbable())
        return false;

    int l = 0;
    if (isNerve(&NrvTrickRabbit::TrickRabbitNrvJump::sInstance) && MR::isGreaterStep(this, 1))
        l = 1;

    if (l) 
        setNerve(&NrvTrickRabbit::TrickRabbitNrvFallDown::sInstance);
    else
        setNerve(&NrvTrickRabbit::TrickRabbitNrvTumble::sInstance);

    return true;
}

bool TrickRabbit::requestCaught() {
    if (isCaughtable()) {
        if (_E1) {
            MR::showModel(this);
            MR::validateShadow(this, 0);
        }
        
        MR::invalidateClipping(this);
        MR::forwardNode(mTalkMessageCtrl);
        setNerve(&NrvTrickRabbit::TrickRabbitNrvCaught::sInstance);
        return true;
    }

    return false;
}

bool TrickRabbit::tryStartDemoRunnaway() {
    if (MR::tryStartDemoMarioPuppetable(this, "逃走")) {
        MR::startEventBGM(1);
        if (mMultiEventCamera) {
            mMultiEventCamera->start(CameraTargetArg(this), 0);
        }
        MR::moveNodeNearPosition(mRailGraphIter, mPosition, -1.0f, 0);
        if (selectEdgeStartEvent(mRailGraphIter)) {
            setNerve(&NrvTrickRabbit::TrickRabbitNrvDemoRunaway::sInstance);
            if (_E1) {
                MR::hideModelAndOnCalcAnim(this);
                MR::invalidateShadow(this, 0);
            }
        }
        else {
            setNerve(&NrvTrickRabbit::TrickRabbitNrvDemoEnd::sInstance);
        }
        return true;

    }

    return false;
}

void TrickRabbit::endDemoRunnaway() {
    MR::endDemo(this, "逃走");
    if (mMultiEventCamera)
        mMultiEventCamera->endForce();

    startRouteLevelControl(1, 0x4B0);
    setNerve(&NrvTrickRabbit::TrickRabbitNrvWait::sInstance);
    _E9 = true;
}

bool TrickRabbit::tryRunaway() {
    if (MR::isNearPlayer(this, 1200.0f)) {
        ::RunnawayNodeSelector stack_8 = ::RunnawayNodeSelector();
        MR::moveNodeNearPosition(mRailGraphIter, mPosition, -1.0f, &stack_8);

        TVec3f stack_C(mPosition-*MR::getPlayerPos());
        selectEdgeRunnaway(mRailGraphIter, stack_C, -0.30000001f);
        setNerve(&NrvTrickRabbit::TrickRabbitNrvRunaway::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryBrakeTurn() {
    if (MR::isGreaterStep(this, 0x3C) && MR::isNearPlayer(this, 600.0f)) {
        TVec3f stack_2C;
        TVec3f stack_20;
        TVec3f stack_14(*MR::getPlayerPos()-mPosition);
        stack_2C.set(stack_14);
        TVec3f stack_8(*MR::getNextNodePosition(mRailGraphIter)-mPosition);
        stack_20.set(stack_8);
        MR::normalizeOrZero(&stack_2C);
        MR::normalizeOrZero(&stack_20);
        f32 f = stack_20.dot(stack_2C);
        if (f > 0.1f) {
            MR::selectReverseEdge(mRailGraphIter);
            setNerve(&NrvTrickRabbit::TrickRabbitNrvBrakeTurn::sInstance);
            return true;
        }
    }
    return false;
}

bool TrickRabbit::tryBrakeTurnEnd() {
    if (MR::isGreaterStep(this, 0x28)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvRunaway::sInstance);
        return true;
    }
    return false;
}


bool TrickRabbit::tryStop() {
    bool isNear = !MR::isNearPlayer(this, 1500.0f);
    if (isNear) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvWait::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryJumpStart() {
    if (isNextEdgeJump()) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvJumpStart::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryJump() {
    if (MR::isGreaterStep(this, _CC)) {
        MR::zeroVelocity(this);
        setNerve(&NrvTrickRabbit::TrickRabbitNrvJump::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryJumpEnd() {
    if (MR::isGreaterEqualStep(this, _CC)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvRunaway::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryEndFallDown() {
    if (MR::isBindedGround(this)) {
        MR::startAction(this, "FallDownLand");
        MR::zeroVelocity(this);
        setNerve(&NrvTrickRabbit::TrickRabbitNrvFallDownLand::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryEndFallDownLand() {
    if (MR::isGreaterStep(this, 0x14)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvGetUpFromFallDown::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryComebackRouteStart() {
    if (MR::isGreaterStep(this, 0x14)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvComebackRouteStart::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryComebackRoute() {
    if (MR::isGreaterStep(this, _CC)) {
        MR::zeroVelocity(this);
        setNerve(&NrvTrickRabbit::TrickRabbitNrvComebackRoute::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryEndTumble() {
    if (MR::isGreaterStep(this, 0x14) && MR::isBindedGround(this)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvGetUp::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryEndGetUp() {
    if (MR::isGreaterStep(this, 0x14)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvRunaway::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryDemoJumpStart() {
    if (isNextEdgeJump()) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvDemoJumpStart::sInstance);
        return false;
    }
    return true;
}

bool TrickRabbit::tryDemoJump() {
    if (MR::isGreaterStep(this, _CC)) {
        MR::zeroVelocity(this);
        setNerve(&NrvTrickRabbit::TrickRabbitNrvDemoJump::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryDemoJumpEnd() {
    if (MR::isGreaterEqualStep(this, _CC)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvDemoRunaway::sInstance);
        return true;
    }
    return false;
}

bool TrickRabbit::tryEndPowerStarDemo() {
    if (MR::isEndPowerStarAppearDemo(this)) {
        setNerve(&NrvTrickRabbit::TrickRabbitNrvGiveUp::sInstance);
        return true;
    }
    return false;
}

void TrickRabbit::exeWaitStart() {
    if (MR::isFirstStep(this)) {
        mRabbitStateWaitStart->setTalkActionName("StartEvent");
        mRabbitStateWaitStart->_1C = true;
    }
    
    if (MR::updateActorState(this, (ActorStateBaseInterface*)mRabbitStateWaitStart) && !tryStartDemoRunnaway())
        setNerve(&NrvTrickRabbit::TrickRabbitNrvTryStartDemoRunnaway::sInstance);
}

void TrickRabbit::exeDemoRunaway() {
    doRunaway();
    if ((_D0 < 125.0f)) {
        MR::moveNextNode(mRailGraphIter);
        if (!selectEdgeStartEvent(mRailGraphIter)) {
            setNerve(&NrvTrickRabbit::TrickRabbitNrvDemoEnd::sInstance);
            return;
        }
    }
    if (tryDemoJumpStart())
        return;
}

void TrickRabbit::exeDemoJumpStart() {
    if (MR::isFirstStep(this))
        _CC = calcJumpStartTime();

    doJumpStart();

    if (tryDemoJump())
        return;
}

void TrickRabbit::exeDemoJump() {
    if (MR::isFirstStep(this))
        setUpJumpParamFromJMap();

    TVec3f stack_14(mPosition);
    doJump();
    if (tryDemoJump()) {
        TVec3f stack_8(mPosition-stack_14);
        mVelocity.set(stack_8);
    }
}

void TrickRabbit::exeDemoEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "FollowMe");
        if (_E1) {
            MR::showModel(this);
            MR::validateShadow(this, 0);
        }
    }
    doWait();
    if (MR::isStep(this,0x1E)) {
        endDemoRunnaway();
    }
}

void TrickRabbit::exeWait() {
    if (MR::isFirstStep(this)) {
        if (_E1) {
            MR::showModel(this);
            MR::validateShadow(this, 0);
        }
    
        MR::startAction(this, "FollowMe");
    }

    doWait();

    if (tryRunaway())
        return;
}

void TrickRabbit::exeRunaway() {
    doRunaway();
    if (_D0 < 125.0f) {
        MR::moveNextNode(mRailGraphIter);
        TVec3f stack_8(mPosition-*MR::getPlayerPos());
        selectEdgeRunnaway(mRailGraphIter, stack_8, 0.0f);
    }

    if (!tryStop() && !tryJumpStart() && tryBrakeTurn())
        return;
}

void TrickRabbit::exeBrakeTurn() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "Brake");

    updateFootPrint();

    if (!MR::isBindedGround(this))
        MR::addVelocityToGravity(this, 1.0f);

    MR::attenuateVelocity(this, 0.93000001f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    TVec3f stack_8(*MR::getNextNodePosition(mRailGraphIter)-mPosition);
    MR::turnVecToVecCosOnPlane(&_9C, stack_8, mGravity, -1.0f);

    if (tryBrakeTurnEnd())
        return;
}

void TrickRabbit::exeJumpStart() {
    if (MR::isFirstStep(this)) {
        _CC = calcJumpStartTime();
    }

    doJumpStart();

    if (tryJump())
        return;
}

void TrickRabbit::exeJump() {
    if (MR::isFirstStep(this)) 
        setUpJumpParamFromJMap();

    TVec3f stack_14(mPosition);
    doJump();

    if (tryJumpEnd()) {
        TVec3f stack_8(mPosition-stack_14);
        mVelocity.set(stack_8);
    }
}

void TrickRabbit::exeFallDown() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "Fall");

    if (!MR::isBindedGround(this)) {
        if (!MR::isBindedGround(this))
            MR::addVelocityToGravity(this, 1.0f);
        
        MR::attenuateVelocity(this, 0.85000002f);
    }
    else
        MR::attenuateVelocity(this, 0.85000002f);

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (tryEndFallDown())
        return;
}

void TrickRabbit::exeGetUpFromFallDown() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "FallDownEnd");

    MR::zeroVelocity(this);

    if (tryComebackRouteStart())
        return;
}

void TrickRabbit::exeComebackRouteStart() {
    if (MR::isFirstStep(this))
        _CC = 8;

    doJumpStart();

    if (tryComebackRoute())
        return;
}

void TrickRabbit::exeComebackRoute() {
    if (MR::isFirstStep(this))
        setUpJumpParam();

    TVec3f stack_14(mPosition);
    doJump();

    if (tryJumpEnd()) {
        TVec3f stack_8(mPosition-stack_14);
        mVelocity.set(stack_8);
    }
}

void TrickRabbit::exeTumble() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "FallDownLoop");

    if (!MR::isBindedGround(this)) {
        if (!MR::isBindedGround(this)) {
            MR::addVelocityToGravity(this, 1.0f);
        }

        MR::attenuateVelocity(this, 0.99000001f);
        MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    }
    else
        MR::zeroVelocity(this);


    if (tryEndTumble())
        return;
}

void TrickRabbit::exeGetUp() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "FallDownEnd");

    MR::zeroVelocity(this);

    if (tryEndGetUp())
        return;
}

void TrickRabbit::exeCaught() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(0x3C);
        MR::startSystemSE("SE_SY_TOTAL_COMPLETE", -1, -1);
    }
    MR::updateActorStateAndNextNerve(this, (ActorStateBaseInterface*)mRabbitStateCaught, &NrvTrickRabbit::TrickRabbitNrvWaitPowerStarDemo::sInstance);
}

void TrickRabbit::exeWaitPowerStarDemo() {
    TVec3f stack_20;
    if (MR::isFirstStep(this)) {
        MR::endDemo(this, "捕まり");
        MR::startAfterBossBGM();
        TVec3f stack_14(mPosition-(mGravity*200.0f));
        MR::requestAppearPowerStar(this, stack_14);
        MR::startAction(this, "Wait");
        mFootPrint->clear();
    }

    MR::zeroVelocity(this);
    
    if (!MR::isOnGround(this) && !MR::isBindedGround(this))
        MR::addVelocityToGravity(this, 1.0f);

    if (MR::isNearPlayer(this, 70.0f)) {
        MR::getPlayerFrontVec(&stack_20);
        mVelocity+=stack_20;
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::turnDirectionToPlayerDegreeHorizon(this, &_9C, 30.0f);
    
    if (tryEndPowerStarDemo())
        MR::forwardNode(mTalkMessageCtrl);
}

void TrickRabbit::exeGiveUp() {
    if (MR::isFirstStep(this)) {
        mRabbitStateWaitStart->setTalkActionName("GiveUpTalk");
        mRabbitStateWaitStart->_1C = false;
    }
    MR::updateActorState(this, (ActorStateBaseInterface*)mRabbitStateWaitStart);
}

void TrickRabbit::doWait() {
    if (!MR::isBindedGround(this)) 
        MR::addVelocityToGravity(this, 1.0f);

    MR::attenuateVelocity(this, 0.85000002f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::turnDirectionToPlayerDegreeHorizon(this, &_9C, 10.0f);
}

void TrickRabbit::doRunaway() {
    if (MR::isFirstStep(this))
        MR::startAction(this, "Run");

    if (MR::checkPassBckFrame(this, 0.0f))
       updateFootPrint();

    if (MR::checkPassBckFrame(this, 3.0f))
        MR::startSound(this, "SE_SM_RABBIT_JUMP", -1, -1);

    TVec3f stack_14(*MR::getNextNodePosition(mRailGraphIter));
    f32 arg = MR::getSelectEdgeArg1(mRailGraphIter);
    arg=arg/100.0f;

    if (arg < 0.0f)
        arg = 1.0f;

    if (!MR::isBindedGround(this)) 
        MR::addVelocityToGravity(this, 1.0f);

    TVec3f stack_8(stack_14-mPosition);
    addMovingAccel(stack_8, 2.4000001f*arg);
    addKeepRouteRange(1.0f, 100.0f, 400.0f);
    MR::attenuateVelocity(this, 0.85000002f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void TrickRabbit::doJumpStart() {
    if (MR::isFirstStep(this)) 
        MR::startAction(this, "JumpStart");

    updateFootPrint();

    if (MR::isBckOneTimeAndStopped(this)) 
        MR::startAction(this, "JumpStartLoop");

    TVec3f stack_8(*MR::getNextNodePosition(mRailGraphIter)-mPosition);
    MR::turnVecToVecCosOnPlane(&_9C, stack_8, mGravity, 0.99000001f);

    if (!MR::isBindedGround(this))
        MR::addVelocityToGravity(this, 1.0f);

    MR::attenuateVelocity(this, 0.88f);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void TrickRabbit::doJump() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Jump");
        MR::startSound(this, "SE_SM_RABBIT_JUMP", -1, -1);
        MR::offBind(this);
    }
    MR::turnVecToVecCosOnPlane(&_9C, (*MR::getNextNodePosition(mRailGraphIter)-mPosition), mGravity, 0.89999998f);
    TVec3f stack_5C;
    mParabolicPath->calcPosition(&stack_5C, MR::calcNerveRate(this, _CC));

    if (!checkDivingThicket((mPosition-(mGravity*70.0f)), stack_5C-mPosition))
        checkDivingThicket((stack_5C-(mGravity*70.0f)), (mPosition-stack_5C));

    mPosition.set(stack_5C);
}

void TrickRabbit::tearDownWait() {
    MR::invalidateClipping(this);

    if (_E1) {
        MR::hideModelAndOnCalcAnim(this);
        MR::invalidateShadow(this, 0);
    }
}

bool TrickRabbit::checkDivingThicket(const TVec3f& rVec1, const TVec3f& rVec2) {
    Triangle stack_44 = Triangle();
    TPos3f stack_14;
    TVec3f stack_8;

    if (!MR::getFirstPolyOnLineToMap(&stack_8, &stack_44, rVec1, rVec2))
        return false;

    MR::makeMtxUpNoSupportPos(&stack_14, *stack_44.getNormal(0), stack_8);
    MR::emitEffectHit(this, stack_14, "ThicketJump");
    MR::startSound(this, "SE_SM_RABBIT_THICKET", -1, -1);
    return true;
}

bool TrickRabbit::selectEdgeStartEvent(RailGraphIter* pIter) {
    bool b = false;
    pIter->watchStartEdge();
    while(!pIter->isWatchEndEdge()) {
        if (!MR::isWatchedPrevEdge(pIter) && MR::getWatchEdgeArg7(pIter) < -1) {
            pIter->selectEdge();
            b = true;
        }
        
        pIter->watchNextEdge();

    };
    return b;
}

// 100% but has gotos
void TrickRabbit::selectEdgeRunnaway(RailGraphIter* pIter, const TVec3f& rVec, f32 f) {
    TVec3f stack_14;
    MR::normalizeOrZero(rVec, &stack_14);
    f32 v10 = -3.4028235e38f;
    s32 v17;
    s32 r29 = 0;
    s32 r28 = 0;
    s32 r27 = 0;
    pIter->watchStartEdge();

    while (!pIter->isWatchEndEdge()) {
        s32 arg = MR::getWatchEdgeArg7(pIter);
        if (arg >= -1) {
            TVec3f stack_8;
            MR::calcWatchEdgeDirection(pIter, &stack_8);
            f32 dot = stack_8.dot(stack_14);
            v17 = 1;
            
            if (arg != -1) {
                if (arg != _D8)
                    v17 = 0;
            }
            
            bool v18 = dot > f;
            bool isWatched = !MR::isWatchedPrevEdge(pIter);
            bool v20 = 0;


            if (!r29 || v17) {
                if (!r29 && v17)
                    v20 = 1;

                if (!v20) {
                    if (r28 && !v18) {
                        goto endif;
                    }
                    if (!r28 && v18) {
                            v20 = 1;
                        }
                }

                if (!v20) {
                    if (r27 && !isWatched){
                        goto endif;
                    }
                    if (!r27 && isWatched)
                    v20 = 1;
                }

                if (!v20) {
                    if (v10 < dot)
                        v20 = 1;
                }

                if (v20) {
                    v10 = dot;
                    r29 = v17;
                    r28 = v18;
                    r27 = isWatched;
                    pIter->selectEdge();
                }
            }
        }
        endif:
        pIter->watchNextEdge();
    }
}

namespace NrvTrickRabbit {
    void TrickRabbitNrvComebackRoute::executeOnEnd(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        MR::onBind(pThis);
    }

    void TrickRabbitNrvComebackRoute::execute(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        pThis->exeComebackRoute();
    }

    void TrickRabbitNrvFallDownLand::execute(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        if (MR::isBckStopped(pThis)) {
            MR::startAction(pThis, "FallDownLoop");
        }
        pThis->tryEndFallDownLand();
    }

    void TrickRabbitNrvJump::executeOnEnd(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        MR::onBind(pThis);
    }

    void TrickRabbitNrvJump::execute(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        pThis->exeJump();
    }

    void TrickRabbitNrvWait::executeOnEnd(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        pThis->tearDownWait();
    }

    void TrickRabbitNrvWait::execute(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        pThis->exeWait();
    }

    void TrickRabbitNrvDemoJump::executeOnEnd(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        MR::onBind(pThis);
    }

    void TrickRabbitNrvDemoJump::execute(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        pThis->exeDemoJump();
    }

    void TrickRabbitNrvTryStartDemoRunnaway::execute(Spine* pSpine) const {
        TrickRabbit* pThis = (TrickRabbit*)pSpine->mExecutor;
        pThis->tryStartDemoRunnaway();
    }

    TrickRabbitNrvWaitStart(TrickRabbitNrvWaitStart::sInstance);
    TrickRabbitNrvTryStartDemoRunnaway(TrickRabbitNrvTryStartDemoRunnaway::sInstance);
    TrickRabbitNrvDemoRunaway(TrickRabbitNrvDemoRunaway::sInstance);
    TrickRabbitNrvDemoJumpStart(TrickRabbitNrvDemoJumpStart::sInstance);
    TrickRabbitNrvDemoJump(TrickRabbitNrvDemoJump::sInstance);
    TrickRabbitNrvDemoEnd(TrickRabbitNrvDemoEnd::sInstance);
    TrickRabbitNrvWait(TrickRabbitNrvWait::sInstance);
    TrickRabbitNrvRunaway(TrickRabbitNrvRunaway::sInstance);
    TrickRabbitNrvJumpStart(TrickRabbitNrvJumpStart::sInstance);
    TrickRabbitNrvJump(TrickRabbitNrvJump::sInstance);
    TrickRabbitNrvBrakeTurn(TrickRabbitNrvBrakeTurn::sInstance);
    TrickRabbitNrvFallDown(TrickRabbitNrvFallDown::sInstance);
    TrickRabbitNrvFallDownLand(TrickRabbitNrvFallDownLand::sInstance);
    TrickRabbitNrvGetUpFromFallDown(TrickRabbitNrvGetUpFromFallDown::sInstance);
    TrickRabbitNrvComebackRouteStart(TrickRabbitNrvComebackRouteStart::sInstance);
    TrickRabbitNrvComebackRoute(TrickRabbitNrvComebackRoute::sInstance);
    TrickRabbitNrvTumble(TrickRabbitNrvTumble::sInstance);
    TrickRabbitNrvGetUp(TrickRabbitNrvGetUp::sInstance);
    TrickRabbitNrvCaught(TrickRabbitNrvCaught::sInstance);
    TrickRabbitNrvWaitPowerStarDemo(TrickRabbitNrvWaitPowerStarDemo::sInstance);
    TrickRabbitNrvGiveUp(TrickRabbitNrvGiveUp::sInstance);
}
