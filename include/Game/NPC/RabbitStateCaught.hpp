#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class LiveActor;
class ModelObj;
class TalkMessageCtrl;

class RabbitStateCaught : public ActorStateBase< LiveActor > {
public:
    RabbitStateCaught(LiveActor* pHost, TalkMessageCtrl* pTalkCtrl);

    virtual void init();
    virtual void appear();
    virtual void control();

    void unusePowerStarModel();
    void exeTryStartDemo();
    void exeCaught();
    void exeCaughtLand();
    void exeCaughtEvent();
    void setCaughtStartMarioPose();
    void blendBaseMatrixToMario(f32) const;

    /* 0x10 */ TQuat4f mCaughtStartMarioQuat;
    /* 0x20 */ TVec3f mCaughtStartMarioTrans;
    /* 0x2C */ TalkMessageCtrl* mTalkCtrl;
    /* 0x30 */ ModelObj* mPowerStarModel;
    /* 0x34 */ TMtx34f mCaughtLandMarioBaseMtx;
    /* 0x64 */ bool mUsePowerStarModel;
};
