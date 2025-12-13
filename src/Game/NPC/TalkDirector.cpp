#include "Game/NPC/TalkDirector.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/TalkBalloon.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkState.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include "revolution/types.h"

namespace NrvTalkDirector {
    NEW_NERVE(TalkDirectorNrvWait, TalkDirector, Wait);
    NEW_NERVE(TalkDirectorNrvPrep, TalkDirector, Prep);
    NEW_NERVE(TalkDirectorNrvTalk, TalkDirector, Talk);
    NEW_NERVE(TalkDirectorNrvSlct, TalkDirector, Slct);
    NEW_NERVE(TalkDirectorNrvNext, TalkDirector, Next);
    NEW_NERVE(TalkDirectorNrvTerm, TalkDirector, Term);
}  // namespace NrvTalkDirector

TalkDirector::TalkDirector(const char* pArg)
    : LayoutActor(pArg, true), _28(0), mMsgCtrl(nullptr), _3C(0), _40(0), _44(false), mTalkState(nullptr), _4C(false), _4D(false), _4E(false),
      mIsInvalidClipping(false), mDemoType(0), _58(false), _59(false) {}

void TalkDirector::init(const JMapInfoIter& pArg) {
    MR::connectToScene(this, 10, -1, -1, -1);
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

bool TalkDirector::request(TalkMessageCtrl* pArg1, bool arg2) {
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

    if (!arg2 && !TalkFunction::isShortTalk(pArg1) && MR::isPlayerElementModeHopper()) {
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

    if (arg2) {
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

        if (!pArg1->isNearPlayer(pArg1->mTalkDistance * scale)) {
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

    // 1 is probably an enum
    if (getDemoType(pArg1, arg3) == 1 && !MR::canStartDemo()) {
        return false;
    }

    return true;
}

bool TalkDirector::start(TalkMessageCtrl* pArg1, bool arg2, bool arg3, bool arg4) {
    if (!test(pArg1, arg2, arg3)) {
        return false;
    }

    if (isNerve(&NrvTalkDirector::TalkDirectorNrvTalk::sInstance)) {
        return true;
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

void TalkDirector::updateMessage() {
    mBalloonHolder->update();
    mStateHolder->update();

    for (TalkMessageCtrl** i = mMsgControls.mArr; i != &mMsgControls.mArr[_28]; i = i + 1) {
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

TalkState* TalkDirector::initState(TalkMessageCtrl* pArg) {
    u32 state = mStateHolder->getState(pArg);
    TalkBalloon* balloon = mBalloonHolder->getBalloon(pArg);
    balloon->movement();
    TalkMessageInfo* info = TalkFunction::getMessageInfo(pArg);
    if (info->isCameraNormal()) {
        if (info->isCameraEvent()) {
        }
    }

    info = TalkFunction::getMessageInfo(pArg);
    // mMessageInfo = info->_0;
    // return state;
}
