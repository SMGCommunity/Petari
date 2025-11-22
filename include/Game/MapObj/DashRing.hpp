#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class DashRing : public LiveActor {
public:
    DashRing(const char* pName) : LiveActor(pName) {}

    virtual ~DashRing();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void initCommon(const JMapInfoIter&, const char*);
    void calcAxis();
    void calcSubAxis();

    TVec3f mAxis;     // 0x8C
    TVec3f mSubAxis;  // 0x98
    u16 _A4;
    u16 _A6;
    s32 _A8;
    s32 _AC;
    f32 _B0;
    u8 _B4;
    u8 _B5;
    u8 _B6;
    u8 _B7;
    TVec3f _B8;
};