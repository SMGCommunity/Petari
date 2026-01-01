#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;

class BlackHole : public LiveActor {
public:
    BlackHole(const char*);

    virtual ~BlackHole();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);

    bool tryStartDemoCamera();
    void initMapToolInfo(const JMapInfoIter&);
    void initModel();
    void initCubeBox();
    bool isInCubeBox(const TVec3f&) const;
    void updateModelScale(f32, f32);
    void exeWait();
    void exeDemo();
    void exeDisappear();

    ModelObj* mBlackHoleModel;  // 0x8C
    TVec3f _90;
    f32 _9C;
    f32 _A0;
    TBox3f* _A4;
    TMtx34f _A8;
    TMtx34f _D8;
    ActorCameraInfo* mCameraInfo;  // 0x108
};
