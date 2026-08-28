#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"

class WipeRing : public WipeLayoutBase {
public:
    WipeRing(bool, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void wipe(s32);
    /* 0x34 */ virtual void forceClose();
    /* 0x38 */ virtual void forceOpen();
    /* 0x3C */ virtual bool isOpen() const;
    /* 0x40 */ virtual bool isClose() const;
    /* 0x44 */ virtual bool isWipeIn() const;
    /* 0x48 */ virtual bool isWipeOut() const;

    void setCenterPos(const TVec3f&);
    void startAnim(const char*);
    bool getMarioCenterPos(TVec3f*);
    f32 calcRadius() const;
    f32 calcMaxRadius() const;
    void updatePlayerPos();
    void exeClose();
    void exeWipeIn();
    void exeOpen();
    void exeWipeOut();

private:
    /* 0x20 */ TVec2f mCenterPos;
    /* 0x28 */ s32 mAnimFrame;
    /* 0x2C */ f32 mWipeOutFrame;
    /* 0x30 */ f32 mWipeInFrame;
    /* 0x34 */ bool mIsCenterOnPlayer;
};
