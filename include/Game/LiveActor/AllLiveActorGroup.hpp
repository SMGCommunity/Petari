#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"

class AllLiveActorGroup : public LiveActorGroup {
public:
    /// @brief Creates a new `AllLiveActorGroup`.
    AllLiveActorGroup();

    void startInitActorSystemInfo();
    void initActorSystemInfo(const JMapInfoIter&);
    void endInitActorSystemInfo();

    /* 0x18 */ s32 mActorNum;
};

namespace MR {
    AllLiveActorGroup* getAllLiveActorGroup();
    void startInitLiveActorSystemInfo();
    void initLiveActorSystemInfo(const JMapInfoIter&);
    void endInitLiveActorSystemInfo();
};  // namespace MR
