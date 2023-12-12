#pragma once

#include "Game/AreaObj/AreaObj.h"

class ActorCameraInfo;
class JMapIdInfo;

class WarpCube : public AreaObj {
public:
    inline WarpCube(int type, const char *pName) : AreaObj(type, pName) {
        mMapIdInfo = NULL;
    }

    virtual ~WarpCube();

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual void draw() const;

    JMapIdInfo *mMapIdInfo;         // _3C
    ActorCameraInfo *mCameraInfo;   // _40
    char *mEventName;               // _44
    u16 _48;
    u8 _4A[2];
};

class WarpCubeMgr : public AreaObjMgr {
public:
    inline WarpCubeMgr(s32 type, const char *pName) : AreaObjMgr(type, pName) {
        _1C = 0;
    }

    virtual ~WarpCubeMgr() {
        
    }

    u32 _1C;
};