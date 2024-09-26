#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Gravity/GravityCreator.hpp"

class MarioLauncherAttractor : public LiveActor {
	MarioLauncherAttractor(const char *pName);

	virtual ~MarioLauncherAttractor() {}
	virtual void init(const JMapInfoIter &rIter);

	PointGravityCreator* mGravityCreator; // 0x8C
};

namespace NrvMarioLauncherAttractor {
	class MarioLauncherAttractorNrvWait : public Nerve {
	public:
		virtual void execute(Spine *pSpine) const;

		static MarioLauncherAttractorNrvWait sInstance;
	};
};
