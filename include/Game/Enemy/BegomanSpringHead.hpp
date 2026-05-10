#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class BegomanHead : public PartsModel {
public:
    BegomanHead(LiveActor*, const char*, const char*, MtxPtr, int, bool);
    virtual ~BegomanHead();
    virtual void calcAndSetBaseMtx();

private:
    TVec3f* _9C;
};

class BegomanSpringHead : public BegomanHead {
public:
    BegomanSpringHead(LiveActor*, MtxPtr);
    virtual ~BegomanSpringHead();
    virtual void init(const JMapInfoIter&);

    bool isSpringHop();
    void getHopEndBckFrameMax();
    void tryHopStart();
    void tryHopEnd();
    void tryHopJump();
    void forceWaitImmediately();
    void exeWait();
    void exeHopStart();
    void exeHopWait();
    void exeHopEnd();
    void exeHopJump();

private:
    TVec3f* _9C;
};
