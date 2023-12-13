#include "Game/Gravity.hpp"

ConeGravity::ConeGravity() : PlanetGravity() {
	_88 = 360.0f;
	_8C = -1.0f;
	_90 = 0.0f;
	mEnableBottom = true;
	mTopCutRate = 0.0f;
	mLocalMtx.identity();
	mWorldMtx.identity();
}

void ConeGravity::setLocalMatrix(const TPos3f &rMtx) {
	mLocalMtx.setInline(rMtx);
	updateIdentityMtx();
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

void ConeGravity::updateMtx(const TPos3f &rMtx) {
	mWorldMtx.concat(rMtx, mLocalMtx);

	TVec3f dirX;
	mWorldMtx.getXDirInline(dirX);
	_90 = VECMag(reinterpret_cast<Vec*>(&dirX));

	TVec3f dirY; // unused
	mWorldMtx.getYDir(dirY);
}
