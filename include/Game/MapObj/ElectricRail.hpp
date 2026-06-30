#pragma once

#include "Game/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class ElectricRailPoint;
class ElectricRailSeparator;
class ElectricRailShadowDrawer;

class ElectricRail : public LiveActor {
public:
    ElectricRail(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void draw() const;
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void attackSensor(HitSensor*, HitSensor*);

    void disappear();
    void initMapToolInfo(const JMapInfoIter&);
    void initSensor();
    void initPoints();
    void initSeparators();
    void initDisplayList();
    void drawRailGX(f32) const;
    void drawPlane(f32, f32, f32, f32) const;
    void drawPlaneGX(f32, f32, f32, f32) const;
    void initShadow(const JMapInfoIter&);
    void updateHitSensorPos();
    void calcGravity(TVec3f*, const TVec3f&) const;
    void exeDisappear();
    void exeWait();
    void exeDisappeared();

    ElectricRailPoint* mPoints;  // _8C
    s32 mPointCount;             // _90
    TVec3f* _94;
    ElectricRailSeparator* mSeparators;  // _98
    s32 mSeparatorCount;                 // _9C
    void* _A0;
    u32 mDLLength;                            // _A4
    s32 mRailHeight;                          // _A8
    f32 mEaseIn;                              // _AC
    ElectricRailShadowDrawer* mShadowDrawer;  // _B0
    bool _B4;                                 // related to gravity
    u8 _B5;
    u8 _B6;
    u8 _B7;

    static const char* cSensorNameTable[8];
};

class ElectricRailPoint : public LiveActor {
public:
    ElectricRailPoint(const char* name = "電撃レール点");

    virtual ~ElectricRailPoint();
    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);

    u8 _8C;
    u8 _8D;
};

class ElectricRailSeparator : public LiveActor {
public:
    ElectricRailSeparator();

    void setup(const TVec3f&, const TVec3f&, const TVec3f&);

    TVec3f _0;
    TVec3f _C;
    TVec3f _18;
    TVec3f _24;
    TVec3f _30;
};

class ElectricRailShadowDrawer {
public:
    ElectricRailShadowDrawer(const LiveActor*, ElectricRailSeparator*, s32);
};
