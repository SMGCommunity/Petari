#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Gravity/GravityCreator.h"

class MarioLauncherAttractor : public LiveActor {
	MarioLauncherAttractor(const char *pName);

	virtual ~MarioLauncherAttractor() {}
	virtual void init(const JMapInfoIter &rIter);

	PointGravityCreator* mGravityCreator; // _8C
};

namespace NrvMarioLauncherAttractor {
	class MarioLauncherAttractorNrvWait : public Nerve {
	public:
		virtual void execute(Spine *pSpine) const;

		static MarioLauncherAttractorNrvWait sInstance;
	};
};
