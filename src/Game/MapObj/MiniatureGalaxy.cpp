#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"

namespace {
    const char* cDemoNameDomeLecture = "ドームレクチャー２";

    const TVec3f cNamePlateOffset = TVec3f(0.0f, 1500.0f, 0.0f);
}  // namespace

namespace NrvMiniatureGalaxy {
    NEW_NERVE(MiniatureGalaxyNrvWait, MiniatureGalaxy, Wait);
    NEW_NERVE(MiniatureGalaxyNrvSelected, MiniatureGalaxy, Selected);
    NEW_NERVE(MiniatureGalaxyNrvNotSelected, MiniatureGalaxy, NotSelected);
    NEW_NERVE(MiniatureGalaxyNrvConfirmed, MiniatureGalaxy, Confirmed);
    NEW_NERVE(MiniatureGalaxyNrvOpen, MiniatureGalaxy, Open);
}  // namespace NrvMiniatureGalaxy

MiniatureGalaxy::MiniatureGalaxy(const char* pName)
    : LiveActor(pName), mGalaxyType(-1), mState(3), mUnknownModel(), mShadowModel(), mSelectModel(), mStarPlateModel(), mProjmapEffectMtxSetter(),
      _108(), mMiniatureName(), mName(), mCanZoomIn(1), mZoomLevel(20), _124(), _1B8(), mOrbit(),
      mNamePlate() {
    _D4.identity();
    _A4.identity();
}

MiniatureGalaxy::~MiniatureGalaxy() {
}

void MiniatureGalaxy::init(const JMapInfoIter& rIter) {
    MR::getObjectName(&mMiniatureName, rIter);
    mName = mMiniatureName + strlen("Mini");

    MR::initDefaultPos(this, rIter);
    _108.set(mPosition);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    mGalaxyType = arg0;

    initModelManagerWithAnm(isUseKoopaFaceModel() ? "MiniKoopaGalaxy" : mMiniatureName, nullptr, false);

    initEffectKeeper(0, nullptr, false);
    MR::connectToSceneNoShadowedMapObj(this);

    MR::initStarPointerTarget(this, 2500.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateClipping(this);

    initPartsModel();
    initNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvWait::sInstance);

    MR::tryRegisterDemoCast(this, rIter);

    const char* pDemoName = cDemoNameDomeLecture;
    MR::registerDemoCast(this, pDemoName, rIter);
    MR::registerDemoCast(mShadowModel, pDemoName, rIter);
    MR::registerDemoCast(mUnknownModel, pDemoName, rIter);
    MR::registerDemoCast(mSelectModel, pDemoName, rIter);

    if (mStarPlateModel != nullptr) {
        MR::registerDemoCast(mStarPlateModel, pDemoName, rIter);
    }

    SphereSelectorFunction::registerTarget(this);
    MiniatureGalaxyFunction::registerMiniatureGalaxyToHolder(this, rIter);

    mOrbit = new AstroDomeOrbit();
    mOrbit->initWithoutIter();

    mNamePlate = new GalaxyNamePlate(mName, true);
    mNamePlate->initWithoutIter();

    MR::registerDemoSimpleCastAll(mNamePlate);

    makeActorDead();
}

void MiniatureGalaxy::initAfterPlacement() {
    mOrbit->setup(MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(this));
}

void MiniatureGalaxy::appear() {
    const f32 rotations[5] = {0.0f, 210.0f, 80.0f, 330.0f, 130.0f};
    mRotation.set(TVec3f(0.0f, rotations[MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(this)], 0.0f));

    mCanZoomIn = 1;
    mZoomLevel = 20;
    setScale(0.65f);

    if (!MR::isAppearGalaxy(mName)) {
        mState = 0;
    } else {
        if (MR::isOnGameEventFlagGalaxyOpen(mName)) {
            mState = 3;
        } else {
            if (MR::canOpenGalaxy(mName)) {
                mState = 2;
            } else {
                mState = 1;
            }
        }
    }

    LiveActor::appear();

    MR::tryStartAllAnim(this, mMiniatureName);

    mShadowModel->appear();
    MR::startBtk(mShadowModel, "MiniatureGalaxyShadow");

    mSelectModel->appear();
    MR::startBtk(mSelectModel, "MiniatureGalaxySelect");
    showSelectModel();

    mOrbit->appear();

    switch (mState) {
    case 0:
    case 1:
        appearUnknownModel("Unknown");
        break;
    case 2:
        appearUnknownModel("New");
        break;
    case 3:
        MR::showModelIfHidden(this);

        if (isUseKoopaFaceModel()) {
            MR::emitEffect(this, "EyeLight");
        }

        break;
    }

    setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvWait::sInstance);
}

