#pragma once

#include "Game/AreaObj/AreaObj.h"
#include "Inline.h"

class DeathArea : public AreaObj {
public:
    DeathArea(int, const char *);

    virtual void init(const JMapInfoIter &);
    virtual void movement();
    virtual bool isInVolume(const TVec3f &) const;
    virtual const char* getManagerName() const;

    s32 getDeathType() const NO_INLINE;
};