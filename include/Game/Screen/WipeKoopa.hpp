#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"

class WipeKoopa : public WipeLayoutBase {
public:
    WipeKoopa();

    virtual void init(const JMapInfoIter&);
    virtual void wipe(s32);
    virtual void forceClose();
    virtual void forceOpen();
    virtual bool isOpen() const;
    virtual bool isClose() const;
    virtual bool isWipeIn() const;
    virtual bool isWipeOut() const;

    virtual s32 getWipeType() const {
        return 2;
    }

    void exeWait();
    void exeWipeOut();
    void exeClose();

private:
    /* 0x20 */ s32 mStepNum;
};
