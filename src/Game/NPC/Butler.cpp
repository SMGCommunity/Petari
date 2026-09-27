#include "Game/NPC/Butler.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/StarPieceGroup.hpp"
#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/Screen/GalaxyMapController.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

void Butler_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    const char* const cDemoNameDomeLecture1 = "ドームレクチャー１";
    const char* const cDemoNameDomeLecture2 = "ドームレクチャー２";
    const char* const cDemoNameButlerReport = "バトラー報告";
    const char* const cDemoNameStarPiece1 = "スターピース解説前半";
    const char* const cDemoNameStarPiece2 = "スターピース解説後半";
    const char* const cDemoNameGreenDriver = "バトラーグリーンドライバ説明";
    const char* const cMessageId[] = {
        "AstroDome_Butler001", "AstroDome_Butler002",   "AstroDome_Butler003",   "AstroDome_Butler006",
        "AstroDome_Butler007", "AstroGalaxy_Butler005", "AstroGalaxy_Butler006",
    };
};  // namespace

void Butler_FORCE_MATCH_SDATA2_RADIUS() {
    (void)50.0f;
    (void)100.0f;
}

namespace NrvButler {
    NEW_NERVE(ButlerNrvStarPieceReaction, Butler, StarPieceReaction);
    NEW_NERVE(ButlerNrvDemo, Butler, Demo);
    NEW_NERVE(ButlerNrvDemoDomeLecture2, Butler, DemoDomeLecture2);
    NEW_NERVE(ButlerNrvDemoStarPiece2, Butler, DemoStarPiece2);
    NEW_NERVE(ButlerNrvDemoShowGalaxyMap, Butler, DemoShowGalaxyMap);
    NEW_NERVE(ButlerNrvWaitStartDemo, Butler, DemoWait);
};  // namespace NrvButler

Butler::Butler(const char* pName) : NPCActor(pName) {
    mTalkMessage = nullptr;
    _160 = false;
    _164 = 0;
    _168 = 0;
    mButlerState = nullptr;
    _170 = false;
    _171 = false;
}

void Butler::init(const JMapInfoIter& rIter) {
    const char* dome;
    MR::getJMapInfoArg0NoInit(rIter, &_170);
    NPCActorCaps caps = "Butler";
    caps.setDefault();
    caps.mSensorJoint = "Body";
    caps.mSensorSize = 50.0f;
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = 0.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mUseShadow = true;
    caps.mBinder = 0;
    NPCActor::initialize(rIter, caps);
    setDefaults();
    _13C = "Spin";
    mParam.setMoveAction("Wait", "Wait");
    mParam.setTalkAction("Talk", "Talk");
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);

    if (MR::isOnGameEventFlagEndButlerStarPieceLecture()) {
        dome = "AstroGalaxy_Butler000";
    } else {
        dome = "AstroDome_Butler017";
    }

    mButlerState = new ButlerStateStarPieceReaction(this, rIter, dome);
    mButlerState->init();
    initTalkCtrlArray(rIter);

    if (_170) {
        initForAstroDome(rIter);
    } else {
        initForAstroGalaxy(rIter);
    }
}

void Butler::appear() {
    if (!MR::isButlerMapAppear()) {
        MR::setDefaultPose(this);
        LiveActor::appear();
        forceNerveToWait();
    }
}

void Butler::kill() {
    if (!MR::isDead(this)) {
        MR::forceDeleteEffectAll(this);
        NPCActor::kill();
    }
}

void Butler::killIfBatlerMapAppear() {
    if (MR::isButlerMapAppear()) {
        kill();
    } else {
        MR::setDefaultPose(this);
        MR::startBckNoInterpole(this, "Wait");
    }
}

void Butler::startDemoButlerReport(const char* pEvent) {
    s32 eventNum = 2;
    s32 executingStorySequenceEventNum = GameSequenceFunction::getExecutingStorySequenceEventNum();
    switch (executingStorySequenceEventNum) {
    case 4:
        eventNum = 2;
        break;

    case 5:
        eventNum = 3;
        break;

    case 6:
        eventNum = 4;
        break;

    case 7:
        eventNum = 5;
        break;

    case 8:
        eventNum = 6;
        break;

    default:
        break;
    }

    DemoFunction::setDemoTalkMessageCtrlDirect(this, mTalkMessage[eventNum << 0], pEvent);
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(GET_NERVE(Butler, ButlerNrvDemo));
}

