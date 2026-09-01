#include "Game/NPC/RunawayTico.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include <cstdio>

namespace {
    static const s32 sDemoCameraZoomOutStartFrame = 630;
    static const s32 sDemoCameraZoomOutEndFrame = 730;
    static const s32 sDemoCameraZoomOutBgmStartFrame = 617;
    static const s32 sDemoFadeOut = 90;
    static const s32 sDemoFadeIn = 75;
    static const s32 sDemoFadeWait = 30;
    static const f32 sAwayYOffset = -49.7357f;
    static const s32 sWaitSeStartFrame = 210;
    static const s32 sFloatSeStartFrame = 560;
    static const s32 sAppearSeStep = 23;
};  // namespace

namespace NrvRunawayTico {
    NEW_NERVE(RunawayTicoNrvGuide0, RunawayTico, Guide0);
    NEW_NERVE(RunawayTicoNrvGuide1, RunawayTico, Guide1);
    NEW_NERVE(RunawayTicoNrvWhiteIn, RunawayTico, WhiteIn);
    NEW_NERVE(RunawayTicoNrvWhiteOut, RunawayTico, WhiteOut);
    NEW_NERVE(RunawayTicoNrvWait, RunawayTico, Wait);
    NEW_NERVE(RunawayTicoNrvAppear, RunawayTico, Appear);
    NEW_NERVE(RunawayTicoNrvTalk, RunawayTico, Talk);
};  // namespace NrvRunawayTico

RunawayTico::RunawayTico(const char* pName) : Tico(pName), mCameraInfo(), mObjArg1(), mDemoCastID(), mIsStartRunaway(), mIsAllCaught() {
}

void RunawayTico::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == -1) {
        pCollector->addArchive("TicoBaby");
    } else {
        pCollector->addArchive("Tico");
        pCollector->addArchive("TicoMiddle");
        pCollector->addArchive("TicoLow");
    }
}

void RunawayTico::init(const JMapInfoIter& rIter) {
    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    s32 arg1 = 0;
    MR::getJMapInfoArg1NoInit(rIter, &arg1);
    mObjArg1 = arg1;

    s32 colorID;

    if (MR::tryRegisterDemoCast(this, rIter)) {
        mDemoCastID = MR::getDemoCastID(rIter);

        if (mDemoCastID > 0) {
            colorID = 0;
            mObjArg1 = 2;
        } else {
            colorID = -1;
            mObjArg1 = 1;
        }
    }

    initBase(rIter, colorID);
    initMessage(rIter, "RunawayTico");

    if (mObjArg1 == 1) {
        mCameraInfo = MR::createActorCameraInfo(rIter);
        MR::initAnimCamera(this, mCameraInfo, "DemoMeetTico");

        MR::registerDemoActionFunctor(this, MR::Functor(this, &RunawayTico::setDemoTrans), "チコとの出会い[開始]");
        MR::registerDemoActionNerve(this, &NrvRunawayTico::RunawayTicoNrvGuide1::sInstance, "チコとの出会い[チコ変身]");
        MR::registerDemoActionFunctor(this, MR::Functor(this, &RunawayTico::startRunaway), "ウサギ追いかけ[開始]");
        setNerve(&NrvRunawayTico::RunawayTicoNrvGuide0::sInstance);
    } else if (mObjArg1 == 2) {
        MR::registerDemoActionFunctor(this, MR::Functor(this, &RunawayTico::setPosAllCaught), "高楼出現[フェードイン]");
        makeActorDead();
    }

    MR::offRootNodeAutomatic(mMsgCtrl);
    MR::useStageSwitchWriteA(this, rIter);
    MR::invalidateClipping(this);
    MR::onCalcGravity(this);
}

void RunawayTico::initAfterPlacement() {
    if (mObjArg1 == 1 && MR::isOnGameEventFlagEndTicoGuideDemo()) {
        makeActorDead();
    }
}

void RunawayTico::appearBushComment(const TVec3f& rPos) {
    MR::forwardNodeNextBranchLeft(mMsgCtrl);
    appear();
    setPosAfterCaught(rPos);
    MR::requestStartDemoMarioPuppetable(this, "ぼやき", &NrvRunawayTico::RunawayTicoNrvAppear::sInstance,
                                        &NrvRunawayTico::RunawayTicoNrvWait::sInstance);
}

void RunawayTico::appearHoleComment(const TVec3f& rPos) {
    MR::forwardNodeNextBranchRight(mMsgCtrl);
    MR::forwardNodeCurrentBranchLeft(mMsgCtrl);
    appear();
    setPosAfterCaught(rPos);
    MR::requestStartDemoMarioPuppetable(this, "ぼやき", &NrvRunawayTico::RunawayTicoNrvAppear::sInstance,
                                        &NrvRunawayTico::RunawayTicoNrvWait::sInstance);
}

void RunawayTico::appearPipeComment(const TVec3f& rPos) {
    MR::forwardNodeNextBranchRight(mMsgCtrl);
    MR::forwardNodeCurrentBranchRight(mMsgCtrl);
    appear();
    setPosAfterCaught(rPos);
    MR::requestStartDemoMarioPuppetable(this, "ぼやき", &NrvRunawayTico::RunawayTicoNrvAppear::sInstance,
                                        &NrvRunawayTico::RunawayTicoNrvWait::sInstance);
}

void RunawayTico::appearMamaComment(const TVec3f& rPos) {
    mIsAllCaught = true;

    MR::forwardNode(mMsgCtrl);
    appear();
    setPosAfterCaught(rPos);
    MR::requestStartDemoMarioPuppetable(this, "ぼやき", &NrvRunawayTico::RunawayTicoNrvAppear::sInstance,
                                        &NrvRunawayTico::RunawayTicoNrvWait::sInstance);
}

