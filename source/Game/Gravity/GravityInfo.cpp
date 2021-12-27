#include "Game/Gravity.h"

GravityInfo::GravityInfo() {
	_C = -1;
	_0.x = 0.0f;
	_0.y = 0.0f;
	_0.z = 0.0f;
	mGravity = NULL;
}

void GravityInfo::init() {
	_C = -1;
	_0.z = 0.0f;
	_0.y = 0.0f;
	_0.x = 0.0f;
	mGravity = NULL;
}
