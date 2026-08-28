#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AudSoundObject;

class GlaringLightArea : public AreaObj {
public:
    GlaringLightArea(int, const char*);

    /* 0x08 */ virtual ~GlaringLightArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x28 */ virtual const char* getManagerName() const {
        return "GlaringLightArea";
    }

    /* 0x3C */ AudSoundObject* mSoundObj;
    /* 0x40 */ TVec3f mPos;
};

class GlaringLightAreaMgr : public AreaObjMgr {
public:
    GlaringLightAreaMgr(s32, const char*);

    /* 0x08 */ virtual ~GlaringLightAreaMgr();
};
