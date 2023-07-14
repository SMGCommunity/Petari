#pragma once

class MarioHolder {
public:
	void setMarioActor(MarioActor* actor);
};

class MarioActor;


namespace MR {
	MarioHolder* getMarioHolder();
}