void RunawayTico::setPosAfterCaught(const TVec3f& rPos) {
    TVec3f front;
    MR::getPlayerFrontVec(&front);

    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, -mGravity, -front, rPos);
    setBaseMtx(mtx);
    setInitPose();
}

void RunawayTico::setPosAllCaught() {
    MR::forwardNode(mMsgCtrl);
    MR::onRootNodeAutomatic(mMsgCtrl);

    char posName[265];
    snprintf(posName, sizeof(posName), "TicoDemoPos%d", mDemoCastID + 1);

    MR::setNPCActorPos(this, posName);
    setInitPose();

    if (MR::isDead(this)) {
        makeActorAppeared();
    }

    setNerveWait();
}

bool RunawayTico::isStartRunaway() const {
    return mIsStartRunaway;
}

void RunawayTico::startRunaway() {
    mIsStartRunaway = true;

    if (MR::isValidSwitchA(this)) {
        MR::onSwitchA(this);
    }
}

void RunawayTico::setDemoTrans() {
    MR::startAction(this, "DemoMeetTico");
    setBaseMtx(MR::getPlayerBaseMtx());
}

void RunawayTico::exeGuide0() {
    if (MR::isFirstStep(this)) {
        MR::startTimeKeepDemoMarioPuppetable(this, "チコガイドデモ", nullptr);
        MR::onGameEventFlagEndTicoGuideDemo();
        MR::endStartPosCamera();
        MR::startAnimCameraTargetPlayer(this, mCameraInfo, "DemoMeetTico", 0, 1.0f);
        MR::forceToFrameCinemaFrame();
    }

    if (MR::isGreaterEqualStep(this, ::sWaitSeStartFrame)) {
        MR::startLevelSound(this, "SE_SM_LV_TICO_WAIT");
    }

    if (MR::isGreaterEqualStep(this, ::sFloatSeStartFrame)) {
        MR::startLevelSound(this, "SE_SM_LV_TICO_FLOAT_DEMO", sFloatSeMinVolume * 100.0f);
    }

    if (MR::isStep(this, ::sDemoCameraZoomOutBgmStartFrame)) {
        MR::startSubBGM("BGM_MEET_TICO_ZOOM_OUT", false);
    }

    if (MR::isGreaterEqualStep(this, ::sDemoCameraZoomOutStartFrame) && MR::isLessStep(this, ::sDemoCameraZoomOutEndFrame)) {
        MR::startSystemLevelSE("SE_DM_LV_MEET_TICO_ZOOM_OUT");
    }
}

void RunawayTico::exeGuide1() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Metamorphosis");

        TPos3f mtx;
        mtx.set(MR::getJointMtx(this, "Body"));
        MR::addTransMtxLocalY(mtx.toMtxPtr(), ::sAwayYOffset);
        setBaseMtx(mtx);

        mLodCtrl->invalidate();
    }

    if (MR::isBckStopped(this)) {
        MR::hideModel(this);
    }

    if (MR::isDemoPartLastStep("チコとの出会い[ウサギ逃走]")) {
        MR::startStartPosCamera(true);
        MR::endAnimCamera(this, mCameraInfo, "DemoMeetTico", -1, true);
        MR::startStageBGM("MBGM_GALAXY_24", false);
        MR::showModel(this);
        mLodCtrl->validate();
        kill();
    }
}

void RunawayTico::exeWhiteOut() {
    if (MR::isFirstStep(this)) {
        MR::closeWipeWhiteFade(::sDemoFadeOut);
        MR::stopStageBGM(135);
    }

    MR::limitedSound("SE_SM_LV_TICO_WAIT", 1);

    if (MR::isLessEqualStep(this, ::sDemoFadeWait)) {
        MR::startLevelSound(this, "SE_SM_LV_TICO_OP_WAIT");
    }

    if (MR::isWipeActive()) {
        return;
    }

    setNerve(&NrvRunawayTico::RunawayTicoNrvWhiteIn::sInstance);
}

void RunawayTico::exeWhiteIn() {
    if (MR::isLessStep(this, ::sDemoFadeIn)) {
        MR::limitedSound("SE_SM_LV_TICO_WAIT", 1);
    }

    if (MR::isStep(this, ::sDemoFadeIn)) {
        MR::openWipeWhiteFade(::sDemoFadeOut);
        MR::endDemo(this, "ぼやき");
        MR::startTimeKeepDemoMarioPuppetable(this, "チコガイドデモ", "高楼出現[デモ]");
        kill();
    }
}

void RunawayTico::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }
}

void RunawayTico::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("WatchupMore", static_cast< const char* >(nullptr));
        MR::startAction(this, "Appear");
        MR::startSound(this, "SE_SM_RUNAWAY_RABBIT_APPEAR");
    }

    if (MR::isStep(this, ::sAppearSeStep)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvRunawayTico::RunawayTicoNrvTalk::sInstance);
    }
}

void RunawayTico::exeTalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Talk");
    }

    turnToPlayer();

    if (MR::tryTalkForceWithoutDemoAtEnd(mMsgCtrl)) {
        if (mIsAllCaught) {
            MR::startNPCTalkCamera(mMsgCtrl, getBaseMtx(), 1.0f, 0);
            setNerve(&NrvRunawayTico::RunawayTicoNrvWhiteOut::sInstance);
        } else {
            MR::endDemo(this, "ぼやき");
            setNerveWait();
        }
    }
}