void MiniatureGalaxy::kill() {
    if (mGalaxyType == 2) {
        if (isUseKoopaFaceModel()) {
            MR::forceDeleteEffect(this, "EyeLight");
        }

        MR::forceDeleteEffect(mUnknownModel, "EyeLight");
    }

    LiveActor::kill();

    if (!MR::isDead(mShadowModel)) {
        mShadowModel->kill();
    }

    if (!MR::isDead(mUnknownModel)) {
        mUnknownModel->kill();
    }

    if (!MR::isDead(mSelectModel)) {
        mSelectModel->kill();
    }

    if (mStarPlateModel != nullptr && !MR::isDead(mStarPlateModel)) {
        mStarPlateModel->kill();
    }

    mOrbit->kill();
}

void MiniatureGalaxy::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    const char* pObjectName;
    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    s32 archiveType = arg0;
    MR::getObjectName(&pObjectName, rIter);

    if (archiveType == 0) {
        pArchiveList->addArchive(pObjectName);
        pArchiveList->addArchive("MiniatureGalaxyUnknown");
    } else if (archiveType == 1) {
        pArchiveList->addArchive(pObjectName);
        pArchiveList->addArchive("MiniHatenaGalaxy");
    } else if (archiveType == 2) {
        const char* pStageName;
        u32 miniLen = strlen("Mini");
        bool isUnknownKoopa = true;
        pStageName = pObjectName + miniLen;

        if (!MR::isEqualSubString(pObjectName, "KoopaBattleVs") && MR::isAlreadyVisitedStage(pStageName)) {
            isUnknownKoopa = false;
        }

        pArchiveList->addArchive(isUnknownKoopa ? "MiniKoopaGalaxy" : pObjectName);
        pArchiveList->addArchive("MiniatureGalaxyUnknownKoopa");
    }

    pArchiveList->addArchive("MiniatureGalaxyShadow");
    pArchiveList->addArchive("MiniatureGalaxySelect");
    pArchiveList->addArchive("MiniatureGalaxyStarNumber");
    pArchiveList->addArchive("GalaxyNamePlate");
}

void MiniatureGalaxy::control() {
    updateZoomInOut();
    mOrbit->calcGalaxyPos(&mPosition);

    mRotation.y = mRotation.y + 0.4f;
    mRotation.y = fmod(mRotation.y, 360.0);

    updateNamePlate();
}

void MiniatureGalaxy::calcAndSetBaseMtx() {
    TPos3f transform;
    SphereSelectorFunction::calcHandledRotateMtx(TVec3f(0.0f, 0.0f, 0.0f), &transform);

    TPos3f _a;
    _a.set(transform);

    MR::rotateMtxLocalYDegree(_a, mRotation.y);
    MR::setMtxTrans(_a, mPosition);
    MR::setBaseTRMtx(this, _a);

    const TVec3f cameraPosition = MR::getCamPos();
    cameraPosition.subOperatorInLine(mPosition);

    const TVec3f cameraYDirectior = MR::getCamYdir();

    MR::setBaseTRMtx(mShadowModel, _D4);
    mProjmapEffectMtxSetter->updateMtxUseBaseMtx();
}

bool MiniatureGalaxy::receiveOtherMsg(u32 messageId, HitSensor* pSensor1, HitSensor* pSensor2) {
    if (SphereSelectorFunction::trySyncAppearMsgSelectStart(this, messageId)) {
        return true;
    }

    if (SphereSelectorFunction::isMsgTargetSelected(messageId)) {
        if (isNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvWait::sInstance)) {
            if (mState == 2) {
                SphereSelectorFunction::invalidatePointing();
                setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvOpen::sInstance);
                return false;
            }

            if (mState == 3) {
                SphereSelectorFunction::confirmStart();
                setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvSelected::sInstance);
                return true;
            }
        }
    } else {
        if (SphereSelectorFunction::isMsgConfirmStart(messageId) && !isNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvSelected::sInstance)) {
            setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvNotSelected::sInstance);
            return true;
        }

        if (SphereSelectorFunction::isMsgConfirmCancel(messageId)) {
            setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvWait::sInstance);
            return true;
        }

        if (SphereSelectorFunction::isMsgConfirmed(messageId)) {
            setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvConfirmed::sInstance);
            return true;
        }
    }

    return false;
}

