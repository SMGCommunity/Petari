#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class ActorCameraInfo;
class JMapIdInfo;

class WarpCube : public AreaObj {
public:
    WarpCube(int formType, const char* pName) : AreaObj(formType, pName), mMapIdInfo() {
    }

    virtual ~WarpCube();

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void draw() const;

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

    virtual ~WarpCubeMgr() {
    }

    void setInvalidateTimer(AreaObj*, u16);
    void startEventCamera(const AreaObj*);
    void endEventCamera();

    const WarpCube* mWarpCube;  // 0x1C
};
