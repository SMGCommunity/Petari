#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"
#include "Game/Util/Color.hpp"

class WipeFade : public WipeLayoutBase {
public:
    WipeFade(const char*, const Color8&);

    virtual void init(const JMapInfoIter&);
    virtual void draw() const;
    virtual void control();
    virtual void wipe(s32);
    virtual void forceClose();
    virtual void forceOpen();
    virtual bool isOpen() const;
    virtual bool isClose() const;
    virtual bool isWipeIn() const;
    virtual bool isWipeOut() const;

private:
    /* 0x20 */ bool   mIsWipeIn;
    /* 0x24 */ s32    mStepNum;
    /* 0x28 */ s32    mStep;
    /* 0x2C */ Color8 mFillColor;
};
