#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class ActorCameraInfo;
class JMapIdInfo;

class WarpCube : public AreaObj {
public:
    inline WarpCube(int type, const char* pName) : AreaObj(type, pName) { mMapIdInfo = nullptr; }

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
    inline WarpCubeMgr(s32 type, const char* pName) : AreaObjMgr(type, pName) { mWarpCube = 0; }

    virtual ~WarpCubeMgr() {}

    void setInvalidateTimer(AreaObj*, u16);
    void startEventCamera(const AreaObj*);
    void endEventCamera();

    const WarpCube* mWarpCube;  // 0x1C
};