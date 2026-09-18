#pragma once

class GameSystemFrameControl {
public:
    /// @brief Creates a new `GameSystemFrameControl`.
    GameSystemFrameControl();

    void setMovement60fps();

public:
    /* 0x0 */ bool mIs60fps;
};
