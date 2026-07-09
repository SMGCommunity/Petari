#pragma once

#include "Game/Player/MarioModule.hpp"

enum MarioStatus {
    MarioStatus_None,
    MarioStatus_Wall,
    MarioStatus_Damage,
    MarioStatus_Faint,
    MarioStatus_Blown,
    MarioStatus_Hang,
    MarioStatus_Swim,
    MarioStatus_Slider,
    MarioStatus_FireDamage,
    MarioStatus_FireDance,
    MarioStatus_FireRun,
    MarioStatus_Paralyze,
    MarioStatus_AbyssDamage,
    MarioStatus_Freeze,
    MarioStatus_Stun,
    MarioStatus_Crush,
    MarioStatus_Step,
    MarioStatus_Magic,
    MarioStatus_FpView,
    MarioStatus_13,
    MarioStatus_Recovery = MarioStatus_13,
    MarioStatus_Warp = MarioStatus_13,
    MarioStatus_Flip,
    MarioStatus_SideStep,
    MarioStatus_Stick,
    MarioStatus_Rabbit,
    MarioStatus_Foo,
    MarioStatus_Flow,
    MarioStatus_Sukekiyo,
    MarioStatus_Bury,
    MarioStatus_Wait,
    MarioStatus_Climb,
    MarioStatus_Bump,
    MarioStatus_Skate,
    MarioStatus_FrontStep,
    MarioStatus_Teresa,
    MarioStatus_Talk,
    MarioStatus_DarkDamage,
};

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
