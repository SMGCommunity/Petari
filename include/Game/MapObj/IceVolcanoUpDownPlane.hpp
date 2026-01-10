#pragma once

#include "Game/MapObj/RailMoveObj.hpp"

class IceVolcanoUpDownPlane : RailMoveObj {
    IceVolcanoUpDownPlane(const char*);

    void move();
    bool tryCalcNearestPosToPlayer(TVec3f*) const;
    TVec3f* getSoundCalcPos();

    TVec3f _D4;
};
