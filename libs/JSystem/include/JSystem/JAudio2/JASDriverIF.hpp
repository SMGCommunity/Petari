#pragma once

#include <revolution.h>

namespace JASDriver {
    typedef s32 (*SubFrameCallback)(void*);
    bool registerSubFrameCallback(SubFrameCallback, void*);
}  // namespace JASDriver
