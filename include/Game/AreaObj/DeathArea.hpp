#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class DeathArea : public AreaObj {
public:
    enum EDeathType { DeathType_Any, DeathType_AnyNoPlayer };

    DeathArea(int, const char*);

    virtual ~DeathArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    virtual bool isInVolume(const TVec3f&) const;

    virtual const char* getManagerName() const {
        return "DeathArea";
    }

    s32 getDeathType() const;
};
