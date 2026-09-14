#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class BegomanHead : public PartsModel {
public:
    BegomanHead(LiveActor* pHost, const char* pName, const char* pModelName, MtxPtr pMtx, int drawBufferType, bool useHostLight)
        : PartsModel(pHost, pName, pModelName, pMtx, drawBufferType, useHostLight), _9C() {
    }

    virtual ~BegomanHead();
    virtual void calcAndSetBaseMtx();

    /* 0x9C */ TVec3f* _9C;
};

class BegomanSpringHead : public BegomanHead {
public:
    BegomanSpringHead(LiveActor* pActor, MtxPtr pMtx);

    virtual ~BegomanSpringHead();
    virtual void init(const JMapInfoIter& rIter);

    bool isSpringHop();
    s16 getHopEndBckFrameMax();
    void tryHopStart();
    void tryHopEnd();
    void tryHopJump();
    void forceWaitImmediately();
    void exeWait();
    void exeHopStart();
    void exeHopWait();
    void exeHopEnd();
    void exeHopJump();
};
