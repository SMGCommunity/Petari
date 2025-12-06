#pragma once

#include "Game/LiveActor/LiveActorGroup.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class ClipArea;

class ClipAreaHolder : public LiveActorGroup {
public:
    ClipAreaHolder(const char*);

    virtual ~ClipAreaHolder();
    virtual void init(const JMapInfoIter& rIter);

    bool isInArea(const TVec3f&, f32) const;

    /* 0x18 */ bool mIsActive;
};

namespace MR {
    void createClipAreaHolder();
    bool isExistClipAreaHolder();
    void addClipArea(ClipArea*);
    bool isInClipArea(const TVec3f&, f32);
    bool isActiveClipArea();
    void activateClipArea();
    void deactivateClipArea();
    void setBinderExceptSensorType(LiveActor*, const TVec3f*, f32);
};  // namespace MR
