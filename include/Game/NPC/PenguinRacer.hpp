#pragma once

#include "Game/Map/RaceManager.hpp"
#include "Game/NPC/NPCActor.hpp"

class PenguinRacer : public NPCActor, public AbstractRacer {
public:
    PenguinRacer(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void control();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual void initRacer();
    virtual void prepRacer(const RaceManager* pRaceManager);
    virtual void startRacer();
    virtual bool updateRacer(const RaceManager* pRaceManager);
    virtual bool goalRacer();
    virtual void loseRacer();
    virtual void resetRacer(const RaceManager* pRaceManager);
    virtual void exitRacer();
    virtual const Nerve* getMoveNerve() const;
    virtual f32 getMoveSpeed() const;

    void initModel();
    void initSensor();
    void initShadow();
    void initBind();
    void initColor(const JMapInfoIter& rIter);
    void initEtc(const JMapInfoIter& rIter);

    bool tryReaction();

    void exeReaction();
    void exeWait();
    void exeRaceWait();
    void exeRaceWalk();
    void exeRaceSwim();
    void exeRaceStoW();
    void exeRaceWtoS();
    void exeRaceFall();
    void exeRaceGoal();
    void exeRaceTurn();

    void updateMove();
    void updateVelocity();
    void updateRail();

    bool isInRace() const;

    /* 0x180 */ s32 mRaceDisabled;
    /* 0x184 */ f32 mMoveSpeed;
    /* 0x188 */ f32 mSpeed;
    /* 0x18C */ s32 mRailPointNo;
};
