#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class LavaProminence : public LiveActor {
public:
    LavaProminence(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual void startClipped();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void moveOnRail();
    void setGravityAndMakeMtx();
    void updateEffectClipping(bool);

    void exeWaitSwitch();
    void exeWait();
    void exeSign();
    void exeMoveStartExtra();
    void exeMoveStart();
    void exeMoveLoop();
    void exeMoveEnd();
    void exeMoveEndExtra();

    /* 0x8C */ s32 _8C;
    /* 0x90 */ f32 _90;
    /* 0x94 */ f32 _94;
    /* 0x98 */ f32 _98;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ TVec3f _A8;
    /* 0xB4 */ f32 _B4;
    /* 0xB8 */ f32 _B8;
    /* 0xBC */ f32 _BC;
    /* 0xC0 */ f32 _C0;
    /* 0xC4 */ f32 _C4;
    /* 0xC8 */ f32 _C8;
    /* 0xCC */ TMtx34f _CC;
    /* 0xFC */ TMtx34f _FC;
    /* 0x12C */ TVec3f _12C;
    /* 0x138 */ s32 _138;
    /* 0x13C */ TMtx34f _13C;
    /* 0x16C */ f32 _16C;
    /* 0x170 */ f32 _170;
    /* 0x174 */ f32 _174;
    /* 0x178 */ f32 _178;
    /* 0x17C */ TVec3f _17C;
    /* 0x188 */ bool _188;
    /* 0x18C */ TMtx34f _18C;
};
