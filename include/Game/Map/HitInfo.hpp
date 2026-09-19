#pragma once

#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>

class CollisionParts;
class HitSensor;

class Triangle {
public:
    Triangle();

    void fillData(CollisionParts*, u32, HitSensor*);
    const char* getHostName() const;
    s32 getHostPlacementZoneID() const;
    bool isHostMoved() const;
    bool isValid() const;
    const TVec3f* getNormal(int) const;
    const TVec3f* getFaceNormal() const;
    const TVec3f* getEdgeNormal(int) const;
    const TVec3f* getPos(int) const;
    const TVec3f* calcAndGetNormal(int);
    const TVec3f* calcAndGetEdgeNormal(int);
    const TVec3f* calcAndGetPos(int);
    void calcForceMovePower(TVec3f*, const TVec3f&) const;
    JMapInfoIter getAttributes() const;
    TPos3f* getBaseMtx() const;
    TPos3f* getBaseInvMtx() const;
    TPos3f* getPrevBaseMtx() const;

    HitSensor* getSensor() const {
        return mSensor;
    }

    /* 0x00 */ CollisionParts* mParts;
    /* 0x04 */ u32 mIdx;
    /* 0x08 */ HitSensor* mSensor;
    /* 0x0C */ TVec3f mNormals[4];
    /* 0x3C */ TVec3f mPos[3];
};

class HitInfo {
public:
    HitInfo();

    bool isCollisionAtFace() const;
    bool isCollisionAtEdge() const;
    bool isCollisionAtCorner() const;

    /* 0x00 */ Triangle mParentTriangle;
    /* 0x60 */ f32 _60;
    /* 0x64 */ TVec3f mHitPos;
    /* 0x70 */ TVec3f _70;
    /* 0x7C */ TVec3f _7C;
    /* 0x88 */ u8 _88;
};
