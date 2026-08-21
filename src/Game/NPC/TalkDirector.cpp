#include "Game/NPC/TalkDirector.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TalkBalloon.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkState.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Screen/YesNoLayout.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>
#include <cstdio>
#include <revolution/gx/GXGet.h>

namespace {
    TalkDirector* getTalkDirector() {
        return MR::getSceneObj< TalkDirector >(SceneObj_TalkDirector);
    }

    static const f32 sTalkDistanceScale = 1.0f;
    static const f32 sNearScale = 1.2f;
};  // namespace

namespace NrvTalkDirector {
    NEW_NERVE(TalkDirectorNrvWait, TalkDirector, Wait);
    NEW_NERVE(TalkDirectorNrvPrep, TalkDirector, Prep);
    NEW_NERVE(TalkDirectorNrvTalk, TalkDirector, Talk);
    NEW_NERVE(TalkDirectorNrvSlct, TalkDirector, Slct);
    NEW_NERVE(TalkDirectorNrvNext, TalkDirector, Next);
    NEW_NERVE(TalkDirectorNrvTerm, TalkDirector, Term);
};  // namespace NrvTalkDirector

TalkPeekZ::TalkPeekZ() : mDrawSync(DrawSyncManager::sInstance->setCallback(4, 1, this)) {
}

void TalkPeekZ::setDrawSyncToken() {
    GXGetProjectionv(&_20);
    GXGetViewportv(&_3C);
    _8 = 0;
    DrawSyncManager::sInstance->pushBreakPoint();
    GXSetDrawSync(mDrawSync);
}

void TalkPeekZ::drawSyncCallback(u16 arg) {
    if (!MR::isInRange(mScreenPos.x, 0.0f, MR::getScreenWidth() - 1) ||
        !MR::isInRange(mScreenPos.y, 0.0f, JUTVideo::getManager()->getEfbHeight() - 1)) {
        return;
    }

    TVec2f pos;
    MR::convertScreenPosToFrameBufferPos(&pos, mScreenPos);

    GXPeekZ(pos.x, pos.y, &_8);

    TDDraw::invProject(&_14, TVec3f(mScreenPos.x, mScreenPos.y, static_cast< f32 >(_8)), const_cast< MtxPtr >(MR::getCameraViewMtx().mMtx), &_20,
                       &_3C, false);
}

TalkDirector::TalkDirector(const char* pName)
    : LayoutActor(pName, true), mMsgCtrl(), _3C(), _40(), _44(), mTalkState(), _4C(), _4D(), _4E(), mIsInvalidClipping(), mDemoType(), _58(), _59() {
}

TalkDirector::~TalkDirector() {
}

void TalkDirector::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_TalkDirector, -1, -1, -1);
    mBalloonHolder = new TalkBalloonHolder();
    mStateHolder = new TalkStateHolder();
    mMsgControls.init(128);
    mPeekZ = new TalkPeekZ();
    initNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance);
    initBranchResult();
    appear();
}
void TalkDirector::movement() {
    updateMessage();
    LayoutActor::movement();
}

bool TalkDirector::request(TalkMessageCtrl* pCtrl, bool force) {
    _4D = false;
    bool var31 = false;
    _4C = false;
    if (isInvalidTalk()) {
        return false;
    }

    if (pCtrl->mIsOnRootNodeAuto) {
        if (TalkFunction::isShortTalk(pCtrl)) {
            if (!isNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance)) {
                pCtrl->rootNodePre(false);
            }
        } else if (!isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance) && !isNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance)) {
            pCtrl->rootNodePre(false);
        }
    }

    if (!force && !TalkFunction::isShortTalk(pCtrl) && MR::isPlayerElementModeHopper()) {
        return false;
    }

    if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance) && !TalkFunction::isShortTalk(mTalkState->_04)) {
        if (mTalkState->_04 == pCtrl) {
            var31 = true;
        } else {
            return false;
        }
    }

    if (MR::isTalkNone(pCtrl)) {
        TalkFunction::onTalkStateEntry(pCtrl);
    }

    if (force) {
        if (TalkFunction::isShortTalk(pCtrl)) {
            if (pCtrl->isNearPlayer(_40)) {
                _40 = pCtrl;
            }
        } else {
            _40 = pCtrl;
            _3C = pCtrl;
            mMsgCtrl = pCtrl;
        }
    } else {
        f32 scale = var31 ? ::sNearScale : ::sTalkDistanceScale;
        f32 talkDistance = pCtrl->mTalkDistance;

        if (!pCtrl->isNearPlayer(scale * talkDistance)) {
            return false;
        }

        if (pCtrl->isNearPlayer(_40)) {
            _40 = pCtrl;
        }
    }

    if (mMsgCtrl != pCtrl) {
        return false;
    }

    if (!isNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance)) {
        return mTalkState->_04 == pCtrl;
    }

    mTalkState = initState(pCtrl);
    setNerve(&NrvTalkDirector::TalkDirectorNrvPrep::sInstance);
    return true;
}

