#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyCometScreenFilter : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyCometScreenFilter`.
    GalaxyCometScreenFilter();

    void setCometType(const char*);

    /* 0x20 */ bool _20;
};
