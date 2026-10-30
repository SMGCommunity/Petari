#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapPartsRailGuidePoint : public LiveActor {
public:
    MapPartsRailGuidePoint(const LiveActor*, const char*, f32, bool);
    MapPartsRailGuidePoint(const LiveActor*, const char*, int, bool);

    virtual ~MapPartsRailGuidePoint();
    virtual void init(const JMapInfoIter&);

    /* 0x8C */ f32 mRailCoord;
    /* 0x90 */ bool mHasShadow;
};
