#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class AstroChangeStageCube : public AreaObj {
public:
    AstroChangeStageCube(int, const char*);

    /* 0x08 */ virtual ~AstroChangeStageCube();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x24 */ virtual bool isInVolume(const TVec3f&) const;

    u32 _3C;
    u32 _40;
    s32 mWaitFrame;
};
