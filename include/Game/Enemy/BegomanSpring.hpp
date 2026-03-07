#pragma once

#include "Game/Enemy/BegomanBase.hpp"


class BegomanSpring : public BegomanBase {
public:
    BegomanSpring(const char*);
    virtual ~BegomanSpring();
    virtual u32 getKind() const;
    virtual bool setNerveReturn();
    virtual const Nerve* getNerveWait();

private:
    u8 mPad[(0x190) - sizeof(BegomanBase)];
};
