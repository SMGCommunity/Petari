#include "Game/Enemy/EyeBeamer.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/VolumeModelDrawer.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "revolution/mtx.h"

EyeBeamer::EyeBeamer(const char* pName) : LiveActor(pName),
    mRailMover(nullptr),
    mBeamMdl(nullptr), 
    mBeamBloom(nullptr), 
    mBeamVolumeDrawer(nullptr), 
    _CC(0, 0, 0, 1), 
    _DC(0, 0, 0, 1),
    _EC(0, 0, 0),
    _F8(0, 0, 0),
    _104(0, 0, 0),
    _140(0, 0, 0),
    _15C(2000.0f),
    _160(-1.0f),
    mIsInMercatorCube(0) {
    mWaterSurfaceMtx.identity();
    _9C.identity();
}

void EyeBeamer::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mIsInMercatorCube = MR::isInAreaObj("MercatorCube", mPosition);
    initModel();
    MR::makeQuatFromRotate(&_DC, this);
    _CC.x = _DC.x;
    _CC.y = _DC.y;
    _CC.z = _DC.z;
    _CC.w = _DC.w;
    _EC.set(mPosition);
    MR::getJMapInfoArg0NoInit(rIter, &_15C);
    MR::getJMapInfoArg1NoInit(rIter, &_160);
    initHitSensor(2);
    TVec3f stack_14(0.0f, 0.0f, 0.0f);
    MR::addHitSensorMapObj(this, "body", 8, 0.0f, stack_14);
    f32 f = _15C;
    f32 half_15C = 0.5*f;
    MR::addHitSensorEye(this, "beam", 8, 140.0f+(-f*0.5f), TVec3f(0.0f, half_15C, 0.0f));
    initSound(4, 0);
    MR::initCollisionParts(this, "EyeBeamer", getSensor("body"), 0);
    initRailMoveFunction(rIter);
    _F8.set(_EC);
    MR::calcRailStartPos(&_104, this);
    initEffectKeeper(0, 0, 0);
    MR::setEffectHostMtx(this, "BeamSurface", mWaterSurfaceMtx);
    initStartNerve(rIter);

    if (MR::useStageSwitchReadA(this, rIter))
        MR::listenStageSwitchOnA(this, MR::Functor(this, &EyeBeamer::requestStartPatrol));

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvEyeBeamer::EyeBeamerNrvDemoTurn::sInstance, "アイビーマー回転");
        MR::registerDemoActionNerve(this, &NrvEyeBeamer::EyeBeamerNrvDemoGotoPatrol::sInstance, "アイビーマー回転");
        setNerve(&NrvEyeBeamer::EyeBeamerNrvDemoStartWait::sInstance);
    }
    MR::setGroupClipping(this, rIter, 0x20);
    MR::startBck(this, "EyeBeamer", 0);
    makeActorAppeared();
}

void EyeBeamer::initStartNerve(const JMapInfoIter& rIter) {
    s32 arg = -1;
    MR::getJMapInfoArg2WithInit(rIter, &arg);

    switch (arg) {
        case -1:
            initNerve(&NrvEyeBeamer::EyeBeamerNrvWait::sInstance);
        break;
        case 0:
            initNerve(&NrvEyeBeamer::EyeBeamerNrvPatrol::sInstance);
        break;
        default:
            initNerve(&NrvEyeBeamer::EyeBeamerNrvWait::sInstance);
        break;
    }
}

void EyeBeamer::initModel() {
    initModelManagerWithAnm("EyeBeamer", 0, false);
    mBeamVolumeDrawer = new VolumeModelDrawer("ビームボリューム", "EyeBeamerBeamVolume", _9C);
    mBeamBloom = new ModelObj("ビームブルーム", "EyeBeamerBeamBloom", _9C, 0x1E, -2, -2, false);
    mBeamBloom->initWithoutIter();
    MR::invalidateClipping(mBeamBloom);
    mBeamMdl = new ModelObj("ビーム", "EyeBeamerBeam", _9C, 0x19, -2, -2, false);
    mBeamMdl->initWithoutIter();
    MR::invalidateClipping(mBeamMdl);
    MR::startBtk(mBeamMdl, "EyeBeamerBeam");
    MR::registerDemoSimpleCastAll(mBeamBloom);
    MR::registerDemoSimpleCastAll(mBeamMdl);
    MR::connectToScene(this, 0x22, 5, 0xA, 0x1C);
    TVec3f f(mGravity-mPosition);
    OSReport("", f.x);
}

void EyeBeamer::initAfterPlacement() {
    updatePoseAndTrans();
    TVec3f stack_80;
    MR::calcUpVec(&stack_80, this);
    stack_80.negate();
    TVec3f stack_50(stack_80*300.0f);
    TVec3f stack_74;
    stack_74.set(mPosition-stack_50);
    TVec3f stack_44(stack_80*(300.0f+_15C));
    TVec3f stack_68;
    f32 dist;
    if (MR::getFirstPolyOnLineToMapExceptActor(&stack_68, 0, stack_74, stack_44, this)) {
        dist = PSVECDistance(&stack_68, &mPosition);
        TVec3f stack_2C(stack_68+mPosition);
        _140.set(stack_2C*0.5);
        dist = 300.0f+0.5f*dist;
    }
    else {
        TVec3f stack_14((stack_80*_15C)*0.5f);
        _140.set(mPosition+stack_14);
        dist = 300.0f+(0.5f*_15C);
    }

    MR::setClippingTypeSphere(this, dist, &_140);
}

namespace NrvEyeBeamer {
    EyeBeamerNrvDemoStartWait(EyeBeamerNrvDemoStartWait::sInstance);
    EyeBeamerNrvDemoWait(EyeBeamerNrvDemoWait::sInstance);
    EyeBeamerNrvDemoTurn(EyeBeamerNrvDemoTurn::sInstance);
    EyeBeamerNrvDemoGotoPatrol(EyeBeamerNrvDemoGotoPatrol::sInstance);
    EyeBeamerNrvWait(EyeBeamerNrvWait::sInstance);
    EyeBeamerNrvTurn(EyeBeamerNrvTurn::sInstance);
    EyeBeamerNrvGotoPatrol(EyeBeamerNrvGotoPatrol::sInstance);
    EyeBeamerNrvPatrol(EyeBeamerNrvPatrol::sInstance);
};
