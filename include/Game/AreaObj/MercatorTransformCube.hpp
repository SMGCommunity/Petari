#pragma once

#include "Game/AreaObj/AreaObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class DivideMercatorRailPosInfo {
public:
    inline DivideMercatorRailPosInfo() {}

    virtual void setPosition(s32, const TVec3f&) = 0;
};

class MercatorTransformCube : public AreaObj {
public:
    MercatorTransformCube(int, const char*);
    virtual ~MercatorTransformCube();

    virtual void init(const JMapInfoIter&);

    void convertTransAndRotate(TVec3f*, TPos3f*, const TVec3f&, bool) const;
    f32 getSphereRadius() const;
    void calcUV(TVec2f*, const TVec3f&) const;
    void calcLocalBoxSize(TVec3f*) const;

    TMtx34f _3C;
};

namespace MR {
    void getDivideMercatorRailPosition(DivideMercatorRailPosInfo*, const LiveActor*, u32, f32, u32);

    void initDefaultPosForMercator(LiveActor*, const JMapInfoIter&, bool);

    void convertMercatorPlaneToSphereTransAndRotate(TVec3f*, TPos3f*, const TVec3f&, bool);

    MapPartsRailMover* createMapPartsRailMoverForMercator(LiveActor*, const JMapInfoIter&, bool);
};  // namespace MR
