#pragma once

#include <revolution.h>

class GameSystemFrameControl {
public:
    GameSystemFrameControl();

    void setMovement60fps();

    bool mIs60Fps;              //  0x00
};
