#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class AirBubble;

class AirBubbleHolder : public DeriveActorGroup< AirBubble > {
public:
    /// @brief Creates a new `AirBubbleHolder`.
    /// @param pName A pointer to the null-terminated name of the object.
    AirBubbleHolder(const char* pName);

    virtual void init(const JMapInfoIter&);

    void appearAirBubble(const TVec3f&, s32);
};

namespace MR {
    AirBubbleHolder* createAirBubbleHolder();
    void appearAirBubble(const TVec3f&, s32);
};  // namespace MR
