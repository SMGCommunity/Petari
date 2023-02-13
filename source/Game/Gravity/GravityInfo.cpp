#include "Game/Gravity.h"

GravityInfo::GravityInfo() {
	mLargestPriority = -1;
	mGravityVector.x = 0.0f;
	mGravityVector.y = 0.0f;
	mGravityVector.z = 0.0f;
	mGravityInstance = nullptr;
}

void GravityInfo::init() {
	mLargestPriority = -1;
	mGravityVector.z = 0.0f;
	mGravityVector.y = 0.0f;
	mGravityVector.x = 0.0f;
	mGravityInstance = nullptr;
}