bool TalkDirector::test(TalkMessageCtrl* pCtrl, bool arg2, bool arg3) {
    if (isInvalidTalk()) {
        return false;
    }

    if (mMsgCtrl != pCtrl) {
        return false;
    }

    if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance)) {
        return true;
    }

    if (!isNerve(&NrvTalkDirector::TalkDirectorNrvPrep::sInstance)) {
        return false;
    }

    if (!arg2 && !mTalkState->test()) {
        return false;
    }

    if (getDemoType(pCtrl, arg3) == 1 && !MR::canStartDemo()) {
        return false;
    }

    return true;
}

bool TalkDirector::start(TalkMessageCtrl* pCtrl, bool arg2, bool arg3, bool arg4) {
    if (test(pCtrl, arg2, arg3)) {
        if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance)) {
            return true;
        }
    } else {
        return false;
    }

    pCtrl->rootNodePre(true);
    prepTalk(pCtrl, arg2, arg3, arg4);

    if (TalkFunction::isEventNode(pCtrl)) {
        setNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance);
    } else {
        mTalkState->open();
        setNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance);
    }

    return true;
}

void TalkDirector::updateMessage() {
    mBalloonHolder->update();
    mStateHolder->update();

    for (TalkMessageCtrl** pIter = mMsgControls.begin(); pIter != mMsgControls.end(); pIter++) {
        if (MR::isTalkEntry(*pIter)) {
            TalkFunction::onTalkStateNone(*pIter);
        }
    }

    if (isNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance) || isNerve(&NrvTalkDirector::TalkDirectorNrvPrep::sInstance)) {
        mMsgCtrl = _3C;
        _3C = _40;
        _40 = nullptr;
    } else {
        mMsgCtrl = _40;
        _3C = mMsgCtrl;
        _40 = nullptr;
    }

    if (mMsgCtrl != nullptr) {
        _44 = mMsgCtrl;
    }

    if (_3C != nullptr) {
        _3C->updateBalloonPos();

        TalkPeekZ* peek = mPeekZ;
        TVec2f v1(_3C->_1C.x, _3C->_1C.y);
        peek->mScreenPos.set(v1.x, v1.y);
    }

    if (MR::isPowerStarGetDemoActive()) {
        mMsgCtrl = nullptr;
        _4E = true;
    }
}

void TalkDirector::prepTalk(TalkMessageCtrl* pCtrl, bool arg2, bool arg3, bool arg4) {
    _58 = arg2;
    _4C = true;
    _59 = arg4;

    mIsInvalidClipping = MR::isInvalidClipping(pCtrl->mHostActor);
    mDemoType = getDemoType(pCtrl, arg3);

    if (mDemoType == 0) {
        _59 = false;
    }

    MR::invalidateClipping(pCtrl->mHostActor);

    switch (mDemoType) {
    case 2:
        MR::pauseTimeKeepDemo(pCtrl->mHostActor);
        break;
    case 1:
        // "Discussion"
        MR::tryStartDemoMarioPuppetable(pCtrl->mHostActor, "会話");
        MR::requestMovementOn(this);
        mBalloonHolder->pauseOff();
        mStateHolder->pauseOff();
        break;
    }

    if (_59) {
        MR::startPlayerTalk(pCtrl->mHostActor);
    }
}

