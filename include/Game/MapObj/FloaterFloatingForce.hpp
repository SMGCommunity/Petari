#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class FloaterFloatingForce : public MapPartsFunction {
public:
    FloaterFloatingForce(LiveActor*, const char*);

    /* 0x08 */ virtual ~FloaterFloatingForce();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void start();

    /* 0x38 */ virtual void updateHostTrans(TVec3f*) const {
    }

    /* 0x3C */ virtual void updateHostVelocity(TVec3f*) const {
    }

    /* 0x40 */ virtual const TVec3f& getCurrentVelocity() const {
        return TVec3f(0.0f, 0.0f, 0.0f);
    }

    /* 0x18 */ const char* _18;
    /* 0x1C */ TVec3f _1C;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ f32 mMoveConditionType;
};
