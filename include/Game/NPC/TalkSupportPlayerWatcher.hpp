#pragma once

#include <revolution/types.h>

class TalkSupportPlayerWatcher {
public:
    /// @brief Creates a new `TalkSupportPlayerWatcher`.
    TalkSupportPlayerWatcher();

    void update();
    bool isEnableTalkPlayerStateNormal() const;
    bool isEnableTalkPlayerStateEvent() const;
    bool isEnableTalkPlayerSpeed() const NO_INLINE;

    /* 0x00 */ u32 mEnableTalkCounter;
    /* 0x04 */ u32 mDisableTalkCounter;
};
