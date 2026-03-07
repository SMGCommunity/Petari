#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class BigBubbleMoveLimitter : public NameObj {
public:
    BigBubbleMoveLimitter(const char*, s32);

    virtual ~BigBubbleMoveLimitter();
    virtual void init(const JMapInfoIter&);

    bool limitPosition(TVec3f*, f32) const;
    bool limitVelocity(TVec3f*, const TVec3f&, f32) const;

    s32 _C;
    TVec3f _10;
    TVec3f _1C;
    TVec3f _28;
    TVec3f _34;
    TVec3f _40;
    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    s32 _5C;
    s32 _60;
};
