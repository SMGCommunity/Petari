#include "Game/Player/TornadoMario.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

TornadoMario::TornadoMario(const char* pName) : LiveActor(pName), mUp(0.0f, 1.0f, 0.0f) {
}

void TornadoMario::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm(mName, nullptr, true);
    initSound(4, false);
    MR::connectToScene(this, MR::MovementType_Player, MR::CalcAnimType_Player, MR::DrawBufferType_PlayerDecoration, -1);
    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);
    MR::initJointTransform(this);
    MR::initShadowVolumeCylinder(this, 80.0f);
    MR::setShadowDropLength(this, nullptr, 3000.0f);
    MR::onShadowVolumeCutDropLength(this, nullptr);
    MR::setShadowVolumeEndDropOffset(this, nullptr, 50.0f);
    MR::onCalcShadow(this, nullptr);
    MR::invalidateClipping(this);
    appear();
    mIsVisible = false;
    MR::hideModel(this);
}

void TornadoMario::hideForce() {
    if (mIsVisible) {
        mIsVisible = false;
        MR::startBck(this, "MarioTornadoStart", nullptr);
        MR::hideModel(this);
    }
}

void TornadoMario::calcAnim() {
    LiveActor::calcAnim();
}

void TornadoMario::setTrHeight(const TVec3f& rPos, const TVec3f& rFront, const TVec3f& rUp, const TVec3f& rShadowDir) {
    mPosition = rPos;
    mUp = rUp;
    mFront = rFront;
    MR::setShadowDropDirection(this, nullptr, rShadowDir);
}

void TornadoMario::calcAndSetBaseMtx() {
    TPos3f pos;
    MR::makeMtxUpFrontPos(&pos, mUp, mFront, mPosition);
    MR::setBaseTRMtx(this, pos);
}

TornadoMario::~TornadoMario() {
}

void TornadoMario::show() {
    if (!mIsVisible) {
        mIsVisible = true;
        MR::startBck(this, "MarioTornadoStart", nullptr);
        MR::startBtk(this, "MarioTornadoStart");
        _8E = false;
        MR::showModel(this);
        _8C = true;
    }
}

void TornadoMario::hide() {
    if (mIsVisible) {
        mIsVisible = false;
        MR::startBck(this, "MarioTornadoEnd", nullptr);
        MR::startBtk(this, "MarioTornadoEnd");
    }
}

void TornadoMario::control() {
    if (mIsVisible) {
        if (MR::isBckOneTimeAndStopped(this)) {
            MR::startBck(this, "MarioTornadoLoop", nullptr);
            MR::startBck(this, "MarioTornadoLoop", nullptr);
        }
        MR::startLevelSound(this, "SE_PM_LV_TORNADE_FLYING");
    } else {
        if (MR::isBckOneTimeAndStopped(this)) {
            MR::hideModel(this);
        }
    }
}
