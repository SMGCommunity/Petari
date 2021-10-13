#pragma once

#include "Game/AreaObj/AreaObj.h"

class DeathArea : public AreaObj {
public:
    DeathArea(int, const char *);

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual bool isInVolume(const TVec3f &) const;
    virtual const char* getManagerName() const;

    u32 getDeathType() const;
};