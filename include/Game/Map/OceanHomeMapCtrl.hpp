#pragma once

#include "Game/NameObj/NameObj.hpp"

class PlanetMap;
class ModelObj;

class OceanHomeMapCtrl : public NameObj {
public:
    OceanHomeMapCtrl();

    virtual ~OceanHomeMapCtrl();
    virtual void init(const JMapInfoIter&);
    virtual void movement();

    void entryMapRing(PlanetMap*);

    PlanetMap* mOceanHomePlanet;          // 0x0C : Referencing an removed object?
    PlanetMap* mOceanRingPlanet;          // 0x10 : OceanRingPlanet
    u32 _14;                              // 0x14 : Unused variable
    ModelObj* mOceanRingPlanetLowInWater; // 0x18 : OceanRingPlanetLowInWater
    u32 _1C;                              // 0x1C : Unused variable
    u32 _20;                              // 0x20 : Unused variable
};

namespace OceanHomeMapFunction {
    void tryEntryOceanHomeMap(PlanetMap*);
};
