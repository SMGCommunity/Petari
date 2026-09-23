#include "Game/MapObj/MapObjActor.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/MapPartsRailGuideDrawer.hpp"
#include "Game/MapObj/MapPartsRailMover.hpp"
#include "Game/MapObj/MapPartsRailPosture.hpp"
#include "Game/MapObj/MapPartsRailRotator.hpp"
#include "Game/MapObj/MapPartsRotator.hpp"
#include "Game/MapObj/MapPartsSeesaw1AxisRotator.hpp"
#include "Game/MapObj/MapPartsSeesaw2AxisRotator.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util.hpp"
#include "Game/Util/FurMulti.hpp"

#include <cstdio>
#include <cstring>

void MapObjActor_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

namespace NrvMapObjActor {
    NEW_NERVE(HostTypeWait, MapObjActor, Wait);
    NEW_NERVE(HostTypeMove, MapObjActor, Move);
    NEW_NERVE(HostTypeDone, MapObjActor, Done);
};  // namespace NrvMapObjActor

namespace {
    const char* const cBrkNameColorChange = "ColorChange";
    const char* const cBtpNameTexChange = "TexChange";
    const char* const cBtkNameTexChange = "TexChange";
    const char* const cEffectNameAppear = "Appear";
    const char* const cBckNameMove = "Move";
    const char* const cFollowJointName = "Move";
};  // namespace

MapObjActor::MapObjActor(const char* pName)
    : LiveActor(pName), mObjectName(), mPlanetLodCtrl(), mBloomModel(), mModelObj(), mMatrixSetter(), mRailMover(), mRotator(), mRailRotator(),
      mRailPosture(), mRailGuideDrawer(), _B4(), _B5(), _B6(), mWaitNrv(GET_NERVE(MapObjActor, HostTypeWait)),
      mMoveNrv(GET_NERVE(MapObjActor, HostTypeMove)), mDoneNrv(GET_NERVE(MapObjActor, HostTypeDone)) {
}

MapObjActor::MapObjActor(const char* pName, const char* pObjName)
    : LiveActor(pName), mObjectName(pObjName), mPlanetLodCtrl(), mBloomModel(), mModelObj(), mMatrixSetter(), mRailMover(), mRotator(),
      mRailRotator(), mRailPosture(), mRailGuideDrawer(), _B4(), _B6(), mWaitNrv(GET_NERVE(MapObjActor, HostTypeWait)),
      mMoveNrv(GET_NERVE(MapObjActor, HostTypeMove)), mDoneNrv(GET_NERVE(MapObjActor, HostTypeDone)) {
}

void MapObjActor::init(const JMapInfoIter& rIter) {
    if (mObjectName != nullptr) {
        return;
    }

    MR::getObjectName(&mObjectName, rIter);
}

void MapObjActor::initAfterPlacement() {
    if (mMatrixSetter == nullptr) {
        return;
    }

    if (_B4) {
        return;
    }

    mMatrixSetter->updateMtxUseBaseMtx();
}

void MapObjActor::appear() {
    LiveActor::appear();

    if (mBloomModel != nullptr) {
        mBloomModel->appear();
    }

    if (MR::isExistEffectKeeper(this)) {
        const char* appearEffectName = ::cEffectNameAppear;
        if (MR::isRegisteredEffect(this, appearEffectName)) {
            MR::emitEffect(this, appearEffectName);
        }
    }

    if (!MR::isEqualString(mObjectName, "DarkHopperRotateStepA")) {
        const char* startSound = MR::StageEffect::getStartSe(mObjectName);

        if (startSound != nullptr) {
            MR::startSound(this, startSound);
        }
    }

    if (_B6) {
        MR::startSystemSE("SE_SY_READ_RIDDLE_S");
    }
}

void MapObjActor::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    if (mModelObj != nullptr) {
        mModelObj->kill();
    }

    if (mBloomModel != nullptr) {
        mBloomModel->kill();
    }

    LiveActor::kill();
}

