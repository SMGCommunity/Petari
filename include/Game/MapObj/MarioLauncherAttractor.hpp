#pragma once

#include "Game/Gravity/GravityCreator.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class MarioLauncherAttractor : public LiveActor {
public:
    MarioLauncherAttractor(const char* pName);

    virtual ~MarioLauncherAttractor() {}
    virtual void init(const JMapInfoIter& rIter);

    PointGravityCreator* mGravityCreator;  // 0x8C
};

namespace NrvMarioLauncherAttractor {
    class MarioLauncherAttractorNrvWait : public Nerve {
    public:
        virtual void execute(Spine* pSpine) const;

        static MarioLauncherAttractorNrvWait sInstance;
    };
};  // namespace NrvMarioLauncherAttractor