void TalkDirector::termTalk() {
    TalkMessageCtrl* control = mTalkState->_04;
    _4D = true;

    if (mMessageInfo.isCameraNormal()) {
        MR::endNPCTalkCamera(false, -1);
    } else if (mMessageInfo.isCameraEvent()) {
        MR::endMultiActorCamera(mHostActor, mCameraInfo, "会話", false, -1);
    }

    if (!mIsInvalidClipping) {
        MR::validateClipping(control->mHostActor);
    }

    if (_59) {
        MR::endPlayerTalk();
    }

    switch (mDemoType) {
    case 2:
        MR::resumeTimeKeepDemo(control->mHostActor);
        break;
    case 1:
        MR::endDemo(control->mHostActor, "会話");
        break;
    }

    mDemoType = 0;
    _59 = false;
    mIsInvalidClipping = false;
}

TalkState* TalkDirector::initState(TalkMessageCtrl* pCtrl) {
    TalkState* state = mStateHolder->getState(pCtrl);
    TalkBalloon* balloon = mBalloonHolder->getBalloon(pCtrl);
    state->init(pCtrl, balloon);

    TalkMessageInfo* info = TalkFunction::getMessageInfo(pCtrl);

    if (info->isCameraNormal() || info->isCameraEvent()) {
        info = TalkFunction::getMessageInfo(pCtrl);
        mMessageInfo = *info;
        mCameraInfo = pCtrl->mCameraInfo;
        mHostActor = pCtrl->mHostActor;
    }

    return state;
}

bool TalkDirector::isInvalidTalk() const {
    if (_4E) {
        return true;
    }

    if (MR::isStarPointerModeBlueStarReady()) {
        return true;
    }

    if (MR::isFirstPersonCamera()) {
        return true;
    }

    return MR::isPlayerDead();
}

void TalkDirector::appearYesNoSelector(const TalkMessageCtrl* pCtrl) const {
    const char* branchID = pCtrl->getBranchID();

    char buffYes[256];
    snprintf(buffYes, sizeof(buffYes), "Select_%s_Yes", branchID);

    char buffNo[256];
    snprintf(buffNo, sizeof(buffNo), "Select_%s_No", branchID);

    if (pCtrl->isSelectYesNo()) {
        MR::resetYesNoSelectorSE();
    } else {
        MR::setYesNoSelectorSE("SE_SY_TALK_FOCUS_ITEM", "SE_SY_TALK_SELECT_YES", "SE_SY_TALK_SELECT_YES");
    }

    MR::requestMovementOn(MR::getGameSceneLayoutHolder()->mYesNoLayout);
    MR::appearYesNoSelector(buffYes, buffNo, nullptr);
}

s32 TalkDirector::getDemoType(const TalkMessageCtrl* pCtrl, bool arg2) const {
    s32 demoType;

    if (TalkFunction::isShortTalk(pCtrl)) {
        demoType = 0;
    } else if (MR::isTimeKeepDemoActive()) {
        if (arg2) {
            demoType = 2;
        } else {
            demoType = 3;
        }
    } else {
        demoType = 1;
    }

    if (!arg2 && demoType != 3) {
        return 0;
    }

    return demoType;
}

// Gets the arg-th bool of TalkDirector's booleans, which start at 0x70
bool TalkDirector::getBranchResult(u16 arg) {
    if (arg == 18) {
        return MR::isAnyPlayerLeftSupply();
    }

    if (arg == 12) {
        return MR::isLuigiDisappearFromAstroGalaxy();
    }

    if (arg == 13) {
        return MR::isOnLuigiHiding();
    }

    return (reinterpret_cast< bool* >(this) + arg)[0x70];
}

