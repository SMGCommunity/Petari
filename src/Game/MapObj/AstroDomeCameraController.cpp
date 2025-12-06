#include "Game/MapObj/AstroDomeCameraController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace {
    Vec cDefaultUp;
    Vec cZoomInPos;
    Vec cZoomOutPos;
}  // namespace

namespace NrvAstroDomeCameraController {
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxySelectStart, AstroDomeCameraController, GalaxySelectStart);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxySelect, AstroDomeCameraController, GalaxySelect);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxyConfirmStart, AstroDomeCameraController, GalaxyConfirmStart);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxyConfirm, AstroDomeCameraController, GalaxyConfirm);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxyConfirmCancel, AstroDomeCameraController, GalaxyConfirmCancel);
}  // namespace NrvAstroDomeCameraController

AstroDomeCameraController::AstroDomeCameraController(const char* pName) : LiveActor(pName), _8C(gZeroVec), _104(0.0f), _108(gZeroVec) {}

void AstroDomeCameraController::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::initActorCameraProgrammable(this);
    initNerve(&NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxySelectStart::sInstance);
    SphereSelectorFunction::registerTarget(this);
    MR::registerDemoSimpleCastAll(this);
    makeActorDead();
}

void AstroDomeCameraController::appear() {
    LiveActor::appear();
    TVec3f pCamPos = MR::getCamPos();
    _8C.set< f32 >(pCamPos);
    _98.reset(_8C);
    _BC.reset(_108);
    _E0.reset(cDefaultUp);
    MR::startActorCameraProgrammable(this, SphereSelectorFunction::getSelectStartFrame());
    setNerve(&NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxySelectStart::sInstance);
}

void AstroDomeCameraController::kill() {
    MR::endActorCameraProgrammable(this, -1, true);
    LiveActor::kill();
}

void AstroDomeCameraController::control() {
    JMAVECLerp(&_98._0, &_98._C, &_98._18, _104);
    JMAVECLerp(&_BC._0, &_BC._C, &_BC._18, _104);
    JMAVECLerp(&_E0._0, &_E0._C, &_E0._18, _104);
    MR::setProgrammableCameraParam(this, _BC._18, _98._18, _E0._18);
    MR::setProgrammableCameraParamFovy(this, 60.0f);
}

bool AstroDomeCameraController::receiveOtherMsg(u32 v1, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::trySyncAppearMsgSelectStart(this, v1)) {
        return true;
    }
    if (SphereSelectorFunction::isMsgConfirmStart(v1)) {
        setNerve(&NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxyConfirmStart::sInstance);
        return true;
    }
    if (SphereSelectorFunction::isMsgConfirmCancel(v1)) {
        setNerve(&NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxyConfirmCancel::sInstance);
        return true;
    }
    return false;
}

void AstroDomeCameraController::calcZoomOutPos(TVec3f* v1) const {
    SphereSelectorFunction::calcOffsetPos(v1, gZeroVec, TVec3f(0.0f, 0.0f, -22000.0f), _8C.negateInline(), TVec3f(0.0f, 1.0f, 0.0f));
}

void AstroDomeCameraController::calcZoomInPos(TVec3f *v1, const TVec3f &v2) const {
    TVec3f zoomOutPos;
    calcZoomOutPos(&zoomOutPos);
    TVec3f *trans = &SphereSelectorFunction::getSelectedActorTrans();
    TVec3f x;
    JMathInlineVEC::PSVECSubtract2(trans, &zoomOutPos, &x);
    SphereSelectorFunction::calcOffsetPos(v1, SphereSelectorFunction::getSelectedActorTrans(), cZoomInPos, x, v2);
}


void AstroDomeCameraController::calcZoomInTarget(TVec3f* vec1, const TVec3f& vec2) const {
    TVec3f stack;
    stack.scale(200.0f, vec2);
    JMathInlineVEC::PSVECAdd3(&SphereSelectorFunction::getSelectedActorTrans(), &stack, vec1);
}

void AstroDomeCameraController::calcZoomInUp(TVec3f* vec) const {
    TPos3f pos;
    SphereSelectorFunction::calcHandledRotateMtx(TVec3f(0.0f, 0.0f, 0.0f), &pos);
    f32 v3 = pos[2][1];
    f32 v2 = pos[1][1];
    f32 v1 = pos[0][1];
    vec->set<f32>(v1, v2 ,v3);
}

void AstroDomeCameraController::exeGalaxySelectStart() {
    s32 STF = SphereSelectorFunction::getSelectStartFrame();
    TVec3f stack;
    if (MR::isFirstStep(this)) {
        calcZoomOutPos(&stack);
        _98._0.set< f32 >(_98._18);
        _98._C.set< f32 >(stack);
        _BC._0.set< f32 >(_BC._18);
        _BC._C.set< f32 >(_108);
    }
    _104 = MR::getEaseInOutValue(MR::calcNerveRate(this, STF), 0.0, 1.0, 1.0);
    MR::setNerveAtStep(this, &NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxySelect::sInstance, STF);
}

void AstroDomeCameraController::exeGalaxySelect() {
    if (MR::isFirstStep(this)) {
        _104 = 1.0f;
    }
    TVec3f vec;
    calcZoomOutPos(&vec);
    _98.reset(vec);
    _BC.reset(_108);
}

void AstroDomeCameraController::exeGalaxyConfirmStart() {
    TVec3f vecUp;
    TVec3f vecTar;
    TVec3f vecPos;
    s32 frame = SphereSelectorFunction::getConfirmStartCancelFrame();
    calcZoomInUp(&vecUp);
    calcZoomInTarget(&vecTar, vecUp);
    if (MR::isFirstStep(this)) {
        _BC._0.set(_BC._18);
        _BC._C.set(vecTar);
        calcZoomInPos(&vecPos, vecUp);
        _98._0.set(_98._18);
        _98._C.set(vecPos);
        _E0._0.set(_E0._18);
        _E0._C.set(vecUp);
    }
    _104 = MR::calcNerveEaseOutRate(this, frame);
    _BC._C.set(vecTar);
    MR::setNerveAtStep(this, &NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxyConfirm::sInstance, frame);
}

void AstroDomeCameraController::exeGalaxyConfirm() {
    TVec3f vecUp;
    TVec3f vecPos;
    TVec3f vecTar;
    calcZoomInUp(&vecUp);
    if (MR::isFirstStep(this)) {
        _104 = 1.0f;
    }
    calcZoomInPos(&vecPos, vecUp);
    _98.reset(vecPos);
    calcZoomInTarget(&vecTar, vecUp);
    _BC.reset(vecTar);
}

void AstroDomeCameraController::exeGalaxyConfirmCancel() {
    TVec3f vecUp;
    TVec3f vecPos;
    s32 frame = SphereSelectorFunction::getConfirmStartCancelFrame();
    if (MR::isFirstStep(this)) {
        calcZoomOutPos(&vecPos);
        _98._0.set(_98._18);
        _98._C.set(vecPos);
        _BC._0.set(_BC._18);
        _BC._C.set(_108);
        TVec3f vec = TVec3f(cDefaultUp);
        _E0._0.set(_E0._18);
        _E0._C.set(vec);
    }
    _104 = MR::calcNerveEaseInRate(this, frame);
    MR::setNerveAtStep(this, &NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxySelect::sInstance, frame);
}

AstroDomeCameraController::~AstroDomeCameraController() {};

AstroDomeCameraController::Position::Position() {
    _0.zero();
    _C.zero();
    _18.zero();
}

void AstroDomeCameraController::Position::reset(const TVec3f& vec) {
    _0.set(vec);
    _C.set(vec);
    _18.set(vec);
}