void MiniatureGalaxy::initPartsModel() {
    const char* unknownGalaxyName = "MiniatureGalaxyUnknown";
    if (mGalaxyType == 1) {
        unknownGalaxyName = "MiniHatenaGalaxy";
    } else if (mGalaxyType == 2) {
        unknownGalaxyName = "MiniatureGalaxyUnknownKoopa";
    }
    MtxPtr baseMtx = getBaseMtx();
    ModelObj* unknownModel = new ModelObj("Unknownモデル", unknownGalaxyName, baseMtx, 11, -2, -2, false);
    mUnknownModel = unknownModel;
    unknownModel->initWithoutIter();
    MR::invalidateClipping(unknownModel);
    unknownModel->kill();

    ModelObj* shadowModel = new ModelObj("影モデル", "MiniatureGalaxyShadow", _D4, 11, -2, -2, true);
    mShadowModel = shadowModel;
    mProjmapEffectMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(shadowModel);
    MR::newDifferedDLBuffer(shadowModel);
    shadowModel->initWithoutIter();
    MR::invalidateClipping(shadowModel);
    shadowModel->kill();

    mSelectModel = new ModelObj("選択時モデル", "MiniatureGalaxySelect", _A4, 11, -2, -2, false);
    mSelectModel->initWithoutIter();
    MR::invalidateClipping(mSelectModel);
    mSelectModel->kill();

    if (mGalaxyType != 1) {
        mStarPlateModel = new ModelObj("スター数モデル", "MiniatureGalaxyStarNumber", _A4, 11, -2, -2, true);

        const char* cStarNumber1 = "StarNumber1";
        const char* cStarNumber01 = "StarNumber01";
        const char* cStarNumber10 = "StarNumber10";
        int powerStarNum = MR::getPowerStarNumToOpenGalaxy(mName);
        if (powerStarNum < 10) {
            TexMtxCtrl* texMtxCtrl = MR::initDLMakerTexMtx(mStarPlateModel, cStarNumber1);
            texMtxCtrl->setTexMtx(0, &_124);

            MR::hideMaterial(mStarPlateModel, cStarNumber01);
            MR::hideMaterial(mStarPlateModel, cStarNumber10);
        } else {
            TexMtxCtrl* texMtxCtrl = MR::initDLMakerTexMtx(mStarPlateModel, cStarNumber01);
            texMtxCtrl->setTexMtx(0, &_124);

            texMtxCtrl = MR::initDLMakerTexMtx(mStarPlateModel, cStarNumber10);
            texMtxCtrl->setTexMtx(0, &_1B8);

            MR::hideMaterial(mStarPlateModel, cStarNumber1);
        }

        _1B8.mTexMtxInfo.mSRT.mTranslationY = 0.1f * (powerStarNum / 10);
        _124.mTexMtxInfo.mSRT.mTranslationY = 0.1f * (powerStarNum % 10);

        MR::newDifferedDLBuffer(mStarPlateModel);
        mStarPlateModel->initWithoutIter();
        MR::invalidateClipping(mStarPlateModel);
        mStarPlateModel->kill();
    }
}

void MiniatureGalaxy::setScale(f32 scale) {
    mScale.setAll< f32 >(scale);
    MR::setStarPointerTargetRadius3d(this, 2500.0f * scale);

    mShadowModel->mScale.setAll< f32 >(scale);

    if (!isNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvOpen::sInstance)) {
        mUnknownModel->mScale.setAll< f32 >(scale);

        if (mStarPlateModel != nullptr) {
            mStarPlateModel->mScale.setAll< f32 >(scale);
        }
    }
}

void MiniatureGalaxy::tryZoomIn(bool disappear) {
    if (mCanZoomIn == 1 && mZoomLevel == 20) {
        mCanZoomIn = 0;
        mZoomLevel = 0;
    }

    disappear ? disappearSelectModel() : appearSelectModel();
}

void MiniatureGalaxy::tryZoomOut() {
    if (mCanZoomIn == 0 && mZoomLevel == 20) {
        mCanZoomIn = 1;
        mZoomLevel = 0;
    }

    disappearSelectModel();
}

