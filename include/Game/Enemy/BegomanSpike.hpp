#pragma once

#include "Game/Enemy/BegomanBase.hpp"


class BegomanSpike : public BegomanBase {
public:
    BegomanSpike(const char*);
    virtual ~BegomanSpike();
    virtual u32 getKind() const;
    virtual bool setNerveReturn();
    virtual const Nerve* getNerveWait();

private:
    u8 mPad[(0x16C) - sizeof(BegomanBase)];
};
