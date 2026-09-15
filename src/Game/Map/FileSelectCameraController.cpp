#include "Game/Map/FileSelectCameraController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

void FileSelectCameraController_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    // const Vec cTitleTargetPos = {};
    // const Vec cTitlePos = {};
    const Vec cFarTarget = {0.0f, 800.0f, 0.0f};
    const Vec cFarPoint = {0.0f, 0.0f, 15000.0f};
    const Vec cNearTargetOffset = {0.0f, 1100.0f, 0.0f};
    const Vec cNearPointOffset = {0.0f, 0.0f, 4800.0f};
    const f32 cTitleFovy = 60.0f;
    const f32 cFarFovy = 40.0f;
    const f32 cNearFovy = 50.0f;
};  // namespace

namespace {
    NEW_NERVE(FileSelectCameraControllerNrvTitle, FileSelectCameraController, Title);
    NEW_NERVE(FileSelectCameraControllerNrvMoveToFarPoint, FileSelectCameraController, MoveToFarPoint);
    NEW_NERVE(FileSelectCameraControllerNrvFarPoint, FileSelectCameraController, FarPoint);
    NEW_NERVE(FileSelectCameraControllerNrvMoveToNearPoint, FileSelectCameraController, MoveToNearPoint);
    NEW_NERVE(FileSelectCameraControllerNrvNearPoint, FileSelectCameraController, NearPoint);
};  // namespace

FileSelectCameraController::FileSelectCameraController(const char* pName)
    : LiveActor(pName), mWPoint(0.0f, 0.0f, 0.0f), mPrevWPoint(0.0f, 0.0f, 0.0f), mFovy(::cTitleFovy), mPrevFovy(::cTitleFovy),
      mUpVec(0.0f, 1.0f, 0.0f) {
}

void FileSelectCameraController::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::invalidateClipping(this);
    MR::initActorCameraProgrammable(this);
    initNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvTitle));
    makeActorDead();
}

void FileSelectCameraController::appear() {
    LiveActor::appear();
    setNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvTitle));
    MR::startActorCameraProgrammable(this, -1);
}

void FileSelectCameraController::kill() {
    LiveActor::kill();
    MR::endActorCameraProgrammable(this, -1, true);
}

void FileSelectCameraController::goToFarPoint() {
    setNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvMoveToFarPoint));
}

void FileSelectCameraController::goToNearPoint(const TVec3f& rPoint) {
    mNearTarget.set(rPoint + TVec3f(::cNearTargetOffset));

    setNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvMoveToNearPoint));
}

bool FileSelectCameraController::isAtFarPoint() const {
    return isNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvFarPoint));
}

bool FileSelectCameraController::isAtNearPoint() const {
    return isNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvNearPoint));
}

bool FileSelectCameraController::isToOrAtFarPoint() const {
    return isNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvMoveToFarPoint)) || isNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvFarPoint));
}

bool FileSelectCameraController::isToOrAtNearPoint() const {
    return isNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvMoveToNearPoint)) || isNerve(GET_NERVE_ANON(FileSelectCameraControllerNrvNearPoint));
}

void FileSelectCameraController::exeTitle() {
    if (MR::isFirstStep(this)) {
        mWPoint.set< f32 >(::cFarTarget.x, ::cFarTarget.y + 15000.0f, ::cFarTarget.z);
        mPosition.set< f32 >(::cFarPoint.x, ::cFarPoint.y + 15000.0f, ::cFarPoint.z);
        mUpVec.set(0.0f, 1.0f, 0.0f);
    }
}

void FileSelectCameraController::exeMoveToFarPoint() {
    if (MR::isFirstStep(this)) {
        mUpVec.set(0.0f, 1.0f, 0.0f);
    }

    f32 squaredTime = getNerveStep() / 60.0f;

    squaredTime *= squaredTime;
    mWPoint += (TVec3f(::cFarTarget) - mWPoint) * squaredTime;
    mFovy += (::cFarFovy - mFovy) * squaredTime;
    mPosition.add((TVec3f(::cFarPoint) - mPosition) * squaredTime);

    MR::setNerveAtStep(this, GET_NERVE_ANON(FileSelectCameraControllerNrvFarPoint), 60);
}

void FileSelectCameraController::exeFarPoint() {
    mWPoint = ::cFarTarget;
    mFovy = ::cFarFovy;
    mPosition.set(::cFarPoint);
}

void FileSelectCameraController::exeMoveToNearPoint() {
    TVec3f nearPoint = mNearTarget + ::cNearPointOffset;
    f32 squaredTime = getNerveStep() / 60.0f;

    squaredTime *= squaredTime;
    mWPoint += (mNearTarget - mWPoint) * squaredTime;
    mFovy += (::cNearFovy - mFovy) * squaredTime;
    mPosition += (nearPoint - mPosition) * squaredTime;

    MR::setNerveAtStep(this, GET_NERVE_ANON(FileSelectCameraControllerNrvNearPoint), 60);
}

void FileSelectCameraController::exeNearPoint() {
    TVec3f nearPoint = mNearTarget + ::cNearPointOffset;

    mFovy = ::cNearFovy;
    mPosition.set(nearPoint);
}

void FileSelectCameraController::control() {
    MR::setProgrammableCameraParam(this, mPrevWPoint, mPosition, mUpVec);
    MR::setProgrammableCameraParamFovy(this, mPrevFovy);

    mPrevWPoint = mWPoint;
    mPrevFovy = mFovy;
}
