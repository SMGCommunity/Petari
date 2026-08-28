#pragma once

#include "Game/NameObj/NameObj.hpp"

class AudCameraWatcher : public NameObj {
public:
    AudCameraWatcher();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();

    void playAtmosphereSE();

    u8 _C;
};
