#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class FurMulti : public LiveActor {
public:
    void onDraw(u32);
    void offDraw(u32);
};

namespace MR {
    LiveActor* initMultiFur(LiveActor*, s32);
};  // namespace MR
