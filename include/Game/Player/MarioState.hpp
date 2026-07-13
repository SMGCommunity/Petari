#pragma once

#include "Game/Player/MarioModule.hpp"

enum MarioStatus {
    /* 0x00 */ MarioStatus_None,
    /* 0x01 */ MarioStatus_Wall,
    /* 0x02 */ MarioStatus_Damage,
    /* 0x03 */ MarioStatus_Faint,
    /* 0x04 */ MarioStatus_Blown,
    /* 0x05 */ MarioStatus_Hang,
    /* 0x06 */ MarioStatus_Swim,
    /* 0x07 */ MarioStatus_Slider,
    /* 0x08 */ MarioStatus_FireDamage,
    /* 0x09 */ MarioStatus_FireDance,
    /* 0x0A */ MarioStatus_FireRun,
    /* 0x0B */ MarioStatus_Paralyze,
    /* 0x0C */ MarioStatus_AbyssDamage,
    /* 0x0D */ MarioStatus_Freeze,
    /* 0x0E */ MarioStatus_Stun,
    /* 0x0F */ MarioStatus_Crush,
    /* 0x10 */ MarioStatus_Step,
    /* 0x11 */ MarioStatus_Magic,
    /* 0x12 */ MarioStatus_FpView,
    /* 0x13 */ MarioStatus_13,
    /* 0x13 */ MarioStatus_Recovery = MarioStatus_13,
    /* 0x13 */ MarioStatus_Warp = MarioStatus_13,
    /* 0x14 */ MarioStatus_Flip,
    /* 0x15 */ MarioStatus_SideStep,
    /* 0x16 */ MarioStatus_Stick,
    /* 0x17 */ MarioStatus_Rabbit,
    /* 0x18 */ MarioStatus_Foo,
    /* 0x19 */ MarioStatus_Flow,
    /* 0x1A */ MarioStatus_Sukekiyo,
    /* 0x1B */ MarioStatus_Bury,
    /* 0x1C */ MarioStatus_Wait,
    /* 0x1D */ MarioStatus_Climb,
    /* 0x1E */ MarioStatus_Bump,
    /* 0x1F */ MarioStatus_Skate,
    /* 0x20 */ MarioStatus_FrontStep,
    /* 0x21 */ MarioStatus_Teresa,
    /* 0x22 */ MarioStatus_Talk,
    /* 0x23 */ MarioStatus_DarkDamage,
};

enum MarioStateMsg {
    /* 0x0 */ MarioStateMsg_Start,
    /* 0x1 */ MarioStateMsg_Close,
    /* 0x2 */ MarioStateMsg_Update,
    /* 0x3 */ MarioStateMsg_Notice,
    /* 0x4 */ MarioStateMsg_Keep,
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
