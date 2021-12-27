#include "Game/Gravity.h"

PlanetGravityManager::PlanetGravityManager(const char *pName) : NameObj(pName) {
	mNumGravities = 0;
}

void PlanetGravityManager::init(const JMapInfoIter &rIter) {

}

// PlanetGravityManager::calcTotalGravityVector

#ifdef NON_MATCHING
void PlanetGravityManager::registerGravity(PlanetGravity *pGravity) {
	pGravity->mIsRegistered = true;
	s32 index = mNumGravities++;
	mGravities[index] = pGravity;

	for (s32 i = mNumGravities - 1; i >= 1 ; i--) {
		PlanetGravity* v7 = mGravities[i];
		PlanetGravity* v8 = mGravities[i - 1];

		if (v7->mPriority <= v8->mPriority)
			break;

		mGravities[i - 1] = v7;
		mGravities[i] = v8;
	}
}
#endif
