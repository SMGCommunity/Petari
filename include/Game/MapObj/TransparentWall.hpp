#pragma once

#include "Game/MapObj/InvisiblePolygonObj.hpp"

class TransparentWall : public InvisiblePolygonObj {
public:
    TransparentWall(const char*);

    virtual ~TransparentWall();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    s32 _BC;
    bool _C0;
};
