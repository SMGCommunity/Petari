#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class FileSelectSky : public LiveActor {
public:
    FileSelectSky(const char*);

    virtual ~FileSelectSky();
    virtual void init(const JMapInfoIter&);
    virtual void calcAnim();
    virtual void calcAndSetBaseMtx();
    virtual bool receiveOtherMsg(u32, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();

    f32 _8C; 
    f32 _90;
    TPos3f _94;
    ProjmapEffectMtxSetter* mProjmapEffectMtxSetter; // _C4  
};
