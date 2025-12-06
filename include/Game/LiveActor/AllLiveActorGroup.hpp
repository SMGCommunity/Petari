#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class AllLiveActorGroup : public LiveActorGroup {
public:
    AllLiveActorGroup();

    virtual ~AllLiveActorGroup();

    void initActorSystemInfo(const JMapInfoIter&);

    s32 _18;
};

namespace MR {
    AllLiveActorGroup* getAllLiveActorGroup();
    void startInitLiveActorSystemInfo();
    void initLiveActorSystemInfo(const JMapInfoIter&);
    void endInitLiveActorSystemInfo();
};  // namespace MR
