#pragma once

#include "Game/NPC/TicoFat.hpp"

class TicoStarRing;

class TicoGalaxy : public TicoFat {
public:
    TicoGalaxy(const char *);

    const char* mGalaxyName;            // 0x208
    TicoStarRing* mStarRing;            // 0x20C
};