void Butler::startDemoDomeLecture1() {
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(GET_NERVE(Butler, ButlerNrvDemo));
    MR::endStartPosCamera();
}

void Butler::startDemoDomeLecture2() {
    s32 i = 0;
    if (!MR::isOnGameEventFlagEndButlerDomeLecture()) {
        i = 0;
    } else if (!MR::isOnGameEventFlagEndButlerGalaxyMoveLecture()) {
        i = 1;
    }

    DemoFunction::setDemoTalkMessageCtrlDirect(this, mTalkMessage[i], "ドームレクチャー２");
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(GET_NERVE(Butler, ButlerNrvDemo));
    setNerve(GET_NERVE(Butler, ButlerNrvDemoDomeLecture2));
}

void Butler::startDemoStarPiece1() {
    _160 = true;
    _164 = false;
    _168 = false;
    MR::setSensorRadius(this, "Body", 100.0f);
    MR::onSwitchB(this);
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(GET_NERVE(Butler, ButlerNrvDemo));
    MR::endStartPosCamera();
}

void Butler::startDemoStarPiece2() {
    MR::setSensorRadius(this, "Body", 50.0f);
    MR::offSwitchB(this);
    static_cast< StarPieceGroup* >(MR::getPairedGroupMember(this))->forceKillStarPieceAll(false);
    MR::getPairedGroupMember(this)->kill();

    _160 = false;

    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(GET_NERVE(Butler, ButlerNrvDemo));
    setNerve(GET_NERVE(Butler, ButlerNrvDemoStarPiece2));
}

void Butler::tryStartShowGalaxyMap() {
    bool storySequence = GameSequenceFunction::getExecutingStorySequenceEventNum() == 8;
    if (storySequence) {
        setNerve(GET_NERVE(Butler, ButlerNrvDemoShowGalaxyMap));
    }
}

void Butler::resetStatus() {
    MR::setDefaultPose(this);
    MR::startBckNoInterpole(this, "Wait");
}

bool Butler::messageBranchFunc(u32 msg) {
    bool stupidBool;
    const char* triLeg;
    switch (msg) {
    case 0:
        return _160;
    case 1:
        stupidBool = false;
        triLeg = "TriLegLv1Galaxy";
        if (MR::isOnGameEventFlagGalaxyOpen(triLeg) || MR::canOpenGalaxy(triLeg)) {
            stupidBool = true;
        }

        return stupidBool;
    default:
        return false;
    }
}

void Butler::control() {
    if (_160) {
        if (!MR::isDemoActive()) {
            MR::requestStarPieceLectureGuidance();
            MR::requestCounterLayoutAppearanceForTicoEat(false);
        }

        tryReplaceStarPieceIfExecLecture();
    }

    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED");
        MR::startSound(this, "SE_SV_BUTLER_TRAMPLED");
    }

    if (NPCActor::isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_BUTLER_POINT");
    }

    if (_160) {
        bool temp = _171;
        if (MR::isStarPointerPointing1P(this, "弱", false, false)) {
            _171 = true;
        } else {
            _171 = false;
        }

        if (!temp && _171 == 1) {
            MR::startSystemSE("SE_SY_TICOFAT_POINT");
        }
    }

    NPCActor::control();
}

bool Butler::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (_160 || MR::isOnGameEventFlagEndButlerStarPieceLecture()) {
        if (MR::isMsgLockOnStarPieceShoot(msg)) {
            return true;
        } else if (MR::isMsgStarPieceReflect(msg)) {
            return false;
        } else if (MR::isMsgStarPieceAttack(msg)) {
            if (_160) {
                _164++;
                if (tryStartStarPieceReaction()) {
                    MR::startSystemSE("SE_SY_STAR_PIECE_STOCK_MAX");
                }
            } else {
                bool v1 = isNerve(mWaitNerve) || isNerve(GET_NERVE(Butler, ButlerNrvStarPieceReaction));

                if (v1) {
                    setNerve(GET_NERVE(Butler, ButlerNrvStarPieceReaction));
                }
            }

            return true;
        }
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

bool Butler::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::trySyncKillMsgSelectStart(this, msg)) {
        return true;
    } else {
        return MR::isMsgHitmarkEmit(msg);
    }
}

