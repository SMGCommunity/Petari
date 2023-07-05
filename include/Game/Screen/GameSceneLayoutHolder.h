#pragma once

class GameSceneLayoutHolder {
public:
	void changeLifeMeterGround();
	void changeLifeMeterSwim();
};

namespace MR {
	GameSceneLayoutHolder& getGameSceneLayoutHolder();
}
