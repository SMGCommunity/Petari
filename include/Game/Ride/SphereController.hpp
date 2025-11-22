#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class SphereController {
public:
    /// @brief Creates a new `SphereController`.
    SphereController();

    virtual f32  calcMoveVector(TVec3f*, const TVec3f&);
    virtual f32  calcJumpPower() const;
    virtual bool doBrake() const;
    virtual void update(const TVec3f&);
    virtual void notifyActivate();
    virtual void notifyDeactivate();

protected:
    /* 0x04 */ u32 _4;
};
