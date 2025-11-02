#include "Game/NPC/TalkState.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"

TalkState::TalkState() {
    _04 = nullptr;
    _08 = nullptr;
}

void TalkState::init(TalkMessageCtrl *pArg1, TalkBalloon *pArg2) {
    _04 = pArg1;
    _08 = pArg2;
    mMessageID = _04->getMessageID();
}

u32 TalkState::getPageCount() const {
    return 0;
}

void TalkState::balloonOff() const {
    MR::hideScreen((LayoutActor *)_08);
}

bool TalkState::isLostMessage(const TalkMessageCtrl *pArg1) const {
    if (pArg1 == nullptr) {
        return true;
    } else {
        if (pArg1 != _04) {
            return true;  
        } else {
            return _04->getMessageID() != mMessageID;
        }
    }
};

bool TalkState::isSelfInterrupt(const TalkMessageCtrl *pArg1) const {
    if (pArg1 == nullptr) {
        return false;
    } else {
        if (pArg1 != _04) {
            return false;  
        } else {
            return _04->getMessageID() != mMessageID;
        }
    }
}


bool TalkStateShort::prep(const TalkMessageCtrl *pArg1) {
    return !TalkState::isLostMessage(pArg1);
}

bool TalkStateShort::talk(const TalkMessageCtrl *) {
    return false;
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

bool TalkStateEvent::term(const TalkMessageCtrl *pArg1) {
    if (_14++ > 0x1e) {
        _14 = 0;
        return true;
    } else {
        return false;
    }
}

u32 TalkStateEvent::getPageCount() const {
    return mPageCount;
}
