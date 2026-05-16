#pragma once

#include "Game/NPC/PenguinRacer.hpp"

class PenguinRacerLeader : public PenguinRacer {
public:
    PenguinRacerLeader(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void resetRacer(const RaceManager* pRaceManager);
    virtual void exitRacer();

    bool branchFunc(u32);
    bool eventFunc(u32);

    void exeWait();
    void exePre();
    void exeReady();
    void exePost();
    void exeTalk();
    void exeTakeOutStar();

    /* 0x190 */ TakeOutStar* mTakeOutStar;
    /* 0x194 */ ActorCameraInfo* mCameraInfo;
};
