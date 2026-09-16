#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class FloaterFloatingForceTypeSpring;
class WaveFloatingForce;

class OceanWaveFloater : public MapObjActor {
public:
    OceanWaveFloater(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void initAfterPlacement();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    f32 getCurrentSinkDepth() const;
    void controlEffect();

    /* 0xC4 */ FloaterFloatingForceTypeSpring* mSpring;
    /* 0xC8 */ WaveFloatingForce* mForce;
    /* 0xCC */ TVec3f mRipplePos;
    /* 0xD8 */ s32 mSoundDelay;
    /* 0xDC */ TVec3f mMapGroundUpper;
    /* 0xE8 */ f32 mOffset;
    /* 0xEC */ bool mCanRipple;
};