void MiniatureGalaxy::updateZoomInOut() {
    if (mZoomLevel != 20) {
        mZoomLevel++;
        f32 zoomProgress = static_cast< f32 >(mZoomLevel) / 20.0f;

        if (mCanZoomIn == 0) {
            f32 scale = MR::getLinerValue(MR::getScaleWithReactionValueZeroToOne(zoomProgress, 1.5f, -0.08f), 0.65f, 1.0f, 1.0f);
            setScale(scale);

            if (mZoomLevel == 1) {
                switch (mState) {
                case 0:
                case 1:
                    playNeedStarME();
                    break;
                case 2:
                case 3:
                    playPointedME();
                    break;
                }
            }
        } else {
            setScale(MR::getEaseOutValue(zoomProgress, 1.0f, 0.65f, 1.0f));
        }
    }
}

void MiniatureGalaxy::updateNamePlate() {
    if (!MR::isDead(mNamePlate)) {
        TVec3f newPosition;
        newPosition.add(mPosition, cNamePlateOffset);
        mNamePlate->setPos3D(newPosition);
    }
}

void MiniatureGalaxy::appearUnknownModel(const char* brkName) {
    mUnknownModel->appear();
    MR::startBrk(mUnknownModel, brkName);

    if (mGalaxyType == 0) {
        MR::startBtk(mUnknownModel, "MiniatureGalaxyUnknown");
    } else if (mGalaxyType == 1) {
        MR::startBtk(mUnknownModel, "MiniHatenaGalaxy");
    }

    if (mStarPlateModel != nullptr) {
        mStarPlateModel->appear();
        MR::startBrk(mStarPlateModel, brkName);
        MR::startBck(mStarPlateModel, "MiniatureGalaxyStarNumber", nullptr);

        MR::setBckFrameAndStop(mStarPlateModel, mGalaxyType == 2 ? 1.0f : 0.0f);
    }

    hideUnknownModel();
}

void MiniatureGalaxy::showUnknownModel() {
    MR::hideModelAndOnCalcAnimIfShown(this);
    MR::showModelIfHidden(mUnknownModel);

    if (mStarPlateModel != nullptr) {
        MR::showModelIfHidden(mStarPlateModel);
    }

    if (mGalaxyType == 2) {
        MR::emitEffect(mUnknownModel, "EyeLight");
    }
}

void MiniatureGalaxy::showSelectModel() {
    MR::showModelIfHidden(mSelectModel);
    MR::startBrk(mSelectModel, "Disappear");

    f32 frame = MR::getBrkFrame(mSelectModel);
    MR::setBrkFrame(mSelectModel, frame);
}

void MiniatureGalaxy::hideUnknownModel() {
    MR::hideModelAndOnCalcAnimIfShown(this);
    MR::hideModelIfShown(mUnknownModel);

    if (mStarPlateModel != nullptr) {
        MR::hideModelIfShown(mStarPlateModel);
    }

    if (mGalaxyType == 2) {
        MR::forceDeleteEffect(mUnknownModel, "EyeLight");
    }
}

void MiniatureGalaxy::appearSelectModel() {
    if (MR::isBrkPlaying(mSelectModel, "Disappear")) {
        f32 frame = MR::getBrkFrameMax(mSelectModel) - MR::getBrkFrame(mSelectModel);

        MR::startBrk(mSelectModel, "Appear");
        MR::setBrkFrame(mSelectModel, frame);
    }
}

void MiniatureGalaxy::disappearSelectModel() {
    if (MR::isBrkPlaying(mSelectModel, "Appear")) {
        f32 frame = MR::getBrkFrameMax(mSelectModel) - MR::getBrkFrame(mSelectModel);

        MR::startBrk(mSelectModel, "Disappear");
        MR::setBrkFrame(mSelectModel, frame);
    }
}

void MiniatureGalaxy::showNamePlate(bool a1) {
    if (!MR::isDemoActive()) {
        if (mState == 0) {
            mNamePlate->showUnknown(a1);
        } else if (mState == 1) {
            mNamePlate->showUnknown(a1);
        } else if (mState == 2) {
            mNamePlate->showNew(a1);
        } else if (mState == 3) {
            mNamePlate->showOpen(a1);
        }
    }
}

bool MiniatureGalaxy::isUseKoopaFaceModel() const {
    bool useKoopaFaceModel;
    if (mGalaxyType != 2) {
        useKoopaFaceModel = false;
    } else {
        useKoopaFaceModel = true;
        if (!MR::isEqualSubString(mMiniatureName, "KoopaBattleVs") && MR::isAlreadyVisitedStage(mName)) {
            useKoopaFaceModel = false;
        }
    }

    return useKoopaFaceModel;
}

