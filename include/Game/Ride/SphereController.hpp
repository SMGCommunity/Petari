#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class SphereController {
public:
    /// @brief Creates a new `SphereController`.
    SphereController();

    virtual f32 calcMoveVector(TVec3f*, const TVec3f&);
    virtual f32 calcJumpPower() const;
    /* 0x08 */ virtual bool doBrake() const;
    /* 0x0C */ virtual void update(const TVec3f&);
    /* 0x10 */ virtual void notifyActivate();
    /* 0x14 */ virtual void notifyDeactivate();
    // void drawDebug(); // DEBUG

protected:
    /* 0x04 */ u32 _4;
};
