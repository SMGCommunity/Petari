#include "Game/MapObj/RainCloud.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "revolution/types.h"

namespace NrvRainCloud {
    NEW_NERVE(RainCloudNrvAppear, RainCloud, Appear);
    NEW_NERVE(RainCloudNrvWait, RainCloud, Wait);
    NEW_NERVE(RainCloudNrvDisappear, RainCloud, Disappear);
    NEW_NERVE(RainCloudNrvEnd, RainCloud, End);
    NEW_NERVE(RainCloudNrvSoftTouch, RainCloud, SoftTouch);
    NEW_NERVE(RainCloudNrvHardTouch, RainCloud, HardTouch);
}; // namespace NrvRainCloud

namespace {
    static s32 sThunderStep = 0x8C;
}

RainCloud::RainCloud(const char* pName)
    : LiveActor(pName) {
    mCloudCylinder = nullptr;
    mRailMover = nullptr;
    _F4 = 0;
    _F8.x = 0.0f;
    _F8.y = 0.0f;
    _F8.z = 0.0f;
    _104 = 0.0f;
    _108 = -1.0f;
    mIsOnPlayer = false;
    _10D = false;
    _90.identity();
    _C0.identity();
}

void RainCloud::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("RainCloud", nullptr, false);
    MR::getJMapInfoArg3NoInit(rIter, &_10D);
    if (!_10D) {
        _90.set(getBaseMtx());
        mCloudCylinder = new ModelObj("雨雲雨範囲", "RainCloudCylinder", _90, -2, -2, -2, false);
        mCloudCylinder->initWithoutIter();
        MR::invalidateClipping(mCloudCylinder);
    }

    MR::connectToSceneCollisionMapObj(this);

    if (!_10D) {
        initHitSensor(2);
        MR::addBodyMessageSensorMapObj(this);
        MR::addHitSensorCallbackMapObjSimple(this, "drop_water", 4, 150.0f);
    } else {
        initHitSensor(1);
        MR::addBodyMessageSensorMapObj(this);
    }

    MR::initCollisionParts(this, "RainCloud", getSensor("body"), nullptr);
    initEffectKeeper(0, nullptr, false);
    if (!_10D) {
        MR::setEffectHostMtx(this, "Splash", _C0);
    }

    initSound(4, false);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
        mRailMover->start();
    }

    _F4 = MR::createLodCtrlPlanet(this, rIter, -1.0f, -1);
    MR::initShadowVolumeFlatModel(this, "RainCloudVolume", MR::getJointMtx(this, "Shadow"));
    MR::getJMapInfoArg0NoInit(rIter, &_108);
    if (_108 != -1.0f) {
        _104 = _108;
        MR::setShadowDropLength(this, nullptr, _108);
        MR::setShadowVolumeEndDropOffset(this, nullptr, 80.0f);
        MR::excludeCalcShadowToMyCollision(this, nullptr);
        MR::onShadowVolumeCutDropLength(this, nullptr);
        MR::onCalcShadow(this, nullptr);
    } else {
        _108 = 2000.0f;
        MR::invalidateShadow(this, nullptr);
    }

    TVec3f v13;
    v13.setPS(mPosition);
    f32 boundRadius;
    MR::calcModelBoundingRadius(&boundRadius, this);
    boundRadius += 0.5f * _104;
    if (mRailMover != nullptr) {
        f32 v11;
        MR::calcRailClippingInfo(&v13, &v11, this, 100.0f, 500.0f);
        boundRadius += v11;
    }

    JMAVECScaleAdd(&mGravity, &mPosition, &_F8, (0.5f * _104));
    MR::setClippingTypeSphere(this, boundRadius, &_F8);
    MR::setGroupClipping(this, rIter, 16);
    MR::joinToGroupArray(this, rIter, "雲集団", 16);
    MR::startBck(this, "Wait", nullptr);

    if (_10D) {
        MR::startBpk(this, "Fine");
        MR::startBpk(_F4->_14, "Fine");
    } else {
        MR::startBpk(this, "Rain");
        MR::startBpk(_F4->_14, "Rain");
    }

    initNerve(&NrvRainCloud::RainCloudNrvWait::sInstance);
    makeActorAppeared();
}

void RainCloud::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    bool arg3 = false;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);
    pCollector->addArchive("RainCloud");
    pCollector->addArchive("RainCloudVolume");
    pCollector->addArchive("RainCloudLow");

    if (!arg3) {
        pCollector->addArchive("RainCloudCylinder");
    }
}

void RainCloud::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
        MR::startBpk(this, "Appear");
        MR::validateCollisionParts(this);
        mCloudCylinder->appear();
        MR::startSound(this, "SE_OJ_RAIN_CLOUD_APPEAR", -1, -1);
    }

    if (MR::isStep(this, sThunderStep)) {
        MR::startSound(this, "SE_OJ_RAIN_CLOUD_THUNDER", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        MR::validateHitSensors(this);
        setNerve(&NrvRainCloud::RainCloudNrvWait::sInstance);
    }
}

