#include "Game/NPC/TalkState.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkBalloon.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkSupportPlayerWatcher.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"

TalkState::TalkState() :
    _04(nullptr),
    mBalloon(nullptr)
{

}

void TalkState::init(TalkMessageCtrl *pArg1, TalkBalloon *pArg2) {
    _04 = pArg1;
    mBalloon = pArg2;
    mMessageID = _04->getMessageID();
}

u32 TalkState::getPageCount() const {
    return 0;
}

void TalkState::balloonOff() const {
    MR::hideScreen((LayoutActor *)mBalloon);
}

bool TalkState::isLostMessage(const TalkMessageCtrl *pArg1) const {
    if (pArg1 == nullptr) {
        return true;
    }

    if (pArg1 != _04) {
        return true;  
    }

    return _04->getMessageID() != mMessageID;
};

bool TalkState::isSelfInterrupt(const TalkMessageCtrl *pArg1) const {
    if (pArg1 == nullptr) {
        return false;
    }

    if (pArg1 != _04) {
        return false;  
    }
    
    return _04->getMessageID() != mMessageID;
}


bool TalkStateShort::prep(const TalkMessageCtrl *pArg1) {
    return !TalkState::isLostMessage(pArg1);
}

bool TalkStateShort::talk(const TalkMessageCtrl *) {
    return false;
}

void TalkStateShort::open() {
    mBalloon->open(_04);
    LiveActor* actor = _04->mHostActor;
    TalkMessageInfo* info = TalkFunction::getMessageInfo(_04);
    MR::startTalkSound(info->_6, actor);
}

void TalkStateShort::clos() {
    mBalloon->close();
}


TalkStateEvent::TalkStateEvent() {
    _18 = nullptr;
    _14 = nullptr;
    _10 = nullptr;
    _1D = true;
}

bool TalkStateEvent::prep(const TalkMessageCtrl *pArg1) {
    return !TalkState::isLostMessage(pArg1);
}

bool TalkStateEvent::test() {
    return _18->isEnableTalkPlayerStateEvent();
}

void TalkStateEvent::open() {
    TalkMessageInfo* info = TalkFunction::getMessageInfo(_04);
    MR::startTalkSound(info->_6, _04->mHostActor);

    _1C = 1;
    mPageCount = 0;
    _10 = 0;

    if (!MR::isTimeKeepDemoActive() || !info->isCameraNormal()) {
        _04->startCamera(-1);
    }
    
    mBalloon->open(_04);
}

void TalkStateEvent::clos() {
    mBalloon->close();
}

bool TalkStateEvent::talk(const TalkMessageCtrl *pArg1) {
    if (MR::testCorePadButtonA(0)) {
        if (_10++ > 10) {
            for (int i = 0; i < 2; i++) {
                mBalloon->skipMessage();
            }
        }
    } else {
        _10 = 0;
    }

    if (!mBalloon->isTextAppearedAll()) {
        return false;
    }

    bool unknown_bool = false;

    if (_1C != nullptr) {
        _1C = MR::testCorePadButtonA(0);
    } else if (MR::testCorePadButtonA(0) && !MR::testCorePadTriggerA(0)) {
        _1C = true;
        unknown_bool = true;
    }

    if (TalkFunction::isSelectTalk(_04) && !mBalloon->hasNextPage()) {
        return true;
    }

    if (unknown_bool) {
        mPageCount++;
        return !mBalloon->turnPage();
    }
    
    return false;
}

bool TalkStateEvent::term(const TalkMessageCtrl *pArg1) {
    if (_14++ > 30) {
        _14 = 0;
        return true;
    } else {
        return false;
    }
}

u32 TalkStateEvent::getPageCount() const {
    return mPageCount;
}

bool TalkStateNormal::test() {
    if (!_18->isEnableTalkPlayerStateNormal() || !MR::testCorePadTriggerA(0)) {
        return false;
    }
    
    return TalkStateEvent::test();
}

void TalkStateNormal::clos() {
    _24->term();
    mBalloon->close();
}

bool TalkStateNormal::term(const TalkMessageCtrl *pArg1) {
    if (pArg1 != nullptr && pArg1 != _04) {
        return true;
    }

    if (pArg1 != nullptr && TalkFunction::isShortTalk(pArg1)) {
        return true;
    }

    return TalkStateEvent::term(pArg1);
}

#ifdef NON_MATCHING
// Stuck at 99% because assembly string labels don't match, even though the code *should* be correct.
bool TalkStateNormal::prep(const TalkMessageCtrl *pArg1) {
    if (TalkStateNormal::isLostMessage(pArg1)) {
        _24->term();
        return false;
    }

    TalkStateNormal::updateButton();

    if (!_18->isEnableTalkPlayerStateNormal()) {
        _24->term();
    } else if (!MR::testCorePadTriggerA(0)) {
        TalkMessageInfo *info = TalkFunction::getMessageInfo(_04);
        if (!_24->isOpen()) {
            if (info->isBalloonSign()) {
                _24->openWithRead();
            } else {
                _24->openWithTalk();
            }
            MR::startSystemSE("SE_SM_TALK_BUTTON_APPEAR", -1, -1);
        }
    } else {
        _24->term();
        MR::startSystemSE("SE_SY_TALK_START", -1, -1);
        MR::startCSSound("CS_CLICK_OPEN", nullptr, 0);
    }

    return true;
}
#endif


void TalkStateCompose::init(TalkMessageCtrl *pArg1, TalkBalloon *pArg2) {
    TalkState::init(pArg1, pArg2);
    mSecondBalloon->open(pArg1);
}

bool TalkStateCompose::test() {
    if (_04->isNearPlayer(_04->mTalkDistance)) {
        return TalkStateNormal::test();
    }

    return false;
}

void TalkStateCompose::open() {
    TalkStateEvent::open();
    mSecondBalloon->close();
}

bool TalkStateCompose::prep(const TalkMessageCtrl *pArg1) {
    bool unknown_bool = true;

    if (TalkState::isLostMessage(pArg1)) {
        unknown_bool = false;
    } else if (_04->isNearPlayer(_04->mTalkDistance)) {
        unknown_bool = TalkStateNormal::prep(pArg1);
    } else {
        _24->term();
    }

    if (!unknown_bool) {
        _24->term();
        mSecondBalloon->close();
    }

    return unknown_bool;
}

void TalkStateHolder::update() {
    _0->update();
}

void TalkStateHolder::pauseOff() {
    MR::requestMovementOn(_4);
}

u32 TalkStateHolder::getState(const TalkMessageCtrl *pArg1) {
    TalkMessageInfo *info = TalkFunction::getMessageInfo(pArg1);

    if (info->isNormalTalk()) {
        return mTalkNormal;
    }

    if (info->isShortTalk()) {
        return mTalkShort;
    }

    if (info->isEventTalk()) {
        return mTalkEvent;
    }

    if (info->isComposeTalk()) {
        return mTalkCompose;
    }

    return mTalkUnknown;
}
