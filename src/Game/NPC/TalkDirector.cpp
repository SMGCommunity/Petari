#include "Game/NPC/TalkDirector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TalkBalloon.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkState.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "revolution/types.h"
#include <cstdio>

namespace NrvTalkDirector {
    NEW_NERVE(TalkDirectorNrvWait, TalkDirector, Wait);
    NEW_NERVE(TalkDirectorNrvPrep, TalkDirector, Prep);
    NEW_NERVE(TalkDirectorNrvTalk, TalkDirector, Talk);
    NEW_NERVE(TalkDirectorNrvSlct, TalkDirector, Slct);
    NEW_NERVE(TalkDirectorNrvNext, TalkDirector, Next);
    NEW_NERVE(TalkDirectorNrvTerm, TalkDirector, Term);
}  // namespace NrvTalkDirector

TalkDirector::TalkDirector(const char* pArg)
    : LayoutActor(pArg, true), mMsgCtrl(nullptr), _3C(nullptr), _40(nullptr), _44(nullptr), mTalkState(nullptr), _4C(false), _4D(false), _4E(false),
      mIsInvalidClipping(false), mDemoType(0), _58(false), _59(false) {}

TalkDirector::~TalkDirector() {}

void TalkDirector::init(const JMapInfoIter& pArg) {
    MR::connectToScene(this, MR::MovementType_TalkDirector, -1, -1, -1);
    mBalloonHolder = new TalkBalloonHolder();
    mStateHolder = new TalkStateHolder();
    mMsgControls.init(0x80);
    mPeekZ = new TalkPeekZ();
    initNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance);
    initBranchResult();
    appear();
}
void TalkDirector::movement() {
    updateMessage();
    LayoutActor::movement();
}

bool TalkDirector::request(TalkMessageCtrl* pArg1, bool force) {
    _4D = false;
    bool var31 = false;
    _4C = false;
    if (isInvalidTalk()) {
        return false;
    }

    if (pArg1->mIsOnRootNodeAuto) {
        if (TalkFunction::isShortTalk(pArg1)) {
            if (!isNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance)) {
                pArg1->rootNodePre(false);
            }
        } else if (!isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance) && !isNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance)) {
            pArg1->rootNodePre(false);
        }
    }

    if (!force && !TalkFunction::isShortTalk(pArg1) && MR::isPlayerElementModeHopper()) {
        return false;
    }

    if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance) && !TalkFunction::isShortTalk(mTalkState->_04)) {
        if (mTalkState->_04 == pArg1) {
            var31 = true;
        } else {
            return false;
        }
    }

    if (MR::isTalkNone(pArg1)) {
        TalkFunction::onTalkStateEntry(pArg1);
    }

    if (force) {
        if (TalkFunction::isShortTalk(pArg1)) {
            if (pArg1->isNearPlayer(_40)) {
                _40 = pArg1;
            }
        } else {
            _40 = pArg1;
            _3C = pArg1;
            mMsgCtrl = pArg1;
        }
    } else {
        f32 scale = var31 ? 1.2f : 1.0f;
        f32 talkDistance = pArg1->mTalkDistance;

        if (!pArg1->isNearPlayer(scale * talkDistance)) {
            return false;
        }

        if (pArg1->isNearPlayer(_40)) {
            _40 = pArg1;
        }
    }

    if (mMsgCtrl != pArg1) {
        return false;
    }

    if (!isNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance)) {
        return mTalkState->_04 == pArg1;
    }

    mTalkState = initState(pArg1);
    setNerve(&NrvTalkDirector::TalkDirectorNrvPrep::sInstance);
    return true;
}

bool TalkDirector::test(TalkMessageCtrl* pArg1, bool arg2, bool arg3) {
    if (isInvalidTalk()) {
        return false;
    }

    if (mMsgCtrl != pArg1) {
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

    if (getDemoType(pArg1, arg3) == 1 && !MR::canStartDemo()) {
        return false;
    }

    return true;
}

bool TalkDirector::start(TalkMessageCtrl* pArg1, bool arg2, bool arg3, bool arg4) {
    if (test(pArg1, arg2, arg3)) {
        if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance)) {
            return true;
        }
    } else {
        return false;
    }

    pArg1->rootNodePre(true);
    prepTalk(pArg1, arg2, arg3, arg4);

    if (TalkFunction::isEventNode(pArg1)) {
        setNerve(&NrvTalkDirector::TalkDirectorNrvNext::sInstance);
    } else {
        mTalkState->open();
        setNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance);
    }

    return true;
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

void TalkDirector::appearYesNoSelector(const TalkMessageCtrl* pArg) const {
    const char* branchID = pArg->getBranchID();

    char buff[0x100];
    snprintf(buff, 0x100, "Select_%s_Yes", branchID);

    char buff2[0x100];
    snprintf(buff2, 0x100, "Select_%s_No", branchID);

    if (pArg->isSelectYesNo()) {
        MR::resetYesNoSelectorSE();
    } else {
        MR::setYesNoSelectorSE("SE_SY_TALK_FOCUS_ITEM", "SE_SY_TALK_SELECT_YES", "SE_SY_TALK_SELECT_YES");
    }
    MR::requestMovementOn((LayoutActor*)MR::getGameSceneLayoutHolder()->mYesNoLayout);

    MR::appearYesNoSelector(buff, buff2, nullptr);
}

