#include "Game/Enemy/EyeBeamer.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/VolumeModelDrawer.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include "revolution/mtx.h"

EyeBeamer::EyeBeamer(const char* pName)
    : LiveActor(pName),
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
      mIsInMercatorCube(false) {
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
    MR::addHitSensorMapObj(this, "body", 8, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorEye(this, "beam", 8, 140.0f + (0.5f * _15C), TVec3f(0.0f, 0.5f * (-_15C), 0.0f));
    initSound(4, 0);
    MR::initCollisionParts(this, "EyeBeamer", getSensor("body"), 0);
    initRailMoveFunction(rIter);
    _F8.set(_EC);
    MR::calcRailStartPos(&_104, this);
    initEffectKeeper(0, 0, 0);
    MR::setEffectHostMtx(this, "BeamSurface", mWaterSurfaceMtx);
    initStartNerve(rIter);

    if (MR::useStageSwitchReadA(this, rIter))
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &EyeBeamer::requestStartPatrol));

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvEyeBeamer::EyeBeamerNrvDemoTurn::sInstance, "アイビーマー回転");
        MR::registerDemoActionNerve(this, &NrvEyeBeamer::EyeBeamerNrvDemoGotoPatrol::sInstance, "アイビーマー降下");
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
}

void EyeBeamer::initAfterPlacement() {
    updatePoseAndTrans();
    TVec3f stack_80;
    MR::calcUpVec(&stack_80, this);
    stack_80.negate();
    TVec3f stack_74;
    stack_74.set(mPosition - (stack_80 * 300.0f));
    TVec3f stack_68;
    f32    dist;
    if (MR::getFirstPolyOnLineToMapExceptActor(&stack_68, 0, stack_74, stack_80 * (300.0f + _15C), this)) {
        dist = PSVECDistance(&stack_68, &mPosition);
        _140.set((stack_68 + mPosition) * 0.5);
        dist = 300.0f + 0.5f * dist;
    } else {
        _140.set(mPosition + ((stack_80 * _15C) * 0.5f));
        dist = 300.0f + (0.5f * _15C);
    }

    MR::setClippingTypeSphere(this, dist, &_140);
}

void EyeBeamer::initRailMoveFunction(const JMapInfoIter& rIter) {
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mRailMover->start();
        MR::moveCoordToStartPos(this);
    }
}

void EyeBeamer::updatePoseAndTrans() {
    TPos3f stack_38;
    stack_38.identity();
    stack_38.setQuat(_DC);
    if (mIsInMercatorCube) {
        TPos3f stack_8;
        stack_8.identity();
        MR::convertMercatorPlaneToSphereTransAndRotate(&mPosition, &stack_8, _EC, true);
        stack_38.concat(stack_8, stack_38);
    } else {
        mPosition.set(_EC);
    }
    stack_38.setTrans(mPosition);
    MR::setBaseTRMtx(this, stack_38);
    MR::setBaseScale(this, mScale);
}

void EyeBeamer::updateWaterSurfaceMtx() {
    if (_160 < 0.0f)
        return;

    mWaterSurfaceMtx.setQuat(_DC);
    TVec3f stack_38;
    f32    f3 = mWaterSurfaceMtx[2][1];
    f32    f2 = mWaterSurfaceMtx[1][1];
    f32    f1 = mWaterSurfaceMtx[0][1];
    stack_38.set< f32 >(f1, f2, f3);
    TVec3f stack_2C;
    TVec3f stack_20(_EC - (stack_38 * (_160 + (stack_38).dot(_EC - _104))));
    stack_2C.set(stack_20);

    if (mIsInMercatorCube) {
        TPos3f stack_44;
        stack_44.identity();
        MR::convertMercatorPlaneToSphereTransAndRotate(&stack_2C, &stack_44, stack_2C, true);
        mWaterSurfaceMtx.concat(stack_44, mWaterSurfaceMtx);
    }
    mWaterSurfaceMtx.setTrans(stack_2C);
}

