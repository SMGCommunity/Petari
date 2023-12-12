#pragma once

#include "Game/AreaObj/AreaObj.h"

class MercatorTransformCube : public AreaObj {
public:
    MercatorTransformCube(int, const char *);
    virtual ~MercatorTransformCube();

    virtual void init(const JMapInfoIter &);

    void convertTransAndRotate(TVec3f *, TPos3f *, const TVec3f &, bool) const;
    f32 getSphereRadius() const;
    void calcUV(TVec2f *, const TVec3f &) const;
    void calcLocalBoxSize(TVec3f *) const;

    TMtx34f _3C;
};
