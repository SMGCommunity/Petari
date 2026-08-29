#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class MapPartsRailMover;
class PartsModel;

class ElectricBall : public LiveActor {
public:
    class Ball {
    public:
        Ball();

        void init(LiveActor*);
        void updatePosition(const TPos3f&);
        void activate();

        /* 0x00 */ PartsModel* mHost;
        /* 0x04 */ TVec3f mPosition;
        /* 0x10 */ bool mIsActive;
    };

    ElectricBall(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual MtxPtr getBaseMtx() const {
        return (MtxPtr)mBaseMtx.mMtx;
    };
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void initBalls(const JMapInfoIter&);
    Ball* getNearestBall();
    void startMove();

    void exeWait() {};
    void exeMove() {};
    void exeAttack();

    /* 0x8C */ TPos3f mBaseMtx;
    /* 0xBC */ MR::AssignableArray< Ball > mBalls;
    /* 0xC4 */ TVec3f mClosestBallPos;
    /* 0xD0 */ f32 mRadius;    // Obj_arg1
    /* 0xD4 */ f32 mAngularSpeed;  // Obj_arg2
    /* 0xD8 */ MapPartsRailMover* mRailMover;
};