void TalkDirector::initBranchResult() {
    mIsKinopioExplorerRescued = MR::isKinopioExplorerRescued();
    mIsKinopioExplorerOrganize = MR::isKinopioExplorerOrganize();
    mIsKinopioExplorerTalkGetGrandStar2 = MR::isKinopioExplorerTalkGetGrandStar2();
    mIsKinopioExplorerTrickComet = MR::isKinopioExplorerTrickComet();
    mIsKinopioExplorerTalkGetGrandStar3 = MR::isKinopioExplorerTalkGetGrandStar3();
    mIsKinopioExplorerStartMessenger = MR::isKinopioExplorerStartMessenger();
    mIsKinopioExplorerTalkGetGrandStar4 = MR::isKinopioExplorerTalkGetGrandStar4();
    mIsKinopioExplorerTalkGetGrandStar5 = MR::isKinopioExplorerTalkGetGrandStar5();
    mIsKinopioExplorerTalkGetGrandStar6 = MR::isKinopioExplorerTalkGetGrandStar6();
    mIsKinopioExplorerTalkGoFinalBattle = MR::isKinopioExplorerTalkGoFinalBattle();
    mIsEndLuigiHideAndSeek = MR::isEndLuigiHideAndSeekEvent();
    mIsKinopioExplorerCompleteTrickComet = MR::isKinopioExplorerCompleteTrickComet();
    mHasOneGreenStar = MR::calcCurrentGreenStarNum() == 1;
    mHasTwoGreenStars = MR::calcCurrentGreenStarNum() == 2;
    mHasThreeGreenStars = MR::calcCurrentGreenStarNum() == 3;
    mIsUnlockedRedDriver = MR::isOnGameEventFlagRedDriver();
    mIsActiveLuigiHideAndSeek = MR::isActiveLuigiHideAndSeekEvent();
    mIsGalaxyPurpleCometLaunch = MR::isGalaxyPurpleCometLaunch();
    misRosettaTalkTorchLecture = MR::isRosettaTalkTorchLecture();
    mIsRosettaTalkTrickComet = MR::isRosettaTalkTrickComet();
    mIsRosettaTalkKoopa = MR::isRosettaTalkKoopa();
    mIsRosettaTalkCountDownStart = MR::isRosettaTalkCountDownStart();
    mIsRosettaTalkAstroDemoRecover = MR::isRosettaTalkAstroDomeRecover();
    mIsRosettaTalkTorchProgress = MR::isRosettaTalkTorchProgress();
    mIsOnGameEventFlagViewNormalEnding = MR::isOnGameEventFlagViewNormalEnding();
}

void TalkDirector::pauseOff() {
    MR::requestMovementOn(this);
    mBalloonHolder->pauseOff();
    mStateHolder->pauseOff();
}

void TalkDirector::balloonOff() {
    if (mTalkState != nullptr) {
        mTalkState->balloonOff();
        mBalloonHolder->balloonOff();
    }
}

bool TalkDirector::isSystemTalking() const {
    if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance) || isNerve(&NrvTalkDirector::TalkDirectorNrvSlct::sInstance) ||
        isNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance)) {
        return !TalkFunction::isShortTalk(mTalkState->_04);
    }

    return false;
}

bool TalkDirector::isNormalTalking() const {
    return isSystemTalking() && mDemoType == 1;
}

LiveActor* TalkDirector::getTalkingActor() const {
    if (isSystemTalking()) {
        return mTalkState->_04->mHostActor;
    }

    return nullptr;
}

void TalkDirector::exePrep() {
    if (MR::isLessStep(this, 4)) {
    }

    if (mTalkState->prep(mMsgCtrl)) {
        TalkFunction::onTalkStateEnableStart(mTalkState->_04);
        return;
    }

    TalkFunction::onTalkStateNone(mTalkState->_04);
    mTalkState = nullptr;
    setNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance);
}

void TalkDirector::exeWait() {
}

