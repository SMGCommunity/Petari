#pragma once

#include "NameObj/NameObj.h"

enum SceneObj
{
    SENSOR_HIT_CHECKER = 0x0,
    COLLISION_DIRECTOR = 0x1,
    CLIPPING_DIRECTOR = 0x2,
    DEMO_DIRECTOR = 0x3,
    EVENT_DIRECTOR = 0x4,
    EFFECT_SYSTEM = 0x5,
    LIGHT_DIRECTOR = 0x6,
    SCENE_DATA_INITIALIZER = 0x7,
    STAGE_DATA_HOLDER = 0x8,
    MSG_SENSOR_HOLDER = 0x9,
    STAGE_SWITCH_CONTROLLER = 0xA,
    SWITCH_WATCHER_HOLDER = 0xB,
    SLEEP_CONTROLLER = 0xC,

    OCEAN_HOME_MAP_CTRL = 0x40,

    RACE_MANAGER = 0x65,

    SPHERE_SELECTOR = 0x6F,

    PLANET_MAP_CREATOR = 0x74
};

class SceneObjHolder
{
public:
    SceneObjHolder();

    NameObj* create(int);
    bool isExist(int) const;
    NameObj* getObj(int) const;
    NameObj* newEachObj(int);

    NameObj* mObjs[0x7B]; // _0
};