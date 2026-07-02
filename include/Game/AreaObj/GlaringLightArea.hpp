#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudSoundObject;

class GlaringLightArea : public AreaObj {
public:
    GlaringLightArea(int, const char*);

    virtual ~GlaringLightArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    virtual const char* getManagerName() const {
        return "GlaringLightArea";
    }

    /* 0x3C */ AudSoundObject* mSoundObj;
    /* 0x40 */ TVec3f mPos;
};

class GlaringLightAreaMgr : public AreaObjMgr {
public:
    GlaringLightAreaMgr(s32, const char*);

    virtual ~GlaringLightAreaMgr();
};
