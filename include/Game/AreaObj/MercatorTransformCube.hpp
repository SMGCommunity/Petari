#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class LiveActor;
class MapPartsRailMover;

class DivideMercatorRailPosInfo {
public:
    inline DivideMercatorRailPosInfo() {
    }

    virtual void setPosition(s32, const TVec3f&) = 0;
};

class MercatorTransformCube : public AreaObj {
public:
    MercatorTransformCube(int, const char*);

    virtual void init(const JMapInfoIter&);

    void convertTransAndRotate(TVec3f*, TPos3f*, const TVec3f&, bool) const;
    f32 getSphereRadius() const;
    void calcUV(TVec2f*, const TVec3f&) const;
    void calcLocalBoxSize(TVec3f*) const;

    /* 0x3C */ TMtx34f _3C;
};

namespace MR {
    void convertMercatorPlaneToSphereTransAndRotate(TVec3f*, TPos3f*, const TVec3f&, bool);
    void convertMercatorPlaneToSphereTransAndRotate(TVec3f*, TVec3f*, const TVec3f&, bool);
    f32 initDefaultPosForMercator(LiveActor*, const JMapInfoIter&, bool);
    void calcNearestRailPosForMercator(TVec3f*, const LiveActor*, f32);
    void calcRailClippingInfoForMercator(TVec3f*, f32*, LiveActor*, f32, f32);
    void initAndSetRailClippingForMercator(TVec3f*, LiveActor*, f32, f32);
    f32 calcRailTotalLengthForMercator(const LiveActor*, f32);
    void getDivideMercatorRailPosition(DivideMercatorRailPosInfo*, const LiveActor*, u32, f32, u32);
    MapPartsRailMover* createMapPartsRailMoverForMercator(LiveActor*, const JMapInfoIter&, bool);
};  // namespace MR
