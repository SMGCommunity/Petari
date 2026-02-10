#pragma once
#include "Game/NameObj/NameObj.hpp"
#include "Game/LiveActor/LiveActor.hpp"


namespace JMath {
    void gekko_ps_copy12(void*, const void*);
};

class BossStinkBug : public LiveActor {
public:
    BossStinkBug(const char*);
    virtual ~BossStinkBug();

private:
    u8 mPad[(0x114) - sizeof(LiveActor)];
};
