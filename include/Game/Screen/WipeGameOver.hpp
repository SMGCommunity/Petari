#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"

class WipeGameOver : public WipeLayoutBase {
public:
    WipeGameOver();

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void kill();
    /* 0x30 */ virtual void wipe(s32);
    /* 0x34 */ virtual void forceClose();
    /* 0x38 */ virtual void forceOpen();
    /* 0x3C */ virtual bool isOpen() const;
    /* 0x40 */ virtual bool isClose() const;
    /* 0x44 */ virtual bool isWipeIn() const;
    /* 0x48 */ virtual bool isWipeOut() const;

    virtual s32 getWipeType() const {
        return 2;
    }

    void exeActive();
    void exeWait();
};
