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

    /* 0x0C */ PlanetMap* mOceanHomePlanet;           // Referencing an removed object?
    /* 0x10 */ PlanetMap* mOceanRingPlanet;           // OceanRingPlanet
    /* 0x14 */ u32 _14;                               // Unused variable
    /* 0x18 */ ModelObj* mOceanRingPlanetLowInWater;  // OceanRingPlanetLowInWater
    /* 0x1C */ u32 _1C;                               // Unused variable
    /* 0x20 */ u32 _20;                               // Unused variable
};

namespace OceanHomeMapFunction {
    void tryEntryOceanHomeMap(PlanetMap*);
};  // namespace OceanHomeMapFunction
