#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Color8;

class AstroDomeOrbit : public LiveActor {
public:
    AstroDomeOrbit();

    virtual ~AstroDomeOrbit();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;

    void drawBloom() const;
    void setup(s32);
    void moveCoord();
    void calcGalaxyPos(TVec3f*) const;
    void initDraw(const Color8&) const;
    void drawCelling(f32, bool, f32) const;
    void drawSide(f32, bool, f32) const;
    f32 calcRepeatedRotateCoord(f32) const;

    /* 0x8C */ f32 mOrbitRadius;
    /* 0x90 */ f32 mAngle;
};
