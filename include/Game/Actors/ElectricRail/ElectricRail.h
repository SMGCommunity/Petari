#ifndef ELECTRICRAIL_H
#define ELECTRICRAIL_H

#include "Actor/LiveActor/LiveActor.h"
#include "Actor/Nerve/Nerve.h"

class ElectricRailPoint;
class ElectricRailSeparator;
class ElectricRailShadowDrawer;

class ElectricRail : public LiveActor
{
public:
    ElectricRail(const char *);

    virtual ~ElectricRail();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void draw() const;
    virtual void makeActorAppeared();
    virtual void makeActorDead();
    virtual void attackSensor(HitSensor *, HitSensor *);

    void disappear();
    void initMapToolInfo(const JMapInfoIter &);
    void initSensor();
    void initPoints();
    void initSeparators();
    void initDisplayList();
    void drawRailGX(f32) const;
    void drawPlane(f32, f32, f32, f32) const;
    void drawPlaneGX(f32, f32, f32, f32) const;
    void initShadow(const JMapInfoIter &);
    void updateHitSensorPos();
    void calcGravity(JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &) const;
    void exeDisappear();

    ElectricRailPoint* mPoints; // _8C
    s32 mPointCount; // _90
    JGeometry::TVec3<f32>* _94;
    ElectricRailSeparator* mSeparators; // _98
    s32 mSeparatorCount; // _9C
    void* _A0;
    u32 mDLLength; // _A4
    s32 mRailHeight; // _A8
    f32 mEaseIn; // _AC
    ElectricRailShadowDrawer* mShadowDrawer; // _B0
    bool _B4; // related to gravity
    u8 _B5;
    u8 _B6;
    u8 _B7;

    static const char *cSensorNameTable[8];
};

namespace NrvElectricRail
{
    class ElectricRailNrvWait : public Nerve
    {
    public:
        ElectricRailNrvWait();

        virtual void execute(Spine *) const;

        static ElectricRailNrvWait* sInstance;
    };

    class ElectricRailNrvDisappear : public Nerve
    {
    public:
        ElectricRailNrvDisappear();

        virtual void execute(Spine *) const;

        static ElectricRailNrvDisappear* sInstance;
    };

    class ElectricRailNrvDisappeared : public Nerve
    {
    public:
        ElectricRailNrvDisappeared();

        virtual void execute(Spine *) const;

        static ElectricRailNrvDisappeared* sInstance;
    };
};

class ElectricRailPoint : public LiveActor
{
public:
    ElectricRailPoint(const char *name = "電撃レール点");

    virtual ~ElectricRailPoint();
    virtual void init(const JMapInfoIter &);
    virtual void attackSensor(HitSensor *, HitSensor *);

    u8 _8C;
    u8 _8D;
};

class ElectricRailSeparator : public LiveActor
{
public:
    ElectricRailSeparator();

    void setup(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);

    JGeometry::TVec3<f32> _0;
    JGeometry::TVec3<f32> _C;
    JGeometry::TVec3<f32> _18;
    JGeometry::TVec3<f32> _24;
    JGeometry::TVec3<f32> _30;
};

class ElectricRailShadowDrawer
{
public:
    ElectricRailShadowDrawer(const LiveActor *, ElectricRailSeparator *, s32);
};

#endif // ELECTRICRAIL_H