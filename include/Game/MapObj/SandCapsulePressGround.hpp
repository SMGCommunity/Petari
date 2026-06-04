#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SandCapsulePressGround : public MapObjActor {
public:
    SandCapsulePressGround(const char*);
    virtual ~SandCapsulePressGround();

private:
    u8 mPad[(0x10C) - sizeof(MapObjActor)];
};