void TalkDirector::updateMessage() {
    mBalloonHolder->update();
    mStateHolder->update();

    for (TalkMessageCtrl** i = mMsgControls.mArray.mArr; i != &mMsgControls[mMsgControls.mCount]; i = i + 1) {
        if (MR::isTalkEntry(*i)) {
            TalkFunction::onTalkStateNone(*i);
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
        // mPeekZ stuff to do
    }

    if (MR::isPowerStarGetDemoActive()) {
        mMsgCtrl = nullptr;
        _4E = true;
    }
}

void TalkDirector::prepTalk(TalkMessageCtrl* pArg1, bool arg2, bool arg3, bool arg4) {
    _58 = arg2;
    _4C = true;
    _59 = arg4;

    mIsInvalidClipping = MR::isInvalidClipping(pArg1->mHostActor);
    mDemoType = getDemoType(pArg1, arg3);

    if (mDemoType == 0) {
        _59 = false;
    }

    MR::invalidateClipping(pArg1->mHostActor);

    switch (mDemoType) {
    case 2:
        MR::pauseTimeKeepDemo(pArg1->mHostActor);
        break;
    case 1:
        // "Discussion"
        MR::tryStartDemoMarioPuppetable(pArg1->mHostActor, "会話");
        MR::requestMovementOn(this);
        mBalloonHolder->pauseOff();
        mStateHolder->pauseOff();
        break;
    }

    if (_59) {
        MR::startPlayerTalk(pArg1->mHostActor);
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

TalkState* TalkDirector::initState(TalkMessageCtrl* pArg) {
    TalkState* state = mStateHolder->getState(pArg);
    TalkBalloon* balloon = mBalloonHolder->getBalloon(pArg);
    state->init(pArg, balloon);

    TalkMessageInfo* info = TalkFunction::getMessageInfo(pArg);
    if (info->isCameraNormal() || info->isCameraEvent()) {
        info = TalkFunction::getMessageInfo(pArg);
        mMessageInfo = *info;
        mCameraInfo = pArg->mCameraInfo;
        mHostActor = pArg->mHostActor;
    }

    return state;
}

s32 TalkDirector::getDemoType(const TalkMessageCtrl* pArg, bool arg2) const {
    s32 demoType;
    if (TalkFunction::isShortTalk(pArg)) {
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

void TalkDirector::exePrep() {
    MR::isLessStep(this, 4);
    if (mTalkState->prep(mMsgCtrl)) {
        TalkFunction::onTalkStateEnableStart(mTalkState->_04);
        return;
    }

    TalkFunction::onTalkStateNone(mTalkState->_04);
    mTalkState = nullptr;
    setNerve(&NrvTalkDirector::TalkDirectorNrvWait::sInstance);
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

void TalkDirector::exeWait() {}

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
        MR::startSystemSE("SE_SM_TALKBLN_OPEN", -1, -1);
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

void MR::pauseOffTalkDirector() {
    ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->pauseOff();
}

void MR::balloonOffTalkDirector() {
    ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->balloonOff();
}

void MR::invalidateTalkDirector() {
    TalkDirector* director = (TalkDirector*)MR::getSceneObjHolder()->getObj(0x19);
    director->_4E = true;
}

void MR::setTalkDirectorDrawSyncToken() {
    TalkDirector* director = (TalkDirector*)MR::getSceneObjHolder()->getObj(0x19);
    if (director) {
        ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->mPeekZ->setDrawSyncToken();
    }
}

bool MR::isActiveTalkBalloonShort() {
    TalkDirector* director = (TalkDirector*)MR::getSceneObjHolder()->getObj(0x19);
    if (director) {
        return ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->mBalloonHolder->isActiveBalloonShort();
    }

    return false;
}

bool TalkFunction::requestTalkSystem(TalkMessageCtrl* pCtrl, bool force) {
    return ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->request(pCtrl, force);
}

void TalkFunction::startTalkSystem(TalkMessageCtrl* pCtrl, bool force, bool demo, bool notPuppetable) {
    ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->start(pCtrl, force, demo, notPuppetable);
}

void TalkFunction::endTalkSystem(TalkMessageCtrl* pCtrl) {
    MR::getSceneObjHolder()->getObj(0x19);
    MR::isTalkEnableEnd(pCtrl);
}

bool TalkFunction::isTalkSystemStart(const TalkMessageCtrl* pCtrl) {
    TalkDirector* director = (TalkDirector*)MR::getSceneObjHolder()->getObj(0x19);

    return director->mMsgCtrl == pCtrl && director->_4C;
}

bool TalkFunction::isTalkSystemEnd(const TalkMessageCtrl* pCtrl) {
    TalkDirector* director = (TalkDirector*)MR::getSceneObjHolder()->getObj(0x19);

    return director->mMsgCtrl == pCtrl && director->_4D;
}

bool TalkFunction::getBranchAstroGalaxyResult(u16 arg) {
    return ((TalkDirector*)MR::getSceneObjHolder()->getObj(0x19))->getBranchResult(arg);
}

void TalkFunction::registerTalkSystem(TalkMessageCtrl* pCtrl) {
    TalkDirector* director = (TalkDirector*)MR::getSceneObjHolder()->getObj(0x19);
    director->mMsgControls.push_back(pCtrl);
}
