#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TBox.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>

class ActorCameraInfo;
class ModelObj;

class BlackHole : public LiveActor {
public:
    BlackHole(const char*);

    /* 0x08 */ virtual ~BlackHole();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);

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
