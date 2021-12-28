#include "Game/Gravity.h"

ConeGravity::ConeGravity() : PlanetGravity() {
	_88 = 360.0f;
	_8C = -1.0f;
	_90 = 0.0f;
	mEnableBottom = true;
	mTopCutRate = 0.0f;
	mLocalMtx.identity();
	_58.identity();
}

void ConeGravity::setEnableBottom(bool val) {
	mEnableBottom = val;
}

void ConeGravity::setTopCutRate(f32 val) {
	if (val < 0.0f)
		val = 0.0f;
	else if (val > 1.0f)
		val = 1.0f;

	mTopCutRate = val;
}