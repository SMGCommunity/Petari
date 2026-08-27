#pragma once

#include "Game/MapObj/MapPartsFunction.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class FloaterFloatingForce : public MapPartsFunction {
public:
    FloaterFloatingForce(LiveActor*, const char*);

    virtual ~FloaterFloatingForce();
    virtual void init(const JMapInfoIter&);
    virtual void start();

    virtual void updateHostTrans(TVec3f*) const {
    }

    virtual void updateHostVelocity(TVec3f*) const {
    }

    virtual const TVec3f& getCurrentVelocity() const {
        return TVec3f(0.0f, 0.0f, 0.0f);
    }

    /* 0x18 */ const char* _18;
    /* 0x1C */ TVec3f _1C;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ f32 mMoveConditionType;
};
