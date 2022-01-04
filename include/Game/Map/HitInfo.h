#pragma once

#include "Game/Util/JMapInfo.h"
#include "JSystem/JGeometry/TVec.h"

class CollisionParts;
class HitSensor;

class Triangle {
public:
    Triangle();

    Triangle &operator=(const Triangle &);

    void fillData(CollisionParts *, unsigned long, HitSensor *);
    const char *getHostName() const;
    s32 getHostPlacementZoneID() const;
    bool isHostMoved() const;
    bool isValid() const;
    const TVec3f *getNormal(int) const;
    const TVec3f *getFaceNormal() const;
    const TVec3f *getEdgeNormal(int) const;
    const TVec3f *getPos(int) const;
    const TVec3f *calcAndGetNormal(int);
    const TVec3f *calcAndGetEdgeNormal(int);
    const TVec3f *calcAndGetPos(int);
    void calcForceMovePower(TVec3f *, const TVec3f &) const;
    JMapInfoIter getAttributes() const;
    TPos3f *getBaseMtx() const;
    TPos3f *getBaseInvMtx() const;
    TPos3f *getPrevBaseMtx() const;

    CollisionParts* mParts; // _0
    u32 mIdx;               // _4
    HitSensor* mSensor;     // _8
    TVec3f mNormals[4];     // _C
    TVec3f mPos[3];         // _3C
};

class HitInfo {
public:
    Triangle mParentTriangle;   // _0
    f32 _60;
    TVec3f _64;
    TVec3f _70;
    TVec3f _7C;
    u8 _88;
    u8 _89;
    u8 _8A;
    u8 _8B;
};