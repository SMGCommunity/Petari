#pragma once

#include "Game/MapObj/MapPartsRotator.hpp"

class MapPartsSeesaw2AxisRotator : public MapPartsRotatorBase {
public:
    MapPartsSeesaw2AxisRotator(LiveActor*, const char*, f32);

    virtual ~MapPartsSeesaw2AxisRotator();
    virtual void init(const JMapInfoIter&) override;
    virtual bool isWorking() const override;
    virtual void start() override;
    virtual void end() override;
    virtual bool receiveMsg(u32) override;
    virtual const TMtx34f& getRotateMtx() const override {
        return mRotateMtx;
    }

    virtual bool isMoving() const override {
        return isWorking();
    }

    void exeWait() {
    }

    void exeMove();
    void exeStay();
    void exeHipDrop();
    void rotate();
    void restoreMove();
    f32 getInertiaConst() const;

    /* 0x18 */ f32 mRotateAngle;
    /* 0x1C */ f32 mInertiaConst;
    /* 0x20 */ f32 mRestoreForce;
    /* 0x24 */ TPos3f mBaseMtx;
    /* 0x54 */ TPos3f mBaseMtxInv;
    /* 0x84 */ TPos3f mRotateMtx;
    /* 0xB4 */ TVec3f mUp;
    /* 0xC0 */ bool mHipDrop;
    /* 0xC4 */ f32 mRotateSpeed;
    /* 0xC8 */ const char* mMoveSound;
    /* 0xCC */ f32 mMoveSoundSpeed;
};
