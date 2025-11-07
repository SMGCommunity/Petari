#pragma once

#include <revolution.h>

class TalkSupportPlayerWatcher {
public:
    TalkSupportPlayerWatcher();

    void update();
    bool isEnableTalkPlayerStateNormal() const;
    bool isEnableTalkPlayerStateEvent() const;
    bool isEnableTalkPlayerStateSpeed() const;

    u32 _04;
    u32 _08;
};
