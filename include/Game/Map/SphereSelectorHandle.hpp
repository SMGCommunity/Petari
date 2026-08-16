#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SphereSelectorHandle : public LiveActor {
public:
    SphereSelectorHandle(const char*);

    virtual ~SphereSelectorHandle();
    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)&_90;
    }

    void validateRotate();
    void invalidateRotate();
    bool isHolding() const;
    bool isPointing() const;
    void rotateAxisX();
    void rotateAxisY();
    void updateBaseMtx();
    void changeBgmRotateState();
    void playRotateSE();
    void resetRotateParam();
    void clearPointerVelocity();
    void setStateConfirmStartAtFirstStep();
    bool tryRelease();
    void stackPointerVelocity();
    TVec2f* getPointerVelocity();

    void exeWait();
    void exeHold();
    void exeSpin();
    void exeDemoRotate();
    void exeDisappear();
    void exeInvalidRotate();
    void exeGalaxyConfirmStart();
    void exeGalaxyConfirmWait();
    void exeGalaxyConfirmCancel();
    void exeIdleForFileSelect();
    void exeIdleEndForFileSelect();

    inline bool isIdleState();

public:
    /* 0x08C */ bool mIsFileSelectMode;
    /* 0x090 */ TPos3f _90;
    /* 0x0C0 */ TVec3f _C0;
    /* 0x0CC */ f32 mRotateSpeed;
    /* 0x0D0 */ f32 _D0;
    /* 0x0D4 */ f32 _D4;
    /* 0x0D8 */ f32 _D8;
    /* 0x0DC */ TVec3f _DC;
    /* 0x0E8 */ TVec3f _E8;
    /* 0x0F4 */ TVec2f _F4[3];
    /* 0x10C */ s32 _10C;
    /* 0x110 */ TVec3f _110;
    /* 0x11C */ TVec3f _11C;
    /* 0x128 */ bool _128;
};
