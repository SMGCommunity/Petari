#pragma once

#include "Game/Screen/LayoutActor.hpp"

class WipeLayoutBase : public LayoutActor {
public:
    inline WipeLayoutBase(const char* pName) : LayoutActor(pName, true) {}

    virtual ~WipeLayoutBase() {}
    virtual void wipe(s32) = 0;
    virtual void forceClose() = 0;
    virtual void forceOpen() = 0;
    virtual bool isOpen() const = 0;
    virtual bool isClose() const = 0;
    virtual bool isWipeIn() const = 0;
    virtual bool isWipeOut() const = 0;

    virtual const char* getWipeName() const { return mName; }
    virtual s32 getWipeType() const { return 0; }
};