void MapObjActor::initialize(const JMapInfoIter& rIter, const MapObjActorInitInfo& rInfo) {
    bool hasRail = MR::isConnectedWithRail(rIter);
    if (rInfo.mSetDefaultPosition) {
        MR::initDefaultPos(this, rIter);
    }

    bool useDisplayList = rInfo.mUseProjectMapMtx || rInfo.mUseMirrorReflection || rInfo.mDummyChangeTexture != nullptr;
    if (rInfo.mModelName != nullptr) {
        mObjectName = rInfo.mModelName;
    }

    initModelManagerWithAnm(mObjectName, nullptr, useDisplayList);

    if (rInfo.mDummyChangeTexture != nullptr) {
        MR::initDLMakerChangeTex(this, rInfo.mDummyChangeTexture);
        MR::newDifferedDLBuffer(this);
    }

    MtxPtr pJointMtx;
    bool hasCollision = MR::isExistCollisionResource(this, mObjectName);
    connectToScene(rInfo);

    if (rInfo._8C) {
        MR::initLightCtrl(this);
    }

    if (rInfo.mUseProjectMapMtx) {
        mMatrixSetter = MR::initDLMakerProjmapEffectMtxSetter(this);
        MR::newDifferedDLBuffer(this);
        _B4 = rInfo._48;
    }

    if (rInfo.mUseMirrorReflection) {
        MR::initMirrorReflection(this);
        _B5 = rInfo._74;
        TPos3f mtx(getBaseMtx());
        MR::setMirrorReflectionInfoFromMtxYUp(mtx);
    }

    if (rInfo.mInitBinder) {
        initBinder(rInfo.mBinderRadius, rInfo.mBinderCenterY, 0);
    }

    if (rInfo.mHasEffect) {
        initEffectKeeper(0, rInfo.mEffectName, false);
    }

    if (rInfo.mSound > 0) {
        bool hasSoundPos = rInfo.mSoundPos != nullptr;
        initSound(rInfo.mSound, hasSoundPos);

        if (hasSoundPos) {
            mSoundObject->setTrans(rInfo.mSoundPos);
        }
    }

    if (rInfo.mIsAppearRiddleSE) {
        _B6 = true;
    }

    if (rInfo.mHasShadows && rInfo.mShadowLength != 0.0f) {
        if (rInfo.mShadowName != nullptr) {
            MR::initShadowFromCSV(this, rInfo.mShadowName);
        } else {
            MR::initShadowFromCSV(this, "Shadow");
        }

        if (rInfo.mShadowLength != -1.0f) {
            MR::setShadowDropLength(this, nullptr, rInfo.mShadowLength);
        }
    }

    if (rInfo.mCalcGravity) {
        MR::onCalcGravity(this);
    }

    if (rInfo.mUseBaseMtxFollowTarget) {
        MR::addBaseMatrixFollowTarget(this, rIter, nullptr, nullptr);
    }

    if (rInfo.mNerve != nullptr) {
        initNerve(rInfo.mNerve);
    }

    if (rInfo.mHasSensors) {
        initHitSensor(1);
        f32 radius = rInfo.mSensorRadius;
        if (rInfo.mIsAffectedByScale) {
            radius *= mScale.x;
        }

        TVec3f position(rInfo.mSensorPosition);
        if (rInfo.mIsAffectedByScale) {
            position.mul(mScale);
        }

        if (rInfo.mHasHitSensorCB) {
            MR::addHitSensorCallbackMapObj(this, "body", rInfo._1C, radius);
        } else {
            MR::addHitSensorMapObj(this, "body", rInfo._1C, radius, position);
        }
    }

    if (hasCollision) {
        if (!rInfo.mHasSensors) {
            initHitSensor(1);
            MR::addBodyMessageSensorMapObj(this);
        }

        const char* pJointName = ::cFollowJointName;
        if (MR::isExistJoint(this, pJointName)) {
            pJointMtx = MR::getJointMtx(this, pJointName);
            MR::initCollisionParts(this, mObjectName, getSensor("body"), pJointMtx);
        } else {
            MR::initCollisionParts(this, mObjectName, getSensor("body"), nullptr);
        }

        MR::tryCreateCollisionMoveLimit(this, getSensor("body"));
    }

    if (hasRail) {
        initRailRider(rIter);
    }

    if (rInfo.mHasRailMover && hasRail) {
        mRailMover = new MapPartsRailMover(this);
        mRailMover->init(rIter);
    }

    if (rInfo.mHasRotator) {
        mRotator = new MapPartsRotator(this);
        mRotator->init(rIter);
    }

    if (rInfo.mHasRailRotator) {
        mRailRotator = new MapPartsRailRotator(this);
        mRailRotator->init(rIter);
    }

    if (rInfo._C) {
        mRotator = new MapPartsSeesaw1AxisRotator(this, rInfo.mSeesaw1AxisRotatorName, rInfo.mSeesaw1AxisRotatorRadius);
        mRotator->init(rIter);
    }

    if (rInfo._D) {
        mRotator = new MapPartsSeesaw2AxisRotator(this, rInfo.mSeesaw1AxisRotatorName, rInfo.mSeesaw1AxisRotatorRadius);
        mRotator->init(rIter);
    }

    if (rInfo.mUseRailPosture && hasRail) {
        s32 posture = 0;
        MR::getMapPartsArgMovePosture(&posture, this);

        if (posture != 0) {
            mRailPosture = new MapPartsRailPosture(this);
            mRailPosture->init(rIter);
        }
    }

    if (hasRail) {
        s32 guideType = 0;
        MR::getMapPartsArgRailGuideType(&guideType, this);

        if (guideType != 0) {
            mRailGuideDrawer = MR::createMapPartsRailGuideDrawer(this, "RailPoint", rIter);
        }
    }

    MR::tryStartAllAnim(this, mObjectName);

    if (rInfo.mColorChangeArg > -1) {
        MR::startBrk(this, ::cBrkNameColorChange);
        MR::setBrkFrameAndStop(this, rInfo.mColorChangeArg);
    }

    if (rInfo.mTextureChangeArg > -1) {
        const char* pTextureName = ::cBtpNameTexChange;
        if (MR::isExistBtp(this, pTextureName)) {
            MR::startBtp(this, pTextureName);
            MR::setBtpFrameAndStop(this, rInfo.mTextureChangeArg);
        }

        if (MR::isExistBtk(this, pTextureName)) {
            MR::startBtk(this, ::cBtkNameTexChange);
            MR::setBtkFrameAndStop(this, rInfo.mTextureChangeArg);
        }
    }

    f32 clippingRadius = -1.0f;
    if (0.0f < rInfo.mClippingRadius) {
        clippingRadius = rInfo.mClippingRadius;
    } else {
        MR::calcModelBoundingRadius(&clippingRadius, this);
    }

    if (rInfo.mIsAffectedByScale) {
        clippingRadius *= mScale.x;
    }

    MR::setClippingTypeSphere(this, clippingRadius);

    if (MR::isValidInfo(rIter) && rInfo.mGroupClipping > 0) {
        MR::setGroupClipping(this, rIter, rInfo.mGroupClipping);
    }

    if (rInfo.mFarClipping != 0.0f) {
        MR::setClippingFar(this, rInfo.mFarClipping);
    }

    if (!rInfo.mNoUseLOD && LodCtrlFunction::isExistLodLowModel(mObjectName)) {
        mPlanetLodCtrl = MR::createLodCtrlPlanet(this, rIter, -1.0f, rInfo._88);

        if (rInfo.mColorChangeArg > -1) {
            const char* pColorName = ::cBrkNameColorChange;
            if (MR::isExistBrk(this, pColorName)) {
                MR::startBrk(mPlanetLodCtrl->_14, pColorName);
                MR::setBrkFrameAndStop(mPlanetLodCtrl->_14, rInfo.mColorChangeArg);
            }
        }

        if (rInfo.mTextureChangeArg > -1) {
            const char* pTextureName = ::cBtpNameTexChange;
            if (MR::isExistBtp(this, pTextureName)) {
                MR::startBtp(mPlanetLodCtrl->_14, pTextureName);
                MR::setBtpFrameAndStop(mPlanetLodCtrl->_14, rInfo.mTextureChangeArg);
            }

            if (MR::isExistBtk(this, pTextureName)) {
                MR::startBtk(mPlanetLodCtrl->_14, ::cBtkNameTexChange);
                MR::setBtkFrameAndStop(mPlanetLodCtrl->_14, rInfo.mTextureChangeArg);
            }
        }
    }

    if (MR::isExistSubModel(mObjectName, "Bloom")) {
        char modelName[0x100];
        snprintf(modelName, sizeof(modelName), "%sBloom", mObjectName);
        const char* pName = mName;
        mBloomModel = MR::createModelObjBloomModel(pName, modelName, getBaseMtx());
        mBloomModel->mScale.set(mScale);
        MR::calcModelBoundingRadius(&clippingRadius, this);
        MR::setClippingFarMax(mBloomModel);
        MR::setClippingTypeSphere(mBloomModel, clippingRadius);
    }

    tryCreateBreakModel(rInfo);
    makeSubModels(rIter, rInfo);

    if (rInfo.mInitFur) {
        MR::initMultiFur(this, rInfo._5C);
    }

    makeActorAppeared();

    if (MR::useStageSwitchWriteA(this, rIter)) {
        initCaseUseSwitchA(rInfo);
    } else {
        initCaseNoUseSwitchA(rInfo);
    }

    if (MR::useStageSwitchWriteB(this, rIter)) {
        initCaseUseSwitchB(rInfo);
    } else {
        initCaseNoUseSwitchB(rInfo);
    }

    MR::useStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);

        if (rInfo.mIsAppearRiddleSE) {
            _B6 = true;
        }

        makeActorDead();
    }

    MR::useStageSwitchSleep(this, rIter);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (mModelObj != nullptr) {
            MR::tryRegisterDemoCast(mModelObj, rIter);
        }

        if (MR::isRegisteredDemoActionAppear(this)) {
            if (rInfo.mIsAppearRiddleSE) {
                _B6 = true;
            }

            makeActorDead();
        }
    }
}

