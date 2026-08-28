#pragma once

#include "Game/Enemy/BegomanBase.hpp"

class BegomanSpring : public BegomanBase {
public:
    BegomanSpring(const char*);
    /* 0x08 */ virtual ~BegomanSpring();
    virtual u32 getKind() const;
    /* 0x7C */ virtual bool setNerveReturn();
    /* 0x84 */ virtual const Nerve* getNerveWait();

private:
    u8 mPad[(0x190) - sizeof(BegomanBase)];
};