void MiniatureGalaxy::playPointedME() {
    switch (MR::getRandom(0l, 5l)) {
    case 0:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY1");
        break;
    case 1:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY2");
        break;
    case 2:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY3");
        break;
    case 3:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY4");
        break;
    case 4:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY5");
        break;
    }
}

void MiniatureGalaxy::playNeedStarME() {
    switch (MR::getRandom(0l, 5l)) {
    case 0:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY_N1");
        break;
    case 1:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY_N2");
        break;
    case 2:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY_N3");
        break;
    case 3:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY_N4");
        break;
    case 4:
        MR::startSystemME("ME_ASTRO_DOME_HIT_GALAXY_N5");
        break;
    }
}

void MiniatureGalaxy::exeWait() {
    if (MR::isFirstStep(this)) {
        switch (mState) {
        case 0:
        case 1:
        case 2:
            showUnknownModel();
            break;
        case 3:
            MR::showModelIfHidden(this);

            if (isUseKoopaFaceModel()) {
                MR::emitEffect(this, "EyeLight");
            }
            break;
        }

        showSelectModel();
    }

    SphereSelectorFunction::tryRegisterPointingTarget(this, (HandlePointingPriority)0);

    if (SphereSelectorFunction::isPointingTarget(this)) {
        tryZoomIn(false);
        showNamePlate(true);
    } else {
        tryZoomOut();

        if (SphereSelectorFunction::isSelectWait() != 0 && SphereSelectorFunction::isValidPointing() &&
            MiniatureGalaxyFunction::getPointingMiniatureGalaxy() == nullptr) {
            showNamePlate(false);
        }
    }

    mOrbit->moveCoord();
}

void MiniatureGalaxy::exeNotSelected() {
    if (MR::isFirstStep(this)) {
        switch (mState) {
        case 0:
        case 1:
        case 2:
            hideUnknownModel();
            break;
        case 3:
            MR::hideModelAndOnCalcAnimIfShown(this);

            if (isUseKoopaFaceModel()) {
                MR::forceDeleteEffect(this, "EyeLight");
            }
            break;
        }

        MR::hideModelIfShown(mSelectModel);
    }
}

void MiniatureGalaxy::exeOpen() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(mUnknownModel, "Select");
        MR::emitEffect(mUnknownModel, "MiniatureGalaxyNewOpenSign");
        MR::startSystemSE("SE_SY_NEW_GALAXY_OPEN_SIGN", -1, -1);

        mCanZoomIn = 0;
        mZoomLevel = 20;
        setScale(1.0f);

        disappearSelectModel();

        MR::onGameEventFlagGalaxyOpen(mName);
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(mName);
        if (accessor.isExistGrandStar()) {
            MR::stopStageBGM(30);
        }
    }

    if (MR::isLessStep(this, 45)) {
        MR::startSystemSE("SE_SY_LV_NEW_GALAXY_OPENING", -1, -1);
    }

    if (MR::isStep(this, 45)) {
        MR::startSystemSE("SE_SY_NEW_GALAXY_OPEN", -1, -1);
        MR::emitEffect(mUnknownModel, "MiniatureGalaxyNewOpen");

        MR::tryRumblePadWeak(this, 0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, 75)) {
        mState = 3;
        MR::showModelIfHidden(this);

        if (isUseKoopaFaceModel()) {
            MR::emitEffect(this, "EyeLight");
        }

        mUnknownModel->kill();

        if (mStarPlateModel != nullptr) {
            mStarPlateModel->kill();
        }

        mCanZoomIn = 1;
        mZoomLevel = 20;
        setScale(0.65f);

        if (MR::makeGalaxyStatusAccessor(mName).isExistGrandStar()) {
            MR::startStageBGM("BGM_KOOPA_APPEAR", false);
            MR::setNextStageBGM("MBGM_ASTRO_DOME_2");
        }
    }

    if (MR::isLessStep(this, 75)) {
        MR::tryRumblePadVeryWeak(this, 0);
    }

    if (MR::isStep(this, 150)) {
        SphereSelectorFunction::validatePointing();
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(mName);

        if (accessor.isExistGrandStar()) {
            MR::startSystemSE("SE_SY_KOOPA_LAUGH", -1, -1);
        }

        setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvWait::sInstance);
    }
}

void MiniatureGalaxy::exeSelected() {
    tryZoomIn(true);
}

void MiniatureGalaxy::exeConfirmed() {}