bool MapObjActor::isObjectName(const char* pName) const {
    return MR::isEqualString(pName, mObjectName);
}

void MapObjActor::connectToScene(const MapObjActorInitInfo& rInfo) {
    if (rInfo.mConnectToScene) {
        if (MR::isExistCollisionResource(this, mObjectName)) {
            s32 type = rInfo._5C;

            if (type == 1) {
                MR::connectToSceneCollisionMapObjStrongLight(this);
            } else if (type == 2) {
                MR::connectToSceneCollisionMapObjWeakLight(this);
            } else {
                MR::connectToSceneCollisionMapObj(this);
            }
        } else if (rInfo._5C == 1) {
            MR::connectToSceneMapObjStrongLight(this);
        } else {
            MR::connectToSceneMapObj(this);
        }
    }
}

void MapObjActor::initCaseUseSwitchA(const MapObjActorInitInfo&) {
    setNerve(mWaitNrv);
}

void MapObjActor::initCaseNoUseSwitchA(const MapObjActorInitInfo&) {
}

void MapObjActor::initCaseUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MR::listenStageSwitchOnOffB(this, MR::Functor(this, &MapObjActor::startMapPartsFunctions), MR::Functor(this, &MapObjActor::endMapPartsFunctions));
}

void MapObjActor::initCaseNoUseSwitchB(const MapObjActorInitInfo& rInfo) {
    MapObjActorUtil::startAllMapPartsFunctions(this);
}

