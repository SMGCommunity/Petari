#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"

class WipeGameOver : public WipeLayoutBase {
public:
    WipeGameOver();

    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual void wipe(s32);
    virtual void forceClose();
    virtual void forceOpen();
    virtual bool isOpen() const;
    virtual bool isClose() const;
    virtual bool isWipeIn() const;
    virtual bool isWipeOut() const;

    virtual s32 getWipeType() const { return 2; }

    void exeActive();
    void exeWait();
};
