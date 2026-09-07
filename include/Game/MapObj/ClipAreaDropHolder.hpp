#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class ClipAreaDrop;

class ClipAreaDropHolder : public DeriveActorGroup< ClipAreaDrop > {
public:
    /// @brief Creates a new `ClipAreaDropHolder`.
    ClipAreaDropHolder();
};

namespace MR {
    NameObj* createClipAreaDropHolder();
    ClipAreaDrop* getDeadClipAreaDrop();
    bool appearClipAreaDrop(const TVec3f&, f32);
};  // namespace MR
