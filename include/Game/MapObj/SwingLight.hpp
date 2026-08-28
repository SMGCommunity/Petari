#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SwingLight : public MapObjActor {
public:
    SwingLight(const char*);
    /* 0x08 */ virtual ~SwingLight();

private:
    u8 mPad[(0xC8) - sizeof(MapObjActor)];
};
