#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


class GCaptureTarget : public LiveActor {
public:
    GCaptureTarget(const char*);
    virtual ~GCaptureTarget();

private:
    u8 mPad[(0xA8) - sizeof(LiveActor)];
};
