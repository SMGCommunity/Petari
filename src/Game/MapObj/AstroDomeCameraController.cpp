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
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"

namespace {
    Vec cDefaultUp;
    Vec cZoomInPos;
    Vec cZoomOutPos;
} // namespace

namespace NrvAstroDomeCameraController {
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxySelectStart, AstroDomeCameraController, GalaxySelectStart);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxySelect, AstroDomeCameraController, GalaxySelect);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxyConfirmStart, AstroDomeCameraController, GalaxyConfirmStart);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxyConfirm, AstroDomeCameraController, GalaxyConfirm);
    NEW_NERVE(AstroDomeCameraControllerNrvGalaxyConfirmCancel, AstroDomeCameraController, GalaxyConfirmCancel);
} // namespace NrvAstroDomeCameraController

AstroDomeCameraController::AstroDomeCameraController(const char* pName)
    : LiveActor(pName), _8C(gZeroVec), _104(0.0f), _108(gZeroVec) {}

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
    _8C.set<f32>(pCamPos);
    _98.reset(_8C);
    _BC.reset(_108);
    _E0.set(cDefaultUp);
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

void AstroDomeCameraController::calcZoomInPos(TVec3f* v1, const TVec3f& v2) const {
    TVec3f zoomOutPos;
    calcZoomOutPos(&zoomOutPos);
    SphereSelectorFunction::calcOffsetPos(v1, SphereSelectorFunction::getSelectedActorTrans(), cZoomInPos, SphereSelectorFunction::getSelectedActorTrans() - zoomOutPos, v2);
}

void AstroDomeCameraController::exeGalaxySelectStart() {
    s32    STF = SphereSelectorFunction::getSelectStartFrame();
    TVec3f stack;
    if (MR::isFirstStep(this)) {
        calcZoomOutPos(&stack);
        _98._0.set<f32>(_98._18);
        _98._C.set<f32>(stack);
        _BC._0.set<f32>(_BC._18);
        _BC._C.set<f32>(_108);
    }
    _104 = MR::getEaseInOutValue(MR::calcNerveRate(this, STF), 0.0, 1.0, 1.0);
    MR::setNerveAtStep(this, &NrvAstroDomeCameraController::AstroDomeCameraControllerNrvGalaxySelect::sInstance, STF);
}
