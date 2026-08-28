#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class ActorCameraInfo;
class JMapIdInfo;

class WarpCube : public AreaObj {
public:
    WarpCube(int formType, const char* pName) : AreaObj(formType, pName), mMapIdInfo() {
    }

    /* 0x08 */ virtual ~WarpCube();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;

    JMapIdInfo* mMapIdInfo;        // 0x3C
    ActorCameraInfo* mCameraInfo;  // 0x40
    char* mEventName;              // 0x44
    u16 _48;
    u8 _4A[2];
};

class WarpCubeMgr : public AreaObjMgr {
public:
    WarpCubeMgr(s32 formType, const char* pName) : AreaObjMgr(formType, pName), mWarpCube() {
    }

    /* 0x08 */ virtual ~WarpCubeMgr() {
    }

    void setInvalidateTimer(AreaObj*, u16);
    void startEventCamera(const AreaObj*);
    void endEventCamera();

    const WarpCube* mWarpCube;  // 0x1C
};