void MapObjActor::control() {
    if (!tryEmitWaitEffect()) {
        tryDeleteWaitEffect();
    }

    if (mPlanetLodCtrl != nullptr) {
        mPlanetLodCtrl->update();
    }

    if (mRailPosture != nullptr) {
        mRailPosture->movement();
    }

    if (mRailMover != nullptr) {
        mRailMover->movement();

        if (mRailMover->isWorking()) {
            mPosition.set(mRailMover->_28);
            mRailMover->tryResetPositionRepeat();
        }
    }

    if (mRotator != nullptr) {
        mRotator->movement();

        if (mRotator->isOnReverse()) {
            const char* startSound = MR::StageEffect::getStartSe(mObjectName);

            if (startSound != nullptr) {
                MR::startSound(this, startSound);
            }
        }
    }

    if (mRailRotator != nullptr) {
        mRailRotator->movement();
    }

    if (mRailMover == nullptr && mRotator == nullptr && mRailRotator == nullptr && !MR::isEqualString(mObjectName, "OceanRingRuinsMove")) {
        const char* movingSound = MR::StageEffect::getMovingSe(mObjectName);
        if (movingSound != nullptr) {
            MR::startLevelSound(this, movingSound);
        }
    }

    if (mRailGuideDrawer != nullptr) {
        mRailGuideDrawer->movement();
    }
}

