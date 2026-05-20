#pragma once

#include "Game/NameObj/NameObj.hpp"

class AudCameraWatcher : public NameObj {
public:
    AudCameraWatcher();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();

    void playAtmosphereSE();

    u8 _C;
};
