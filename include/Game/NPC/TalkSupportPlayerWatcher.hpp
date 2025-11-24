#pragma once

#include <revolution.h>

class TalkSupportPlayerWatcher {
public:
    TalkSupportPlayerWatcher();

    void update();
    bool isEnableTalkPlayerStateNormal() const;
    bool isEnableTalkPlayerStateEvent() const;
    bool isEnableTalkPlayerSpeed() const NO_INLINE;

    u32 _00;  // Counts consecutive frames where Mario has a velocity < 4.0f
    u32 _04;  // Counts consecutive frames where Mario has a velocity >= 4.0f
};