void MapObjActor::calcAndSetBaseMtx() {
    updateProjmapMtx();

    if (MR::isExistMirrorCamera() && _B5) {
        MR::setMirrorReflectionInfoFromModel(this);
    }

    bool hasRotation = true;
    bool hasRotator = true;
    bool isRotatorWorking = false;

    if (mRotator != nullptr && mRotator->isWorking()) {
        isRotatorWorking = true;
    }

    if (!isRotatorWorking) {
        bool isRailRotatorWorking = false;

        if (mRailRotator != nullptr && mRailRotator->isWorking()) {
            isRailRotatorWorking = true;
        }

        if (!isRailRotatorWorking) {
            hasRotator = false;
        }
    }

    if (!hasRotator) {
        bool isRailPostureWorking = false;

        if (mRailPosture != nullptr && mRailPosture->isWorking()) {
            isRailPostureWorking = true;
        }

        if (!isRailPostureWorking) {
            hasRotation = false;
        }
    }

    if (!hasRotation) {
        LiveActor::calcAndSetBaseMtx();
    } else {
        TPos3f mtx;
        mtx.identity();

        if (mRailPosture != nullptr && mRailPosture->isWorking()) {
            mtx.concat(mRailPosture->_18);
        }

        if (mRotator != nullptr && mRotator->isWorking()) {
            mtx.concat(mRotator->getRotateMtx());
        }

        if (mRailRotator != nullptr && mRailRotator->isWorking()) {
            mtx.concat(mRailRotator->_5C);
        }

        mtx.mMtx[0][3] = mPosition.x;
        mtx.mMtx[1][3] = mPosition.y;
        mtx.mMtx[2][3] = mPosition.z;
        MR::setBaseTRMtx(this, mtx);
    }
}

void MapObjActor::startClipped() {
    tryEmitWaitEffect();
    LiveActor::startClipped();
}

void MapObjActor::endClipped() {
    LiveActor::endClipped();
    tryDeleteWaitEffect();
}

bool MapObjActor::tryCreateBreakModel(const MapObjActorInitInfo& rInfo) {
    char buf[0x100];

    if (rInfo._80 != nullptr) {
        snprintf(buf, sizeof(buf), "%s", rInfo._80);
    } else {
        snprintf(buf, sizeof(buf), "%sBreak", mObjectName);
    }

    if (!MR::isExistModel(buf)) {
        return false;
    }

    if (MR::isEqualString(mObjectName, "SandUpDownTowerBreakableWallB")) {
        MtxPtr baseMtx = getBaseMtx();
        mModelObj = MR::createModelObjMapObj("壊れモデル", buf, baseMtx);
    } else {
        MtxPtr baseMtx = getBaseMtx();
        mModelObj = MR::createModelObjMapObjStrongLight("壊れモデル", buf, baseMtx);
    }

    mModelObj->makeActorDead();

    return true;
}

