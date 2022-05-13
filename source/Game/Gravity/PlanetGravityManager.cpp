#include "Game/Gravity.h"

PlanetGravityManager::PlanetGravityManager(const char *pName) : NameObj(pName) {
	mNumGravities = 0;
}

void PlanetGravityManager::init(const JMapInfoIter &rIter) {

}

bool PlanetGravityManager::calcTotalGravityVector(TVec3f *pDest, GravityInfo *pInfo, const TVec3f &rPosition, u32 gravityType, u32 host) const {
	TVec3f totalGravity;
	totalGravity.z = 0.0f;
	totalGravity.y = 0.0f;
	totalGravity.x = 0.0f;

	PlanetGravity** pGravities = (PlanetGravity**)&mGravities;
	bool hasCalculated = false;

	if (pInfo) {
		pInfo->init();
	}

	f32 largestScalar = -1.0f;
	s32 largestPriority = -1;

	while (pGravities != &mGravities[mNumGravities]) {
		// Is gravity valid/active?
		bool validGravity = false;

		if ((*pGravities)->mActivated && (*pGravities)->mValidFollower && (*pGravities)->mAppeared) {
			validGravity = true;
		}

		// Gravity is valid, matches specified gravity type(s) and does not have same host
		if (validGravity && ((gravityType & (*pGravities)->mGravityType) != 0) && (host != (u32)(*pGravities)->mHost)) {
			// Is priority lower -> end of list reached as it's sorted in descending order
			s32 priority = (*pGravities)->mPriority;

			if (priority < largestPriority) {
				break;
			}

			// Calculate gravity
			TVec3f gravityVec;

			if ((*pGravities)->calcGravity(&gravityVec, rPosition)) {
				bool storeInfo = false;
				f32 scalar = VECMag(reinterpret_cast<const Vec*>(&gravityVec));

				// If same priority, add gravity vector to total result
				if (priority == largestPriority) {
					totalGravity.addInline(gravityVec);
					hasCalculated = true;

					if (largestScalar < scalar) {
						storeInfo = true;
					}
				}
				// If larger priority, total gravity gets overriden
				else if (priority > largestPriority) {
					largestPriority = priority;
					hasCalculated = true;
					storeInfo = true;
					totalGravity = gravityVec;
				}

				// Try to store gravity info if necessary
				if (pInfo && storeInfo) {
					largestScalar = scalar;
					pInfo->mGravityVector = gravityVec;
					pInfo->mLargestPriority = largestPriority;
					pInfo->mGravityInstance = *pGravities;
				}
			}
		}

		pGravities++;
	}

	// Store normalized vector as result if necessary
	if (pDest) {
		MR::normalizeOrZero(&totalGravity);
		*pDest = totalGravity;
	}

	return hasCalculated;
}

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
