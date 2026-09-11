#pragma once

#include "Game/MapObj/ClipArea.hpp"

class ClipAreaShapeSphere;

class ClipAreaDrop : public ClipArea {
public:
    /// @brief Creates a new `ClipArea`.
    /// @param pName A pointer to the null-terminated name of the object.
    ClipAreaDrop(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void setBaseSize(f32);

    void exeWait();

    /* 0xC0 */ ClipAreaShapeSphere* mShapeSphere;
    /* 0xC4 */ f32 mBaseSize;
};
