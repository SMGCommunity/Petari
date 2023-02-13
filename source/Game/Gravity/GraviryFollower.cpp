#pragma once

#include "Game/Gravity.h"
#include "Game/Util.h"

GraviryFollower::GraviryFollower(GlobalGravityObj *pGravityObj, const JMapInfoIter &rIter) : BaseMatrixFollower(pGravityObj, rIter), mGravity(nullptr) {
	mGravity = pGravityObj->getGravity();
}

void GraviryFollower::setGravityFollowHost(const NameObj *pHost) {
	mGravity->mHost = pHost;
}

void GraviryFollower::update() {
	GlobalGravityObj* pFollowTarget = static_cast<GlobalGravityObj*>(getFollowTargetActor());

	if (MR::isDead(pFollowTarget) || MR::isClipped(pFollowTarget) || !isValid()) {
		mGravity->mValidFollower = false;
	}
	else {
		TPos3f followMtx;
		calcFollowMatrix(&followMtx);

		mGravity->updateMtx(followMtx);
		mGravity->mValidFollower = true;
	}
}

namespace MR {
	bool addBaseMatrixFollowerGravity(GlobalGravityObj *pGravityObj, const JMapInfoIter &rIter) {
		if (!MR::isValidFollowID(rIter)) {
			return false;
		}

		MR::addBaseMatrixFollower(new GraviryFollower(pGravityObj, rIter));
		return true;
	}
}
