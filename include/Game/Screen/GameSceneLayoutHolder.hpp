#pragma once

class GameSceneLayoutHolder {
public:
    void changeLifeMeterModeGround();
    void changeLifeMeterModeWater();
};

namespace MR {
    GameSceneLayoutHolder& getGameSceneLayoutHolder();
};
