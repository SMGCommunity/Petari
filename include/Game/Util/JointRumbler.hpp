#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class LiveActor;

class JointRumbler {
public:
    JointRumbler(LiveActor*, const char*, f32, f32, u32, bool, s32);

    void update();
    void start();
    void reset();
    bool isRumbling() const;
    void updateModelScale(f32);
    void updateScale(TVec3f*, f32);
};
