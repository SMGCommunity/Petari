#pragma once

#include "Game/Screen/WipeLayoutBase.hpp"

class WipeRing : public WipeLayoutBase {
public:
    WipeRing(bool, const char*);

    virtual void init(const JMapInfoIter&);
    virtual void wipe(s32);
    virtual void forceClose();
    virtual void forceOpen();
    virtual bool isOpen() const;
    virtual bool isClose() const;
    virtual bool isWipeIn() const;
    virtual bool isWipeOut() const;

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
