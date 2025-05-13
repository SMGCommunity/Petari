#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyCometScreenFilter : LayoutActor {
public:
    /// @brief Creates a new `GalaxyCometScreenFilter`.
    GalaxyCometScreenFilter();

    /// @brief Destroys the `GalaxyCometScreenFilter`.
    virtual ~GalaxyCometScreenFilter();

    void setCometType(const char*);

private:
    /* 0x20 */ bool _20;
};
