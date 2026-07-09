#pragma once

#include "Game/Player/MarioModule.hpp"

enum MarioStateMsg {
    MarioStateMsg_Start,
    MarioStateMsg_Close,
    MarioStateMsg_Update,
    MarioStateMsg_Notice,
    MarioStateMsg_Keep,
};

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

    /* 0x08 */ MarioState* _8;
    /* 0x0C */ u32 mStatusId;
    /* 0x10 */ bool _10;
};
