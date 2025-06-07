#pragma once

class GameSystemFrameControl {
public:
    /// @brief Creates a new `GameSystemFrameControl`.
    GameSystemFrameControl();

    void setMovement60fps();

private:
    /* 0x0 */ bool mIs60fps;
};
