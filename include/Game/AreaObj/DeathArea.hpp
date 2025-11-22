#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Inline.hpp"

class DeathArea : public AreaObj {
public:
    DeathArea(int, const char*);

    virtual ~DeathArea();
    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual bool isInVolume(const TVec3f&) const;
    virtual const char* getManagerName() const;

    s32 getDeathType() const NO_INLINE;
};