void Butler::initTalkCtrlArray(const JMapInfoIter& rIter) {
    mTalkMessage = new TalkMessageCtrl*[0x7];

    for (s32 i = 0; i < 7; i++) {
        mTalkMessage[i] = createTalkCtrl(rIter, ::cMessageId[i]);
    }
}

namespace {
    inline void initButlerDemo(Butler* pActor, const JMapInfoIter& rIter, const char* pDemoName, const char* pAnimName, TalkMessageCtrl* pTalkCtrl,
                               const MR::FunctorBase& rStart, const MR::FunctorBase& rReset) {
        MR::initDemoSheetTalkAnim(pActor, rIter, pDemoName, pAnimName, pTalkCtrl);
        MR::registerDemoActionFunctorDirect(pActor, rStart, pDemoName, "開始");
        MR::registerDemoActionFunctorDirect(pActor, rReset, pDemoName, "バトラーリセット");
    }
}  // namespace

void Butler::initForAstroDome(const JMapInfoIter& rIter) {
    MR::tryRegisterDemoCast(this, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "パワースター帰還", rIter);

    static void (Butler::*pResetReport)() = &Butler::killIfBatlerMapAppear;
    static void (Butler::*pStartReport)(const char*) = &Butler::startDemoButlerReport;
    ::initButlerDemo(this, rIter, ::cDemoNameButlerReport, "DemoButlerReport", mTalkMessage[2],
                     MR::Functor(this, pStartReport, ::cDemoNameButlerReport), MR::Functor(this, pResetReport));

    const char* demoNameDomeLecture1 = ::cDemoNameDomeLecture1;
    MR::registerDemoCast(this, demoNameDomeLecture1, rIter);
    DemoFunction::tryCreateDemoTalkAnimCtrlForSceneDirect(this, demoNameDomeLecture1, rIter, "DemoButlerDomeLecture1", nullptr, 0, -1);
    DemoFunction::registerDemoTalkMessageCtrlDirect(this, createTalkCtrl(rIter, "AstroDome_Butler023"), demoNameDomeLecture1);
    MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &Butler::startDemoDomeLecture1), demoNameDomeLecture1, nullptr);
    MR::initDemoSheetTalkAnimFunctor(this, rIter, ::cDemoNameDomeLecture2, "DemoButlerDomeLecture2", getTalkMessage(0),
                                     MR::Functor(this, &Butler::startDemoDomeLecture2));

    static void (Butler::*pResetPiece1)() = &Butler::resetStatus;
    static void (Butler::*pStartPiece1)() = &Butler::startDemoStarPiece1;
    ::initButlerDemo(this, rIter, ::cDemoNameStarPiece1, "DemoButlerStarPiece1", createTalkCtrl(rIter, "AstroDome_Butler011"),
                     MR::Functor(this, pStartPiece1), MR::Functor(this, pResetPiece1));

    static void (Butler::*pResetPiece2)() = &Butler::resetStatus;
    static void (Butler::*pStartPiece2)() = &Butler::startDemoStarPiece2;
    ::initButlerDemo(this, rIter, ::cDemoNameStarPiece2, "DemoButlerStarPiece2", createTalkCtrl(rIter, "AstroDome_Butler014"),
                     MR::Functor(this, pStartPiece2), MR::Functor(this, pResetPiece2));

    MR::joinToGroupArray(this, rIter, nullptr, 32);
    MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc_Inline(this, &Butler::messageBranchFunc));
    SphereSelectorFunction::registerTarget(this);

    if (MR::isButlerMapAppear()) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void Butler::initForAstroGalaxy(const JMapInfoIter& rIter) {
    const char* nameGreenDriver = ::cDemoNameGreenDriver;
    MR::registerDemoCast(this, nameGreenDriver, rIter);
    DemoFunction::registerDemoTalkMessageCtrlDirect(this, mTalkMessage[5], nameGreenDriver);
    MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &Butler::startDemoButlerReport, nameGreenDriver), nameGreenDriver, "開始");
    MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &Butler::tryStartShowGalaxyMap), nameGreenDriver, "マップ表示");
    const char* grandStarName = AstroDemoFunction::getGrandStarReturnDemoName(0);
    AstroDemoFunction::tryRegisterDemo(this, grandStarName, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "ロゼッタ状況説明デモ", rIter);
    AstroDemoFunction::tryRegisterSimpleCastIfAstroGalaxy(this);

    if (MR::isButlerMapAppear()) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

