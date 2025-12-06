#pragma once

#include "Game/MapObj/InvisiblePolygonObj.hpp"

class InvisiblePolygonObjGCapture : public InvisiblePolygonObj {
public:
    InvisiblePolygonObjGCapture(const char*);

    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
};