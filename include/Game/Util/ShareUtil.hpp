#pragma once

#include "Game/NameObj/NameObj.hpp"

class ResourceShare : public NameObj {
public:
    /// @brief Creates a new `ResourceShare`.
    ResourceShare();

    virtual ~ResourceShare();

    /* 0x0C */ void* _C;
    /* 0x10 */ void* _10;
    /* 0x14 */ u32 _14;
};
