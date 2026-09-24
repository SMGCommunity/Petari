#include "Game/MapObj/LavaFloater.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/FloaterFloatingForceTypeNormal.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "math_types.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

namespace {
    const char* const cEffectName = "LavaBubble";
};  // namespace

namespace NrvLavaFloater {
    NEW_NERVE(HostTypeFloat, LavaFloater, Float);
    NEW_NERVE(HostTypeSink, LavaFloater, Sink);
};  // namespace NrvLavaFloater

LavaFloater::LavaFloater(const char* pName)
    : LiveActor(pName), mFloatingForce(), mMtxSetter(), mLodCtrlPlanet(), mObjectName(), _9C(gZeroVec), _A8() {
}

LavaFloater::~LavaFloater() {
}

void LavaFloater::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getObjectName(&mObjectName, rIter);

    const char* pName = mObjectName;
    const char* const names[] = {"LavaHomeFloaterB",  "LavaFloatingStepA", "LavaFloatingStepB",
                                 "LavaFloatingStepC", "LavaFloatingStepD", "FloaterOtakingFloaterA"};

    if (MR::isExistString(pName, names, ARRAY_SIZE(names))) {
        initModelManagerWithAnm(mObjectName, nullptr, true);
        mMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
        MR::newDifferedDLBuffer(this);
    } else {
        initModelManagerWithAnm(mObjectName, nullptr, false);
    }

    initEffectKeeper(nullptr, nullptr, false);
    MR::setEffectHostSRT(this, ::cEffectName, &_9C, &mRotation, nullptr);
    initSound(4, false);
    initHitSensor(1);
    MR::initCollisionParts(this, mObjectName, MR::addBodyMessageSensorMapObj(this), nullptr);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::connectToSceneCollisionMapObj(this);

    if (MR::isEqualString("LavaFloatingStepA", mObjectName)) {
        mLodCtrlPlanet = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    }

    initNerve(GET_NERVE(LavaFloater, HostTypeFloat));
    MR::onCalcGravity(this);
    const char* as = mObjectName;
    mFloatingForce = new FloaterFloatingForceTypeNormal(this, as);
    mFloatingForce->init(rIter);
    mFloatingForce->start();
    makeActorAppeared();
}

void LavaFloater::initAfterPlacement() {
    TVec3f v6(mGravity);
    v6 *= 1000.0f;

    if (!MR::getFirstPolyOnLineToMap(&_9C, nullptr, mPosition, v6)) {
        MR::calcMapGroundUpper(&_9C, this);
    }

    _A8 = mPosition.distance(_9C);

    if ((mPosition - _9C).dot(mGravity) < 0.0f) {
        _A8 *= -1.0f;
    }

    if (mMtxSetter != nullptr) {
        TVec3f uselessVec(0.0f, _A8, 0.0f);
        mMtxSetter->updateMtxUseBaseMtxWithLocalOffset(TVec3f(0.0f, _A8, 0.0f));
    }

    MR::offCalcGravity(this);
}

void LavaFloater::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (mLodCtrlPlanet != nullptr) {
        mLodCtrlPlanet->validate();
    }
}

void LavaFloater::makeActorDead() {
    if (mLodCtrlPlanet != nullptr) {
        mLodCtrlPlanet->invalidate();
    }

    LiveActor::makeActorDead();
}

void LavaFloater::control() {
    mFloatingForce->movement();
    mFloatingForce->updateHostTrans(&mPosition);
    mFloatingForce->updateHostVelocity(&mVelocity);

    if (mLodCtrlPlanet != nullptr) {
        mLodCtrlPlanet->update();
    }
}

void LavaFloater::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void LavaFloater::endClipped() {
    LiveActor::endClipped();

    if (isNerve(GET_NERVE(LavaFloater, HostTypeSink))) {
        MR::emitEffect(this, ::cEffectName);
    }
}

f32 LavaFloater::getCurrentSinkDepth() const {
    TPos3f depthMtx;
    TVec3f v1, v2;
    depthMtx.set(getBaseMtx());
    depthMtx.getTrans(v1);
    depthMtx.getYDir(v2);
    f32 sign = MR::sign(v2.dot(_9C - v1));
    return sign * _9C.distance(v1);
}

void LavaFloater::exeFloat() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffect(this, ::cEffectName);
    }

    if (_A8 < getCurrentSinkDepth()) {
        setNerve(GET_NERVE(LavaFloater, HostTypeSink));
    }
}

void LavaFloater::exeSink() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, ::cEffectName);
    }

    if (MR::isNearZero(getCurrentSinkDepth() - _A8, 1.0f)) {
        setNerve(GET_NERVE(LavaFloater, HostTypeFloat));
    }
}

void LavaFloater_FORCE_MATCH(const FloaterFloatingForce* pForce, TVec3f* pTranslation) {
    pForce->FloaterFloatingForce::updateHostTrans(pTranslation);
}
