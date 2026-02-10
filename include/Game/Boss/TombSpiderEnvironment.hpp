#pragma once

class LiveActor;
class PlanetMap;
class ModelObj;

class TombSpiderEnvironment {
public:
    TombSpiderEnvironment(LiveActor*);

    /* 0x00 */ LiveActor* mActor;
    /* 0x04 */ PlanetMap* mPlanet;
    /* 0x08 */ ModelObj* mCocoon;
};
