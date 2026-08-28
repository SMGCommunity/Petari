#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class DeathArea : public AreaObj {
public:
    enum EDeathType { DeathType_Any, DeathType_AnyNoPlayer };

    DeathArea(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x24 */ virtual bool isInVolume(const TVec3f&) const;

    /* 0x28 */ virtual const char* getManagerName() const {
        return "DeathArea";
    }

    s32 getDeathType() const;
};
