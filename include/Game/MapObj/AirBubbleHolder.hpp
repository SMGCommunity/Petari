#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class AirBubbleHolder : public LiveActorGroup {
public:
    AirBubbleHolder(const char*);

    virtual void init(const JMapInfoIter&);

    void appearAirBubble(const TVec3f&, s32);
};

namespace MR {
    AirBubbleHolder* createAirBubbleHolder();
    void appearAirBubble(const TVec3f&, s32);
};  // namespace MR