void RainCloud::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
    }

    if (!MR::isOnPlayer(this) && mIsOnPlayer) {
        mIsOnPlayer = false;
        setNerve(&NrvRainCloud::RainCloudNrvSoftTouch::sInstance);
    }
}

void RainCloud::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Disappear", nullptr);
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
        MR::startSound(this, "SE_OJ_RAIN_CLOUD_DISAPPEAR", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        mCloudCylinder->kill();
        mIsOnPlayer = false;
    }

    if (MR::isMapPartsRailMovePassedStartPointRepeat(mRailMover)) {
        setNerve(&NrvRainCloud::RainCloudNrvEnd::sInstance);
    }
}

void RainCloud::exeEnd() {
    if (MR::isBckStopped(this) && isNextStartOK()) {
        if (mCloudCylinder != nullptr) {
            mCloudCylinder->kill();
        }

        setNerve(&NrvRainCloud::RainCloudNrvAppear::sInstance);
    }
}

void RainCloud::exeSoftTouch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "OnBee", nullptr);
    }

    if (MR::isBckStopped(this)) {
        mIsOnPlayer = MR::isOnPlayer(this);
        setNerve(&NrvRainCloud::RainCloudNrvWait::sInstance);
    }
}

void RainCloud::exeHardTouch() {
    if (MR::isFirstStep(this)) {
        MR::invalidateCollisionParts(this);
        MR::tryRumblePadMiddle(this, nullptr);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 0x3C)) {
        MR::validateCollisionParts(this);
        setNerve(&NrvRainCloud::RainCloudNrvWait::sInstance);
    }
}

void RainCloud::control() {
    _F4->update();

    if (mRailMover != nullptr && !isNerve(&NrvRainCloud::RainCloudNrvEnd::sInstance) && (mRailMover->movement(), mPosition.set(mRailMover->_28), MR::isMapPartsRailMovePassedEndPointRepeat(mRailMover))) {
        setNerve(&NrvRainCloud::RainCloudNrvDisappear::sInstance);
    } else {
        if (MR::isShadowProjected(this, nullptr)) {
            _104 = MR::getShadowProjectionLength(this, nullptr);
        } else {
            _104 = _108;
        }

        if (!_10D) {
            switchEffect();
            updateRainCylinder();
        }
    }
}

void RainCloud::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isPlayerElementModeBee()) {
            MR::curePlayerElementMode();
            MR::tryRumblePadStrong(this, 0);
        }

        MR::sendArbitraryMsg(ACTMES_PUDDLE_TOUCH_GROUND, pReceiver, pSender);
    }
}

void RainCloud::updateHitSensor(HitSensor* pSensor) {
    f32    radius = pSensor->mRadius;
    TVec3f v7;
    JMAVECScaleAdd(&mGravity, &mPosition, &v7, radius);
    TVec3f v6;
    JMAVECScaleAdd(&mGravity, &mPosition, &v6, (_104 - radius));
    MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), v7, v6);
}

void RainCloud::startClipped() {
    MR::deleteEffectAll(this);
    LiveActor::startClipped();

    if (mCloudCylinder != nullptr) {
        mCloudCylinder->startClipped();
    }
}

void RainCloud::endClipped() {
    LiveActor::endClipped();
    if (mCloudCylinder != nullptr) {
        mCloudCylinder->endClipped();
    }

    if (!_10D && !MR::isDemoActive()) {
        switchEffect();
    }
}

void RainCloud::switchEffect() {
    bool v2 = isNerve(&NrvRainCloud::RainCloudNrvAppear::sInstance) || isNerve(&NrvRainCloud::RainCloudNrvDisappear::sInstance) || isNerve(&NrvRainCloud::RainCloudNrvEnd::sInstance);

    if (v2) {
        if (MR::isEffectValid(this, "Splash")) {
            MR::deleteEffect(this, "Splash");
        }

        if (MR::isEffectValid(this, "Line20")) {
            MR::deleteEffect(this, "Line20");
        }

        if (MR::isEffectValid(this, "Line10")) {
            MR::deleteEffect(this, "Line10");
        }
    } else {
        bool v3;
        if (MR::isShadowProjected(this, nullptr)) {
            v3 = _104 > 1500.0f;
            TVec3f projectionPos;
            MR::getShadowProjectionPos(this, nullptr, &projectionPos);
            TVec3f projectionNormal;
            MR::getShadowProjectionNormal(this, nullptr, &projectionNormal);
            MR::makeMtxUpNoSupportPos(&_C0, projectionNormal, projectionPos);

            if (!MR::isEffectValid(this, "Splash")) {
                MR::emitEffect(this, "Splash");
            }
        } else {
            v3 = true;
            MR::deleteEffect(this, "Splash");
        }

        if (!v3 || MR::isEffectValid(this, "Line20")) {
            if (!v3 && !MR::isEffectValid(this, "Line10")) {
                MR::deleteEffect(this, "Line20");
                MR::emitEffect(this, "Line10");
            }
        } else {
            MR::deleteEffect(this, "Line10");
            MR::emitEffect(this, "Line20");
        }
    }
}

RainCloud::~RainCloud() {
}
