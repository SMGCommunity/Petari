#pragma once

#include "Game/MapObj/MapPartsFunction.h"

class FloaterFloatingForce : public MapPartsFunction {
public:
    FloaterFloatingForce(LiveActor *, const char *);

    virtual ~FloaterFloatingForce();
    virtual void init(const JMapInfoIter &);
    virtual void start();
    virtual void updateHostTrans(TVec3f *) const {

    }

    virtual void updateHostVelocity(TVec3f *) const {

    }

    virtual const TVec3f& getCurrentVelocity() const;

    const char* _18;
    TVec3f _1C;
    TVec3f _28;
    f32 mMoveCondition; // _34
};