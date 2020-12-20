#pragma once

#include <revolution.h>

class TalkMessageInfo
{
public:
    TalkMessageInfo();

    bool isNormalTalk() const;
    bool isShortTalk() const;
    bool isEventTalk() const;
    bool isComposeTalk() const;
    bool isFlowTalk() const;
    bool isNullTalk() const;
    bool isBalloonCall() const;
    bool isBalloonFix() const;
    bool isBalloonSign() const;
    bool isBalloonInfo() const;
    bool isBalloonIcon() const;
    bool isCameraNormal() const;
    bool isCameraEvent() const;
};
