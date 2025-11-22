#include "Game/NPC/Butler.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/StarPieceGroup.hpp"
#include "Game/NPC/ButlerStateStarPieceReaction.hpp"
#include "Game/NPC/NPCActor.hpp"
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
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/types.h"

namespace {
    const char* cMessageId[] = {
        "AstroDome_Butler002", "AstroDome_Butler003", "AstroDome_Butler006", "AstroDome_Butler007", "AstroGalaxy_Butler005", "AstroGalaxy_Butler006",
    };
    const char* cDemoNameGreenDriver = "バトラーグリーンドライバ説明";
    const char* cDemoNameButlerReport = "バトラー報告";
    const char* cDemoNameDomeLecture1 = "ドームレクチャー１";
    const char* cDemoNameDomeLecture2 = "ドームレクチャー2";
    const char* cDemoNameStarPiece1 = "スターピース解説前半";
    const char* cDemoNameStarPiece2 = "スターピース解説後半";
};  // namespace

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

NPCActor::~NPCActor() {}

Butler::~Butler() {}

void Butler::init(const JMapInfoIter& rIter) {
    const char* dome;
    MR::getJMapInfoArg0NoInit(rIter, &_170);
    NPCActorCaps caps = "Butler";
    caps.setDefault();
    caps._44 = "Body";
    caps.mSensorSize = 50.0f;
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = 0.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mUseShadow = true;
    caps._38 = 0;
    NPCActor::initialize(rIter, caps);
    setDefaults2();
    _13C = "Spin";
    const char* wait = "Wait";
    const char* talk = "Talk";
    mParam._14 = wait;
    mParam._18 = wait;
    mParam._1C = talk;
    mParam._20 = talk;
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

void Butler::startDemoButlerReport(const char* event) {
    s32 eventNum = 2;
    s32 executingStorySequenceEventNum = GameSequenceFunction::getExecutingStorySequenceEventNum();
    // executingStorySequenceEventNum is an enum not yet documented
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
    DemoFunction::setDemoTalkMessageCtrlDirect(this, mTalkMessage[eventNum << 0], event);
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(&NrvButler::ButlerNrvDemo::sInstance);
}

void Butler::startDemoDomeLecture1() {
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(&NrvButler::ButlerNrvDemo::sInstance);
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
    setNerve(&NrvButler::ButlerNrvDemo::sInstance);
    setNerve(&NrvButler::ButlerNrvDemoDomeLecture2::sInstance);
}

void Butler::startDemoStarPiece1() {
    _160 = true;
    _164 = false;
    _168 = false;
    MR::setSensorRadius(this, "Body", 100.0f);
    MR::onSwitchB(this);
    MR::invalidateClipping(this);
    LiveActor::appear();
    setNerve(&NrvButler::ButlerNrvDemo::sInstance);
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
    setNerve(&NrvButler::ButlerNrvDemo::sInstance);
    setNerve(&NrvButler::ButlerNrvDemoStarPiece2::sInstance);
}

void Butler::tryStartShowGalaxyMap() {
    bool storySequence = GameSequenceFunction::getExecutingStorySequenceEventNum() == 8;
    if (storySequence) {
        setNerve(&NrvButler::ButlerNrvDemoShowGalaxyMap::sInstance);
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
        MR::startSound(this, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_BUTLER_TRAMPLED", -1, -1);
    }

    if (NPCActor::isPointingSe()) {
        MR::startDPDHitSound();
        MR::startSound(this, "SE_SV_BUTLER_POINT", -1, -1);
    }

    if (_160) {
        bool temp = _171;
        if (MR::isStarPointerPointing1P(this, "弱", false, false)) {
            _171 = true;
        } else {
            _171 = false;
        }

        if (!temp && _171 == 1) {
            MR::startSystemSE("SE_SY_TICOFAT_POINT", -1, -1);
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
                    MR::startSystemSE("SE_SY_STAR_PIECE_STOCK_MAX", -1, -1);
                }
            } else {
                bool v1 = isNerve(mWaitNerve) || isNerve(&NrvButler::ButlerNrvStarPieceReaction::sInstance);

                if (v1) {
                    setNerve(&NrvButler::ButlerNrvStarPieceReaction::sInstance);
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
        mTalkMessage[i] = createTalkCtrl(rIter, cMessageId[i]);
    }
}

void Butler::initForAstroDome(const JMapInfoIter& rIter) {
    MR::tryRegisterDemoCast(this, rIter);
    AstroDemoFunction::tryRegisterDemo(this, "パワースター帰還", rIter);
    const MR::FunctorBase& func1 = MR::Functor(this, &Butler::killIfBatlerMapAppear);
    const char* demoNameButlerReport = cDemoNameButlerReport;
    const MR::FunctorBase& func2 = MR::Functor(this, &Butler::startDemoButlerReport, demoNameButlerReport);
    MR::initDemoSheetTalkAnim(this, rIter, demoNameButlerReport, "DemoButlerReport", mTalkMessage[2]);
    MR::registerDemoActionFunctorDirect(this, func2, demoNameButlerReport, "開始");
    MR::registerDemoActionFunctorDirect(this, func1, demoNameButlerReport, "バトラーリセット");
    const char* demoNameDomeLecture1 = cDemoNameDomeLecture1;
    MR::registerDemoCast(this, demoNameDomeLecture1, rIter);
    DemoFunction::tryCreateDemoTalkAnimCtrlForSceneDirect(this, demoNameDomeLecture1, rIter, "DemoButlerDomeLecture1", nullptr, 0, 0);
    DemoFunction::registerDemoTalkMessageCtrlDirect(this, createTalkCtrl(rIter, "AstroDome_Butler023"), demoNameDomeLecture1);
    MR::registerDemoActionFunctorDirect(this, MR::Functor(this, &Butler::startDemoDomeLecture1), demoNameDomeLecture1, nullptr);
    const MR::FunctorBase& func3 = MR::Functor(this, &Butler::startDemoDomeLecture2);
    TalkMessageCtrl* talkMsg1 = *mTalkMessage;
    MR::initDemoSheetTalkAnimFunctor(this, rIter, cDemoNameDomeLecture2, "DemoButlerDomeLecture2", talkMsg1, func3);
    const MR::FunctorBase& func4 = MR::Functor(this, &Butler::startDemoStarPiece1);
    const MR::FunctorBase& func5 = MR::Functor(this, &Butler::resetStatus);
    TalkMessageCtrl* talkMsg2 = createTalkCtrl(rIter, "AstroDome_Butler011");
    const char* demoNameStarPiece1 = cDemoNameStarPiece1;
    MR::initDemoSheetTalkAnim(this, rIter, demoNameStarPiece1, "DemoButlerStarPiece1", talkMsg2);
    MR::registerDemoActionFunctorDirect(this, func4, demoNameStarPiece1, "開始");
    MR::registerDemoActionFunctorDirect(this, func5, demoNameStarPiece1, "バトラーリセット");
    const MR::FunctorBase& func6 = MR::Functor(this, &Butler::startDemoStarPiece2);
    const MR::FunctorBase& func7 = MR::Functor(this, &Butler::resetStatus);
    TalkMessageCtrl* talkMsg3 = createTalkCtrl(rIter, "AstroDome_Butler014");
    const char* demoNameStarPiece2 = cDemoNameStarPiece2;
    MR::initDemoSheetTalkAnim(this, rIter, demoNameStarPiece2, "DemoButlerStarPiece2", talkMsg3);
    MR::registerDemoActionFunctorDirect(this, func6, demoNameStarPiece2, "開始");
    MR::registerDemoActionFunctorDirect(this, func7, demoNameStarPiece2, "バトラーリセット");
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
    const char* nameGreenDriver = cDemoNameGreenDriver;
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

TalkMessageCtrl* Butler::createTalkCtrl(const JMapInfoIter& rIter, const char* talk) {
    TVec3f vec;
    vec.setPSZeroVec();
    return MR::createTalkCtrlDirectOnRootNodeAutomatic(this, rIter, talk, vec, MR::getJointMtx(this, "Body"));
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

    MR::startSystemSE("SE_SY_LECT_STAR_PIECE_APR", -1, -1);

    _168 = 0;
}

bool Butler::tryStartStarPieceReaction() {
    bool isNerveOn = isNerve(mWaitNerve) || isNerve(&NrvButler::ButlerNrvStarPieceReaction::sInstance);

    if (isNerveOn && _164 <= 5) {
        setNerve(&NrvButler::ButlerNrvStarPieceReaction::sInstance);
        return (_164 >= 5);
    } else {
        isNerveOn = isNerve(mWaitNerve) || isNerve(&NrvButler::ButlerNrvStarPieceReaction::sInstance);

        if (!isNerveOn && _164 == 5) {
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "スターピース解説後半", nullptr, &NrvButler::ButlerNrvWaitStartDemo::sInstance,
                                                        nullptr);
            return true;
        }

        if (_164 > 5 && !isNerve(&NrvButler::ButlerNrvStarPieceReaction::sInstance) && !isNerve(&NrvButler::ButlerNrvWaitStartDemo::sInstance)) {
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "スターピース解説後半", nullptr, &NrvButler::ButlerNrvWaitStartDemo::sInstance,
                                                        nullptr);
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
            MR::requestStartTimeKeepDemoMarioPuppetable(this, "スターピース解説後半", nullptr, &NrvButler::ButlerNrvWaitStartDemo::sInstance,
                                                        nullptr);
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
    TVec3f neg;
    TVec3f dir = MR::getCamZdir();
    JMathInlineVEC::PSVECNegate(&dir, &neg);
    MR::setNPCActorPose(this, neg, MR::getCamYdir(), MR::getCamPos());
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
        setNerve(&NrvButler::ButlerNrvDemo::sInstance);
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
        setNerve(&NrvButler::ButlerNrvDemo::sInstance);
    }
}

void Butler::exeDemoWait() {}