bool MapObjActor::tryEmitWaitEffect() {
    if (!MR::isExistEffectKeeper(this)) {
        return false;
    }

    if (!MR::isRegisteredEffect(this, mObjectName)) {
        return false;
    }

    if (MR::calcCameraDistanceZ(mPosition) > 4000.0f) {
        return false;
    }

    if (MR::isEffectValid(this, mObjectName)) {
        return false;
    }

    MR::emitEffect(this, mObjectName);
    return true;
}

bool MapObjActor::tryDeleteWaitEffect() {
    if (!MR::isExistEffectKeeper(this)) {
        return false;
    }

    if (!MR::isRegisteredEffect(this, mObjectName)) {
        return false;
    }

    if (MR::calcCameraDistanceZ(mPosition) <= 4000.0f) {
        return false;
    }

    if (MR::isNearPlayer(this, 4000.0f)) {
        return false;
    }

    if (!MR::isEffectValid(this, mObjectName)) {
        return false;
    }

    MR::deleteEffect(this, mObjectName);
    return true;
}

void MapObjActor::startMapPartsFunctions() {
    MapObjActorUtil::startAllMapPartsFunctions(this);
}

void MapObjActor::endMapPartsFunctions() {
    MapObjActorUtil::endAllMapPartsFunctions(this);
}

void MapObjActor::pauseMapPartsFunctions() {
    MapObjActorUtil::pauseAllMapPartsFunctions(this);
}

void MapObjActor::setStateWait() {
    setNerve(mWaitNrv);
}

void MapObjActor::updateProjmapMtx() {
    if (mMatrixSetter == nullptr) {
        return;
    }

    if (!_B4) {
        return;
    }

    mMatrixSetter->updateMtxUseBaseMtx();
}

void MapObjActor::exeWait() {
    if (MR::isValidSwitchA(this) && MR::isOnSwitchA(this)) {
        MapObjActorUtil::startAllMapPartsFunctions(this);
        setNerve(mMoveNrv);
    }
}

void MapObjActor::exeMove() {
    if (MR::isFirstStep(this)) {
        const char* moveName = ::cBckNameMove;
        if (MR::isExistBck(this, moveName)) {
            MR::startBck(this, moveName);
        }
    }

    if (MR::isExistBck(this, ::cBckNameMove) && MR::isBckStopped(this)) {
        setNerve(mDoneNrv);
    }
}

void MapObjActor::exeDone() {
}

void MapObjActorUtil::startAllMapPartsFunctions(const MapObjActor* pActor) {
    if (pActor->mRotator != nullptr) {
        pActor->mRotator->start();
    }

    if (pActor->mRailMover != nullptr) {
        pActor->mRailMover->start();
    }

    if (pActor->mRailRotator != nullptr) {
        pActor->mRailRotator->start();
    }

    if (pActor->mRailPosture != nullptr) {
        pActor->mRailPosture->start();
    }

    if (pActor->mRailGuideDrawer != nullptr) {
        pActor->mRailGuideDrawer->start();
    }
}

void MapObjActorUtil::endAllMapPartsFunctions(const MapObjActor* pActor) {
    if (pActor->mRotator != nullptr) {
        pActor->mRotator->end();
    }

    if (pActor->mRailMover != nullptr) {
        pActor->mRailMover->end();
    }

    if (pActor->mRailRotator != nullptr) {
        pActor->mRailRotator->end();
    }

    if (pActor->mRailPosture != nullptr) {
        pActor->mRailPosture->end();
    }
}

void MapObjActorUtil::pauseAllMapPartsFunctions(const MapObjActor* pActor) {
    if (pActor->mRotator != nullptr) {
        pActor->mRotator->mIsActive = false;
    }

    if (pActor->mRailMover != nullptr) {
        pActor->mRailMover->mIsActive = false;
    }

    if (pActor->mRailRotator != nullptr) {
        pActor->mRailRotator->mIsActive = false;
    }
}

void MapObjActorUtil::resumeAllMapPartsFunctions(const MapObjActor* pActor) {
    if (pActor->mRotator != nullptr) {
        pActor->mRotator->mIsActive = true;
    }

    if (pActor->mRailMover != nullptr) {
        pActor->mRailMover->mIsActive = true;
    }

    if (pActor->mRailRotator != nullptr) {
        pActor->mRailRotator->mIsActive = true;
    }
}

