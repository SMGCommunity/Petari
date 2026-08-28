#pragma once

#include "Game/MapObj/InvisiblePolygonObj.hpp"

class InvisiblePolygonObjGCapture : public InvisiblePolygonObj {
public:
    InvisiblePolygonObjGCapture(const char*);

    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
};