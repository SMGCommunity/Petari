#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;

class TicoRail : public LiveActor {
public:
    /// @brief Creates a new `TicoRail`.
    /// @param pName A pointer to the null-terminated name of the object.
    TicoRail(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeLookAround();
    void exeMoveSign();
    void exeMove();
    void exeStop();
    void exeTalkStart();
    void exeTalk();
    void exeTalkCancel();
    void exeGoodBye();
    bool isGreaterEqualStepAndRandom(s32) const;
    bool isSameRailActor(const LiveActor*) const;

    /* 0x8C */ TVec3f _8C;
    /* 0x98 */ LiveActor* _98;
    /* 0x9C */ LodCtrl* mLodCtrl;
};
