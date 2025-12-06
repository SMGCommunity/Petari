#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class MapPartsRailGuidePoint : public LiveActor {
public:
    MapPartsRailGuidePoint(const LiveActor*, const char*, f32, bool);
    MapPartsRailGuidePoint(const LiveActor*, const char*, int, bool);

    virtual ~MapPartsRailGuidePoint();
    virtual void init(const JMapInfoIter&);

    f32 _8C;
    bool _90;
};