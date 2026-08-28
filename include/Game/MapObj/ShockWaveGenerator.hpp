#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;

class ShockWaveGenerator : public LiveActor {
public:
    ShockWaveGenerator(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeDemoEcho();
    void exeGenerate();

    void startShockWave();
    void sendMsgShockWaveToNearEnemy();
    bool isHitCylinder(HitSensor* pSender, HitSensor* pReceiver) const;

    bool isPlayerInCameraStartRange() const;

    /* 0x8C */ ActorCameraInfo* mCameraInfo;
};
