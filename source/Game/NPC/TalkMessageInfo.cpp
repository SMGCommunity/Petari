#include "Game/NPC/TalkMessageInfo.hpp"

TalkMessageInfo::TalkMessageInfo() {
    _0 = 0;
    _4 = 0;
    _6 = 0;
    mCameraType = 2;
    mTalkType = 0;
    _A = -1;
    _B = -1;
}

bool TalkMessageInfo::isNormalTalk() const {
    return !(mTalkType);
}

bool TalkMessageInfo::isShortTalk() const {
    return !(mTalkType - 1); 
}

bool TalkMessageInfo::isEventTalk() const {
    return !(mTalkType - 2); 
}

bool TalkMessageInfo::isComposeTalk() const {
    return !(mTalkType - 3);
}

bool TalkMessageInfo::isFlowTalk() const {
    return !(mTalkType - 4);
}

bool TalkMessageInfo::isNullTalk() const {
    return !(mTalkType - 5);
}

bool TalkMessageInfo::isBalloonCall() const {
    return !(mBalloonType - 2); 
}

bool TalkMessageInfo::isBalloonFix() const {
    return !(mBalloonType - 3);
}

bool TalkMessageInfo::isBalloonSign() const {
    return !(mBalloonType - 4);
}

bool TalkMessageInfo::isBalloonInfo() const {
    return !(mBalloonType - 5);
}

bool TalkMessageInfo::isBalloonIcon() const {
    return !(mBalloonType - 6);
}

bool TalkMessageInfo::isCameraNormal() const {
    return !(mCameraType);
}

bool TalkMessageInfo::isCameraEvent() const {
    return !(mCameraType - 1);
}

