#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class SwingLight : public MapObjActor {
public:
    SwingLight(const char*);
    virtual ~SwingLight();

private:
    u8 mPad[(0xC8) - sizeof(MapObjActor)];
};
