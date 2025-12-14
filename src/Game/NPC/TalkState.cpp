#include "Game/NPC/TalkState.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/NPC/TalkBalloon.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageInfo.hpp"
#include "Game/NPC/TalkSupportPlayerWatcher.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"

TalkState::TalkState() : _04(nullptr), mBalloon(nullptr) {}

void TalkState::init(TalkMessageCtrl* pArg1, TalkBalloon* pBalloon) {
    _04 = pArg1;
    mBalloon = pBalloon;
    mMessageID = _04->getMessageID();
}

u32 TalkState::getPageCount() const {
    return 0;
}

void TalkState::balloonOff() const {
    MR::hideScreen(mBalloon);
}

bool TalkState::isLostMessage(const TalkMessageCtrl* pArg1) const {
    if (pArg1 == nullptr) {
        return true;
    }

    if (pArg1 != _04) {
        return true;
    }

    return _04->getMessageID() != mMessageID;
};

bool TalkState::isSelfInterrupt(const TalkMessageCtrl* pArg1) const {
    if (pArg1 == nullptr) {
        return false;
    }

    if (pArg1 != _04) {
        return false;
    }

    return _04->getMessageID() != mMessageID;
}

TalkStateShort::TalkStateShort() : TalkState() {}

bool TalkStateShort::prep(const TalkMessageCtrl* pArg1) {
    return !TalkState::isLostMessage(pArg1);
}

bool TalkStateShort::talk(const TalkMessageCtrl*) {
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

bool TalkStateEvent::prep(const TalkMessageCtrl* pArg1) {
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

bool TalkStateEvent::talk(const TalkMessageCtrl* pArg1) {
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

    bool unknownBool = false;

    if (_1C != nullptr) {
        _1C = MR::testCorePadButtonA(0);
    } else if (MR::testCorePadButtonA(0) && !MR::testCorePadTriggerA(0)) {
        _1C = true;
        unknownBool = true;
    }

    if (TalkFunction::isSelectTalk(_04) && !mBalloon->hasNextPage()) {
        return true;
    }

    if (unknownBool) {
        mPageCount++;
        return !mBalloon->turnPage();
    }

    return false;
}

bool TalkStateEvent::term(const TalkMessageCtrl* pArg1) {
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

TalkStateNormal::TalkStateNormal() : TalkStateEvent() {}

bool TalkStateNormal::test() {
    if (!_18->isEnableTalkPlayerStateNormal() || !MR::testCorePadTriggerA(0)) {
        return false;
    }

    return TalkStateEvent::test();
}

void TalkStateNormal::clos() {
    mAButton->term();
    mBalloon->close();
}

bool TalkStateNormal::term(const TalkMessageCtrl* pArg1) {
    if (pArg1 != nullptr && pArg1 != _04) {
        return true;
    }

    if (pArg1 != nullptr && TalkFunction::isShortTalk(pArg1)) {
        return true;
    }

    return TalkStateEvent::term(pArg1);
}

// Stuck at 99% because assembly string labels don't match, even though the code *should* be correct.
bool TalkStateNormal::prep(const TalkMessageCtrl* pArg1) {
    if (TalkStateNormal::isLostMessage(pArg1)) {
        mAButton->term();
        return false;
    }

    TalkStateNormal::updateButton();

    if (!_18->isEnableTalkPlayerStateNormal()) {
        mAButton->term();
    } else if (!MR::testCorePadTriggerA(0)) {
        TalkMessageInfo* info = TalkFunction::getMessageInfo(_04);
        if (!mAButton->isOpen()) {
            if (info->isBalloonSign()) {
                mAButton->openWithRead();
            } else {
                mAButton->openWithTalk();
            }
            MR::startSystemSE("SE_SM_TALK_BUTTON_APPEAR", -1, -1);
        }
    } else {
        mAButton->term();
        MR::startSystemSE("SE_SY_TALK_START", -1, -1);
        MR::startCSSound("CS_CLICK_OPEN", nullptr, 0);
    }

    return true;
}

void TalkStateNormal::updateButton() {
    TVec3f camZ(MR::getCamZdir());                   // 0x78
    TVec3f camY(MR::getCamYdir());                   // 0x6c
    TVec3f up;                                       // 0x60
    TVec3f centerPlayer(*MR::getPlayerCenterPos());  // 0x54
    MR::getPlayerUpVec(&up);

    f32 f1 = __fabs(camZ.dot(up));
    f32 f2 = camY.dot(up);
    f1 = f1 * f1;

    TVec3f up_but_bigger(up);  // 0x48 but should be 0x3c
    up_but_bigger.mult(1000.0f);

    MR::calcNormalizedScreenPosition(&up, up_but_bigger + centerPlayer);  // Second arg is 0x3c, but should be 0x48
    MR::calcNormalizedScreenPosition(&centerPlayer, centerPlayer);
    MR::normalize(up - centerPlayer, &up);  // First arg is 0x30
    TVec2f playerScreenPos;                 // 0x28
    MR::calcScreenPosition(&playerScreenPos, *MR::getPlayerCenterPos());

    // JGeometry::TVec2<float>::TVec2(const JGeometry::TVec2<float>&) shouldn't be called
    TVec2f v10(TVec2f(0.0f, -1.0f) * (40.0f * f1));
    playerScreenPos.x += v10.x;
    playerScreenPos.y += v10.y;

    // JGeometry::TVec2<float>::TVec2(const JGeometry::TVec2<float>&) shouldn't be called
    TVec2f v11(TVec2f(up.x, up.y) * (60.0f * f2));
    playerScreenPos.x += v11.x;
    playerScreenPos.y += v11.y;

    mAButton->setTrans(playerScreenPos);
}

TalkStateCompose::TalkStateCompose() : TalkStateNormal() {}

void TalkStateCompose::init(TalkMessageCtrl* pArg1, TalkBalloon* pArg2) {
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

bool TalkStateCompose::prep(const TalkMessageCtrl* pArg1) {
    bool unknownBool = true;

    if (TalkState::isLostMessage(pArg1)) {
        unknownBool = false;
    } else if (_04->isNearPlayer(_04->mTalkDistance)) {
        unknownBool = TalkStateNormal::prep(pArg1);
    } else {
        mAButton->term();
    }

    if (!unknownBool) {
        mAButton->term();
        mSecondBalloon->close();
    }

    return unknownBool;
}

TalkStateHolder::TalkStateHolder() : _00(new TalkSupportPlayerWatcher()) {
    mAButton = new IconAButton(true, false);
    mAButton->initWithoutIter();

    mBalloonShort = new TalkBalloonShort("会話吹き出し[合成会話]");
    mBalloonShort->initWithoutIter();
    mBalloonShort->initInterval();
    mBalloonShort->kill();

    mTalkShort = new TalkStateShort();

    mTalkNormal = new TalkStateNormal();
    mTalkNormal->mAButton = mAButton;
    mTalkNormal->_18 = _00;

    mTalkCompose = new TalkStateCompose();
    mTalkCompose->mAButton = mAButton;
    mTalkCompose->_18 = _00;
    mTalkCompose->mSecondBalloon = mBalloonShort;

    mTalkEvent = new TalkStateEvent();
    mTalkEvent->_18 = _00;

    mTalk = new TalkState();
}

void TalkStateHolder::update() {
    _00->update();
}

void TalkStateHolder::pauseOff() {
    MR::requestMovementOn(mAButton);
}

TalkState* TalkStateHolder::getState(const TalkMessageCtrl* pArg1) {
    TalkMessageInfo* info = TalkFunction::getMessageInfo(pArg1);

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

    return mTalk;
}
