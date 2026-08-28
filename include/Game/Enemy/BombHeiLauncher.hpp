#pragma once

#include "Game/Enemy/BombHei.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class MapObjConnector;

class BombHeiLauncher : public LiveActor {
public:
    BombHeiLauncher(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void startCountdown();
    void onSwitchA();
    void offSwitchA();

    void exeLauncherWait();
    void exeStop();

    TVec3f* getBombVelocity() const {
        return &mBomb->mVelocity;
    }

    /* 0x8C */ BombHei* mBomb;
    /* 0x90 */ s32 mCountdown;
    /* 0x94 */ MapObjConnector* mConnector;
    /* 0x98 */ bool mIsActive;
};
