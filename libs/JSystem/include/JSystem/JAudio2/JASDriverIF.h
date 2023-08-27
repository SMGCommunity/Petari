#pragma once

namespace JASDriver {
    typedef s32 (*SubFrameCallback)(void *);
    bool registerSubFrameCallback(SubFrameCallback, void *);
}
