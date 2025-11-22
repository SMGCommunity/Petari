#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AstroChangeStageCube : public AreaObj {
public:
    AstroChangeStageCube(int, const char*);

    virtual ~AstroChangeStageCube() {}

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual bool isInVolume(const TVec3f&) const;

    u32 _3C;
    u32 _40;
    u32 _44;
};