bool MapObjActorUtil::isRotatorMoving(const MapObjActor* pActor) {
    return pActor->mRotator->isMoving();
}

bool MapObjActorUtil::isRailMoverWorking(const MapObjActor* pActor) {
    return pActor->mRailMover->isWorking();
}

bool MapObjActorUtil::isRailMoverReachedEnd(const MapObjActor* pActor) {
    return pActor->mRailMover->isReachedEnd();
}

f32 MapObjActorUtil::getSeesaw1AxisAngularSpeed(const MapObjActor* pActor) {
    return static_cast< MapPartsSeesaw1AxisRotator* >(pActor->mRotator)->mAngularVelocity;
}

void MapObjActorUtil::forceRotateSeesaw1Axis(const MapObjActor* pActor, f32 a2) {
    static_cast< MapPartsSeesaw1AxisRotator* >(pActor->mRotator)->mAngularAccel = a2;
}

void MapObjActorUtil::startRotator(const MapObjActor* pActor) {
    pActor->mRotator->start();
}

void MapObjActorUtil::startRailMover(const MapObjActor* pActor) {
    pActor->mRailMover->start();
}

void MapObjActorUtil::endRotator(const MapObjActor* pActor) {
    pActor->mRotator->end();
}

void MapObjActorUtil::pauseRotator(const MapObjActor* pActor) {
    pActor->mRotator->mIsActive = false;
}

void MapObjActorUtil::resetRailMoverToInitPos(const MapObjActor* pActor) {
    pActor->mRailMover->resetToInitPos();
}

void MapObjActorUtil::startBreak(MapObjActor* pActor) {
    if (!MapObjActorUtil::tryStartBreak(pActor)) {
        pActor->kill();
    }
}

namespace {
    const char* const cEffectNameBreak = "Break";
    const char* const cBckNameBreak = "Break";
}  // namespace

bool MapObjActorUtil::tryStartBreak(MapObjActor* pActor) {
    const char* stopSe = MR::StageEffect::getStopSe(pActor->mObjectName);
    if (stopSe != nullptr) {
        MR::startSound(pActor, stopSe);
    }

    const char* breakEffect = ::cEffectNameBreak;
    if (MR::isRegisteredEffect(pActor, breakEffect)) {
        MR::emitEffect(pActor, breakEffect);
    }

    ModelObj* modelObj = pActor->mModelObj;
    if (modelObj != nullptr) {
        pActor->mModelObj->appear();
        const char* breakName = ::cBckNameBreak;
        MR::startAllAnim(modelObj, breakName);

        if (MR::isExistBva(pActor, breakName)) {
            MR::startBva(pActor, breakName);
            MR::setBvaFrameAndStop(pActor, 1.0f);
        } else {
            MR::hideModel(pActor);
        }

        MR::invalidateClipping(modelObj);
        return true;
    } else {
        const char* const breakName = ::cBckNameBreak;
        if (MR::isExistBck(pActor, breakName)) {
            MR::startAllAnim(pActor, breakName);
            MR::invalidateClipping(pActor);
            return true;
        }
    }

    return false;
}

bool MapObjActorUtil::isBreakStopped(const MapObjActor* pActor) {
    const LiveActor* actor = pActor->mModelObj;

    if (pActor->mModelObj == nullptr && MR::isExistBck(pActor, ::cBckNameBreak)) {
        actor = pActor;
    }

    if (actor != nullptr) {
        return MR::isBckOneTimeAndStopped(actor);
    }

    return false;
}

void MapObjActorUtil::killBloomModel(MapObjActor* pActor) {
    pActor->mBloomModel->kill();
}

void MapObjActorUtil::appearBloomModel(MapObjActor* pActor) {
    pActor->mBloomModel->appear();
    char buf[0x100];
    snprintf(buf, sizeof(buf), "%sBloom", pActor->mObjectName);
    MR::tryStartAllAnim(pActor->mBloomModel, buf);
}
