#pragma once

#include "Game/MapObj/RailMoveObj.hpp"


class IceVolcanoUpDownPlane : public RailMoveObj {
public:
    IceVolcanoUpDownPlane(const char*);

    void move();
    bool tryCalcNearestPosToPlayer(TVec3f*) const;
    TVec3f* getSoundCalcPos();

    /* 0xD4 */ TVec3f mNearestPosToPlayer;

};
