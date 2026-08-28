#pragma once

#include "Game/Enemy/BegomanBase.hpp"

class BegomanSpike : public BegomanBase {
public:
    BegomanSpike(const char*);
    /* 0x08 */ virtual ~BegomanSpike();
    virtual u32 getKind() const;
    /* 0x7C */ virtual bool setNerveReturn();
    /* 0x84 */ virtual const Nerve* getNerveWait();

private:
    u8 mPad[(0x16C) - sizeof(BegomanBase)];
};
