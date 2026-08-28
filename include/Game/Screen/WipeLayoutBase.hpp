#pragma once

#include "Game/Screen/LayoutActor.hpp"

class WipeLayoutBase : public LayoutActor {
public:
    WipeLayoutBase(const char* pName) : LayoutActor(pName, true) {
    }

    /* 0x30 */ virtual void wipe(s32) = 0;
    /* 0x34 */ virtual void forceClose() = 0;
    /* 0x38 */ virtual void forceOpen() = 0;
    /* 0x3C */ virtual bool isOpen() const = 0;
    /* 0x40 */ virtual bool isClose() const = 0;
    /* 0x44 */ virtual bool isWipeIn() const = 0;
    /* 0x48 */ virtual bool isWipeOut() const = 0;

    /* 0x4C */ virtual const char* getWipeName() const {
        return mName;
    }

    virtual s32 getWipeType() const {
        return 0;
    }
};
