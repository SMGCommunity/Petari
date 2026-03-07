#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Util/MultiEventCamera.hpp"
#include "Game/NameObj/NameObj.hpp"
class JMapInfoIter;

class JumpEmitter : public LiveActor {
public:
    JumpEmitter(const char*);

    virtual ~JumpEmitter();
    virtual void kill();

    void initEventCamera(const JMapInfoIter&);
    void startEventCamera();
    void updateEventCamera();
    void endEventCamera();
    void updateRotate();

    MtxPtr _8C;
    TMtx34f _90;
    PartsModel* mHeadModel;  // 0xC0
    TVec3f _C4;
    u8 _D0;
    ActorCameraInfo* mCameraInfo;        // 0xD4
    MultiEventCamera* mMultEventCamera;  // 0xD8
};

namespace MR {
    bool enableGroupAttack(LiveActor*, f32, f32);
};

class JumpGuarder : public NameObj {
public:
    JumpGuarder(const char*);
    virtual ~JumpGuarder();

private:
    u8 mPad[(0x104) - sizeof(NameObj)];
};
