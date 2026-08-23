#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry.hpp>

typedef void (*PosAttrFunc)(f32, f32, f32);
typedef void (*TexAttrFunc)(f32, f32);

class ElectricRailMovingPoint : public LiveActor {
public:
    ElectricRailMovingPoint(const char* pName = "移動電撃レール点");

    virtual ~ElectricRailMovingPoint();
    virtual void init(const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);

    u8 _8C;
};

class ElectricRailMoving : public LiveActor {
public:
    ElectricRailMoving(const char*);

    virtual ~ElectricRailMoving();
    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void attackSensor(HitSensor*, HitSensor*);

    void disappear();
    bool isTouchRail(const HitSensor*, TVec3f*, TVec3f*) const;
    void initMapToolInfo(const JMapInfoIter&);
    void initSensor();
    void initRail(const JMapInfoIter&);
    void initPoints();
    void initDisplayList();
    void drawRailGX(f32) const;
    void drawPlane(f32, f32, f32, f32) const;
    void drawPlaneGX(f32, f32, f32, f32) const;
    void setVertexAttribute(int, int, f32, f32, f32, f32, f32, PosAttrFunc, TexAttrFunc);
    void updateHitSensorPos();
    void updatePointPos();
    void updatePointPosAndModel();
    bool isValidCoord(f32) const;
    f32 getRepeatedCoord(f32) const;
    void calcPointPos(TVec3f*, f32) const;
    void move();
    void exeDisppear();
    void exeDisappeared();
    void exeWait();

    /* 0x8C */ s32 mSegmentNum;
    /* 0x90 */ f32 mMovementSpeed;
    /* 0x94 */ f32 mSegmentLength;
    /* 0x98 */ s32 mStackHeight;
    TPos3f _9C;
    /* 0xD0 */ TVec3f* mSensorOffsets;
    /* 0xD4 */ ElectricRailMovingPoint* mMovingPoints;
    u32 _D8;
    u32 _DC;
    u32 _E0;
    u32 _E4;
    TVec3f* _E8;
    f32 _EC;
    f32 _F0;
};
