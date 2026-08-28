#pragma once

#include "Game/MapObj/InvisiblePolygonObj.hpp"

class TransparentWall : public InvisiblePolygonObj {
public:
    TransparentWall(const char*);

    /* 0x08 */ virtual ~TransparentWall();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    s32 _BC;
    bool _C0;
};