TalkMessageCtrl* Butler::createTalkCtrl(const JMapInfoIter& rIter, const char* pTalk) NO_INLINE {
    TVec3f vec;
    vec.setPSZeroVec();

    return MR::createTalkCtrlDirectOnRootNodeAutomatic(this, rIter, pTalk, vec, MR::getJointMtx(this, "Body"));
}

void Butler::forceNerveToWait() {
    if (!NPCActor::isEmptyNerve()) {
        NPCActor::popNerve();
    }

    setNerve(mWaitNerve);
}

void Butler::tryReplaceStarPieceIfExecLecture() {
    if (MR::getStarPieceNum() != 0) {
        return;
    }

    if (MR::isDead(MR::getPairedGroupMember(this))) {
        return;
    }

    if (static_cast< StarPieceGroup* >(MR::getPairedGroupMember(this))->isExistAnyStarPiece()) {
        return;
    }

    _168++;

    if (_168 != 120) {
        return;
    }

    static_cast< StarPieceGroup* >(MR::getPairedGroupMember(this))->forceReplaceStarPieceAll();

    MR::startSystemSE("SE_SY_LECT_STAR_PIECE_APR");

    _168 = 0;
}

bool Butler::tryStartStarPieceReaction() {
    bool isNerveOn = isNerve(mWaitNerve) || isNerve(GET_NERVE(Butler, ButlerNrvStarPieceReaction));

    if (isNerveOn && _164 <= 5) {
        setNerve(GET_NERVE(Butler, ButlerNrvStarPieceReaction));
        return (_164 >= 5);
    } else {
        isNerveOn = isNerve(mWaitNerve) || isNerve(GET_NERVE(Butler, ButlerNrvStarPieceReaction));

        if (!isNerveOn && _164 == 5) {
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "スターピース解説後半", nullptr, GET_NERVE(Butler, ButlerNrvWaitStartDemo), nullptr);
            return true;
        }

        if (_164 > 5 && !isNerve(GET_NERVE(Butler, ButlerNrvStarPieceReaction)) && !isNerve(GET_NERVE(Butler, ButlerNrvWaitStartDemo))) {
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "スターピース解説後半", nullptr, GET_NERVE(Butler, ButlerNrvWaitStartDemo), nullptr);
            return true;
        } else {
            return false;
        }
    }

    return false;
}

void Butler::exeStarPieceReaction() {
    bool reaction = _160 && _164 >= 5;

    if (MR::isFirstStep(this)) {
        mButlerState->appear();
        if (reaction) {
            mButlerState->_14 = false;
        }
    }

    if (mButlerState->update()) {
        if (reaction) {
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "スターピース解説後半", nullptr, GET_NERVE(Butler, ButlerNrvWaitStartDemo), nullptr);
        } else {
            forceNerveToWait();
        }
    }
}

void Butler::exeDemo() {
    if (!MR::isTimeKeepDemoActive()) {
        MR::validateClipping(this);
        MR::setDefaultPose(this);
        forceNerveToWait();
    }
}

void Butler::exeDemoDomeLecture2() {
    MR::setNPCActorPose(this, -MR::getCamZdir(), MR::getCamYdir(), MR::getCamPos());
    exeDemo();
}

void Butler::exeDemoStarPiece2() {
    if (MR::isStep(this, 1)) {
        MR::resetPlayerEffect();
    }

    if (MR::isDemoPartStep("説明１→説明２", 29)) {
        MR::overlayWithPreviousScreen(2);
    }

    if (MR::isDemoPartLastStep("終了")) {
        MR::onGameEventFlagEndButlerStarPieceLecture();
        setNerve(GET_NERVE(Butler, ButlerNrvDemo));
    }
}

void Butler::exeDemoShowGalaxyMap() {
    if (MR::isFirstStep(this)) {
        MR::pauseTimeKeepDemo(this);
    }

    if (MR::isStep(this, 30)) {
        MR::startAstroMapLayoutForChallengeGalaxyDiscover();
    }

    if (MR::isStep(this, 70)) {
        MR::resumeTimeKeepDemo(this);
        setNerve(GET_NERVE(Butler, ButlerNrvDemo));
    }
}

void Butler::exeDemoWait() {
}
