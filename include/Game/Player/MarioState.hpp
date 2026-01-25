#pragma once

#include "Game/Player/MarioModule.hpp"

class MarioState : public MarioModule {
public:
    MarioState(MarioActor*, u32);

    virtual void init();
    virtual bool proc(u32);
    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();
    virtual bool keep();
    virtual bool postureCtrl(MtxPtr);
    virtual void hitWall(const TVec3f&, HitSensor*);
    virtual void hitPoly(u8, const TVec3f&, HitSensor*);
    virtual bool passRing(const HitSensor*);
    virtual f32 getBlurOffset() const;
    virtual void draw3D() const;

    u32 getNoticedStatus() const;

    u32 _8;
    u32 mStatusId;  // 0xC
    u8 _10;
    u8 _11;
    u16 _12;
};
