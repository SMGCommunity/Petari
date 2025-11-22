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
    void exeWipeClose();
    void exeWipeIn();
    void exeOpen();
    void exeWipeOut();

private:
    /* 0x20 */ TVec2f _20;
    /* 0x28 */ s32 mStepNum;
    /* 0x2C */ f32 _2C;
    /* 0x30 */ f32 _30;
    /* 0x24 */ bool _34;
};