void EyeBeamer::control() {
    updatePoseAndTrans();
    updateWaterSurfaceMtx();
    MR::startLevelSound(this, "SE_OJ_LV_EYE_BEAMER_MOVE", -1, -1, -1);
}

void EyeBeamer::startClipped() {
    LiveActor::startClipped();
    mBeamBloom->startClipped();
    mBeamMdl->startClipped();
}

void EyeBeamer::endClipped() {
    LiveActor::endClipped();
    mBeamBloom->endClipped();
    mBeamMdl->endClipped();
}

void EyeBeamer::draw() const {
    mBeamVolumeDrawer->draw();
}

void EyeBeamer::calcAnim() {
    LiveActor::calcAnim();
    MtxPtr mtx = MR::getJointMtx(this, "Beam");
    _9C.setInline(mtx);
    f32 one = 1.0f;
    f32 temp = _15C / 2000.0f;
    MR::preScaleMtx(_9C, TVec3f(one, temp, one));
}

void EyeBeamer::calcAndSetBaseMtx(){};

void EyeBeamer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isOnBeam() && MR::isSensorPlayer(pReceiver) && isInBeamRange(*MR::getPlayerPos())) {
        MR::sendMsgEnemyAttackHeatBeam(pReceiver, pSender);
    }
}

void EyeBeamer::requestStartPatrol() {
    if (isNerve(&NrvEyeBeamer::EyeBeamerNrvWait::sInstance)) {
        setNerve(&NrvEyeBeamer::EyeBeamerNrvTurn::sInstance);
        MR::invalidateClipping(this);
    }
}

bool EyeBeamer::tryGotoPatrol() {
    if (MR::isGreaterStep(this, 0x12C)) {
        setNerve(&NrvEyeBeamer::EyeBeamerNrvGotoPatrol::sInstance);
        return true;
    }
    return false;
}

bool EyeBeamer::tryPatrol() {
    if (MR::isGreaterStep(this, 0x1F4)) {
        setNerve(&NrvEyeBeamer::EyeBeamerNrvPatrol::sInstance);
        return true;
    }
    return false;
}

void EyeBeamer::exeDemoTurn() {
    if (MR::isFirstStep(this))
        MR::startSound(this, "SE_OJ_EYE_BEAMER_TURN", -1, -1);

    f32     rate = MR::calcDemoPartStepRate("アイビーマー回転");
    f32     easeIn = MR::getEaseInOutValue(rate, 0.0f, 1.0f, 1.0f);
    TQuat4f stack_30;
    TVec3f  stack_24;
    _CC.getXDir(stack_24);
    stack_30.setRotate(stack_24, PI * easeIn);
    PSQUATMultiply((Quaternion*)&stack_30, (Quaternion*)&_CC, (Quaternion*)&_DC);

    if (MR::isDemoPartLastStep("アイビーマー回転")) {
        TQuat4f stack_14;
        TVec3f  stack_8;
        _CC.getXDir(stack_8);
        stack_14.setRotate(stack_8, PI);
        PSQUATMultiply((Quaternion*)&stack_14, (Quaternion*)&_CC, (Quaternion*)&_DC);
        setNerve(&NrvEyeBeamer::EyeBeamerNrvDemoWait::sInstance);
    }
}

void EyeBeamer::exeDemoGotoPatrol() {
    f32 rate = MR::calcDemoPartStepRate("アイビーマー降下");
    f32 easeInOut = MR::getEaseInOutValue(rate, 0.0f, 1.0f, 1.0f);
    MR::vecBlend(_F8, _104, &_EC, easeInOut);
    if (MR::isDemoPartLastStep("アイビーマー降下"))
        setNerve(&NrvEyeBeamer::EyeBeamerNrvDemoWait::sInstance);
}

