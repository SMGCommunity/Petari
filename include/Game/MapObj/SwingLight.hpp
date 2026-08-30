#pragma once

#include "Game/MapObj/MapObjActor.hpp"

struct Color8;

class LightCylinder : public LiveActor {
public:
    LightCylinder(const char*, MtxPtr, const Color8&);

    void initLightCylinderByOwner(const JMapInfoIter&);

    u8 mPad[(0xD0) - sizeof(LiveActor)];
};

class SwingLight : public MapObjActor {
public:
    SwingLight(const char*);
    virtual ~SwingLight();

private:
    u8 mPad[(0xC8) - sizeof(MapObjActor)];
};
