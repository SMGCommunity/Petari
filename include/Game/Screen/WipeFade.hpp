#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"
#include "Game/Util/Color.hpp"

class WipeFade : public WipeLayoutBase {
public:
    WipeFade(const char*, const Color8&);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x18 */ virtual void draw() const;
    /* 0x2C */ virtual void control();
    /* 0x30 */ virtual void wipe(s32);
    /* 0x34 */ virtual void forceClose();
    /* 0x38 */ virtual void forceOpen();
    /* 0x3C */ virtual bool isOpen() const;
    /* 0x40 */ virtual bool isClose() const;
    /* 0x44 */ virtual bool isWipeIn() const;
    /* 0x48 */ virtual bool isWipeOut() const;

    void setColor(GXColor);

private:
    /* 0x20 */ bool mIsWipeIn;
    /* 0x24 */ s32 mFrame;
    /* 0x28 */ s32 mStep;
    /* 0x2C */ Color8 mFillColor;
};