void TalkDirector::exeTalk() {
    TalkMessageCtrl* control = mTalkState->_04;
    TalkFunction::onTalkStateTalking(control);

    bool cond = false;

    if (mDemoType == 0) {
        cond = mTalkState->isLostMessage(mMsgCtrl);
    }

    if (!cond) {
        if (!mTalkState->talk(mMsgCtrl)) {
            return;
        }

        MR::readMessage(control);
    }

    if (TalkFunction::isSelectTalk(control)) {
        setNerve(&NrvTalkDirector::TalkDirectorNrvSlct::sInstance);
        return;
    }

    mTalkState->clos();
    _50 = false;

    if (control->mIsOnRootNodeAuto && !mTalkState->isSelfInterrupt(_44)) {
        control->rootNodePst();
    }

    setNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance);
    exeNext();
}

void TalkDirector::exeSlct() {
    TalkMessageCtrl* control = mTalkState->_04;

    if (MR::isFirstStep(this)) {
        appearYesNoSelector(control);
        MR::startSystemSE("SE_SM_TALKBLN_OPEN");
    }

    TalkFunction::onTalkStateTalking(control);

    if (MR::isYesNoSelected()) {
        MR::resetYesNoSelectorSE();
        mTalkState->clos();
        _50 = true;

        if (control->mIsOnRootNodeAuto) {
            control->rootNodeSel(MR::isYesNoSelectedYes());
            control->rootNodePre(true);
        }

        setNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance);
        exeNext();
    }
}

void TalkDirector::exeNext() {
    TalkMessageCtrl* control = mTalkState->_04;

    bool cond = control->isCurrentNodeContinue();

    if (control->rootNodeEve()) {
        _50 |= cond;

        if (_50) {
            if (TalkFunction::isShortTalk(control)) {
                termTalk();
            }
            control->rootNodePre(true);
            mTalkState = initState(control);
            mTalkState->open();
            setNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance);

            return;
        }
        termTalk();
        TalkFunction::onTalkStateEnableEnd(control);
        setNerve(&NrvTalkDirector::TalkDirectorNrvTerm::sInstance);
    }
}

void TalkDirector::exeTerm() {
    if (!_58) {
        if (!mTalkState->term(mMsgCtrl)) {
            return;
        }
    }

    TalkFunction::onTalkStateNone(mTalkState->_04);
    mTalkState = nullptr;

    setNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance);
}

bool TalkFunction::requestTalkSystem(TalkMessageCtrl* pCtrl, bool force) {
    return ::getTalkDirector()->request(pCtrl, force);
}

bool TalkFunction::startTalkSystem(TalkMessageCtrl* pCtrl, bool force, bool demo, bool notPuppetable) {
    return ::getTalkDirector()->start(pCtrl, force, demo, notPuppetable);
}

bool TalkFunction::endTalkSystem(TalkMessageCtrl* pCtrl) {
    ::getTalkDirector();
    return MR::isTalkEnableEnd(pCtrl);
}

bool TalkFunction::isTalkSystemStart(const TalkMessageCtrl* pCtrl) {
    TalkDirector* pDirector = ::getTalkDirector();

    return pDirector->mMsgCtrl == pCtrl && pDirector->_4C;
}

bool TalkFunction::isTalkSystemEnd(const TalkMessageCtrl* pCtrl) {
    TalkDirector* pDirector = ::getTalkDirector();

    return pDirector->mMsgCtrl == pCtrl && pDirector->_4D;
}

bool TalkFunction::getBranchAstroGalaxyResult(u16 arg) {
    return ::getTalkDirector()->getBranchResult(arg);
}

void TalkFunction::registerTalkSystem(TalkMessageCtrl* pCtrl) {
    ::getTalkDirector()->mMsgControls.push_back(pCtrl);
}

void MR::pauseOffTalkDirector() {
    ::getTalkDirector()->pauseOff();
}

void MR::balloonOffTalkDirector() {
    ::getTalkDirector()->balloonOff();
}

void MR::invalidateTalkDirector() {
    ::getTalkDirector()->_4E = true;
}

void MR::setTalkDirectorDrawSyncToken() {
    if (::getTalkDirector() != nullptr) {
        ::getTalkDirector()->mPeekZ->setDrawSyncToken();
    }
}

bool MR::isActiveTalkBalloonShort() {
    if (::getTalkDirector() != nullptr) {
        return ::getTalkDirector()->mBalloonHolder->isActiveBalloonShort();
    }

    return false;
}
