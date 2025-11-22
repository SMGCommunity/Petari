#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudSoundObject;

class GlaringLightArea : public AreaObj {
public:
    GlaringLightArea(int, const char*);
    virtual ~GlaringLightArea();

    virtual void init(const JMapInfoIter&);
    virtual void movement();

    virtual const char* getManagerName() const;

    AudSoundObject* mSound;  // 0x3C
    TVec3f mPos;             // 0x40
};

class GlaringLightAreaMgr : public AreaObjMgr {
public:
    GlaringLightAreaMgr(s32, const char*);

    virtual ~GlaringLightAreaMgr();
};