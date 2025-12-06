#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/MapObj/ClipAreaDrop.hpp"
#include "Game/NameObj/NameObj.hpp"

class ClipAreaDropHolder : public DeriveActorGroup< ClipAreaDrop > {
public:
    ClipAreaDropHolder();
    virtual ~ClipAreaDropHolder();
};

namespace MR {
    NameObj* createClipAreaDropHolder();
    ClipAreaDrop* getDeadClipAreaDrop();
    bool appearClipAreaDrop(const TVec3f&, f32);
}  // namespace MR