void EyeBeamer::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_EYE_BEAMER_TURN", -1, -1);
    }
    f32     easeInOut = MR::calcNerveEaseInOutRate(this, 0x12C);
    TQuat4f stack_14;
    TVec3f  stack_8;
    _CC.getXDir(stack_8);
    stack_14.setRotate(stack_8, PI * easeInOut);
    PSQUATMultiply((Quaternion*)&stack_14, (Quaternion*)&_CC, (Quaternion*)&_DC);

    if (tryGotoPatrol())
        return;
}

void EyeBeamer::exeGotoPatrol() {
    f32 easeInOut = MR::calcNerveEaseInOutRate(this, 0x1F4);
    MR::vecBlend(_F8, _104, &_EC, easeInOut);

    if (tryPatrol())
        return;
}

void EyeBeamer::exePatrol() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        TVec3f  stack_48;
        TQuat4f stack_38;
        _CC.getXDir(stack_48);
        stack_38.setRotate(stack_48, PI);
        PSQUATMultiply((Quaternion*)&stack_38, (Quaternion*)&_CC, (Quaternion*)&_DC);
        if (_160 > 0)
            MR::emitEffect(this, "BeamSurface");
    }
    TVec3f stack_2C;
    MR::calcUpVec(&stack_2C, this);
    stack_2C.negate();
    _140.set(mPosition + ((stack_2C * _15C) * 0.5));
    MR::setClippingTypeSphere(this, 300.0f + (0.5f * _15C), &_140);
    mRailMover->movement();
    _EC.set(mRailMover->_28);
}

bool EyeBeamer::isInBeamRange(const TVec3f& rVec) const {
    TVec3f stack_44;
    MR::calcUpVec(&stack_44, this);
    stack_44.negate();
    TVec3f stack_38;
    f32    f3 = _9C[2][3];
    f32    f2 = _9C[1][3];
    f32    f1 = _9C[0][3];
    stack_38.set< f32 >(f1, f2, f3);
    f32 dot = stack_44.dot(rVec - stack_38);

    if (dot < 0.0f || _15C < dot)
        return false;
    else {
        TVec3f stack_2C;
        stack_2C.set(stack_38 + (stack_44 * dot));
        f32 dist = PSVECDistance(&stack_2C, &rVec);

        if (dist < 140.0f)
            return true;
    }

    return false;
}

bool EyeBeamer::isOnBeam() const {
    if (isNerve(&NrvEyeBeamer::EyeBeamerNrvDemoWait::sInstance) || isNerve(&NrvEyeBeamer::EyeBeamerNrvWait::sInstance) || isNerve(&NrvEyeBeamer::EyeBeamerNrvPatrol::sInstance))
        return true;

    return false;
}

EyeBeamer::~EyeBeamer(){};

namespace NrvEyeBeamer {
    void EyeBeamerNrvDemoWait::execute(Spine* pSpine) const {
        EyeBeamer* pEyeBeamer = (EyeBeamer*)pSpine->mExecutor;
        if (MR::isDemoLastStep())
            pEyeBeamer->setNerve(&NrvEyeBeamer::EyeBeamerNrvPatrol::sInstance);
    }
    EyeBeamerNrvDemoStartWait(EyeBeamerNrvDemoStartWait::sInstance);
    EyeBeamerNrvDemoWait(EyeBeamerNrvDemoWait::sInstance);
    EyeBeamerNrvDemoTurn(EyeBeamerNrvDemoTurn::sInstance);
    EyeBeamerNrvDemoGotoPatrol(EyeBeamerNrvDemoGotoPatrol::sInstance);
    EyeBeamerNrvWait(EyeBeamerNrvWait::sInstance);
    EyeBeamerNrvTurn(EyeBeamerNrvTurn::sInstance);
    EyeBeamerNrvGotoPatrol(EyeBeamerNrvGotoPatrol::sInstance);
    EyeBeamerNrvPatrol(EyeBeamerNrvPatrol::sInstance);
}; // namespace NrvEyeBeamer
