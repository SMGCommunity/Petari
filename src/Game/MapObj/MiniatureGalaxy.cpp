#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/LiveActor/MaterialCtrl.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/MapObj/AstroDomeOrbit.hpp"
#include "Game/MapObj/MiniatureGalaxyHolder.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/GalaxyNamePlate.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

enum MiniatureGalaxyType {
    MiniatureGalaxyType_Normal,
    MiniatureGalaxyType_Hatena,
    MiniatureGalaxyType_Koopa,
};

enum MiniatureGalaxyState {
    MiniatureGalaxyState_Hatena,
    MiniatureGalaxyState_Unknown,
    MiniatureGalaxyState_New,
    MiniatureGalaxyState_Open,
};

namespace {
    const Vec cNamePlateOffset = {0.0f, 1500.0f, 0.0f};
    const f32 cPointingRadius = 2500.0f;
    // const f32 cShadowHeight = _;
    const f32 cZoomInScale = 1.0f;
    const f32 cZoomOutScale = 0.65f;
    const s32 cZoomFrame = 20;
    const f32 cRotateSpeedY = 0.4f;
    // const f32 cNearClipDistance = _;
    const s32 cOpenDisappearStep = 75;
    const s32 cOpenMiniatureAppearStep = 45;
    const s32 cOpenTotalFrame = 150;
    const char* const cDemoNameDomeLecture2 = "ドームレクチャー２";

    const char* getGalayNameFromObjectName(const char* pObjName) {
        return pObjName + strlen("Mini");
    }

    bool isVisitedGalaxy(const char* pGalaxyName) {
        return MR::isAlreadyVisitedStage(pGalaxyName);
    }

    bool isUseKoopaFaceModel(const char* pObjName, const char* pGalaxyName) {
        return MR::isEqualSubString(pObjName, "KoopaBattleVs") || !::isVisitedGalaxy(pGalaxyName);
    }

    ModelObj* createGalaxyPart(const char* pName, const char* pModelName, MtxPtr pMtx, bool a1) {
        return new ModelObj(pName, pModelName, pMtx, MR::DrawBufferType_NoShadowedMapObj, -2, -2, a1);
    }

    void initGalaxyPart(ModelObj* pObj) {
        pObj->initWithoutIter();
        MR::invalidateClipping(pObj);
        pObj->kill();
    }
};  // namespace

namespace NrvMiniatureGalaxy {
    NEW_NERVE(MiniatureGalaxyNrvWait, MiniatureGalaxy, Wait);
    NEW_NERVE(MiniatureGalaxyNrvSelected, MiniatureGalaxy, Selected);
    NEW_NERVE(MiniatureGalaxyNrvNotSelected, MiniatureGalaxy, NotSelected);
    NEW_NERVE(MiniatureGalaxyNrvConfirmed, MiniatureGalaxy, Confirmed);
    NEW_NERVE(MiniatureGalaxyNrvOpen, MiniatureGalaxy, Open);
};  // namespace NrvMiniatureGalaxy

MiniatureGalaxy::MiniatureGalaxy(const char* pName)
    : LiveActor(pName), mType(-1), mState(MiniatureGalaxyState_Open), mUnknownModel(), mShadowModel(), mSelectModel(), mStarPlateModel(),
      mProjmapEffectMtxSetter(), mInitPos(gZeroVec), mObjectName(), mGalaxyName(), mCanZoomIn(1), mZoomLevel(::cZoomFrame), mOnesTexMtx(),
      mTensTexMtx(), mOrbit(), mNamePlate() {
    mShadowBaseMtx.identity();
    mPosMtx.identity();
}

void MiniatureGalaxy::init(const JMapInfoIter& rIter) {
    MR::getObjectName(&mObjectName, rIter);
    mGalaxyName = ::getGalayNameFromObjectName(mObjectName);

    MR::initDefaultPos(this, rIter);
    mInitPos.set(mPosition);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    mType = arg0;

    initModelManagerWithAnm(isUseKoopaFaceModel() ? "MiniKoopaGalaxy" : mObjectName, nullptr, false);

    initEffectKeeper(0, nullptr, false);
    MR::connectToSceneNoShadowedMapObj(this);

    MR::initStarPointerTarget(this, ::cPointingRadius, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateClipping(this);

    initPartsModel();
    initNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvWait));

    MR::tryRegisterDemoCast(this, rIter);

    const char* pDemoName = ::cDemoNameDomeLecture2;
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

    mNamePlate = new GalaxyNamePlate(mGalaxyName, true);
    mNamePlate->initWithoutIter();

    MR::registerDemoSimpleCastAll(mNamePlate);

    makeActorDead();
}

void MiniatureGalaxy::initAfterPlacement() {
    mOrbit->setup(MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(this));
}

void MiniatureGalaxy::appear() {
    const f32 rotations[] = {0.0f, 210.0f, 80.0f, 330.0f, 130.0f};
    mRotation.set< f32 >(0.0f, rotations[MiniatureGalaxyFunction::calcMiniatureGalaxyIndex(this)], 0.0f);

    mCanZoomIn = 1;
    mZoomLevel = ::cZoomFrame;
    setScale(::cZoomOutScale);

    if (!MR::isAppearGalaxy(mGalaxyName)) {
        mState = MiniatureGalaxyState_Hatena;
    } else if (MR::isOnGameEventFlagGalaxyOpen(mGalaxyName)) {
        mState = MiniatureGalaxyState_Open;
    } else if (MR::canOpenGalaxy(mGalaxyName)) {
        mState = MiniatureGalaxyState_New;
    } else {
        mState = MiniatureGalaxyState_Unknown;
    }

    LiveActor::appear();

    MR::tryStartAllAnim(this, mObjectName);

    mShadowModel->appear();
    MR::startBtk(mShadowModel, "MiniatureGalaxyShadow");

    mSelectModel->appear();
    MR::startBtk(mSelectModel, "MiniatureGalaxySelect");
    showSelectModel();

    mOrbit->appear();

    switch (mState) {
    case MiniatureGalaxyState_Hatena:
    case MiniatureGalaxyState_Unknown:
        appearUnknownModel("Unknown");

        break;
    case MiniatureGalaxyState_New:
        appearUnknownModel("New");

        break;
    case MiniatureGalaxyState_Open:
        MR::showModelIfHidden(this);

        if (isUseKoopaFaceModel()) {
            MR::emitEffect(this, "EyeLight");
        }

        break;
    }

    setNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvWait));
}

void MiniatureGalaxy::kill() {
    if (mType == MiniatureGalaxyType_Koopa) {
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
    const char* objectName;

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    s32 type = arg0;

    MR::getObjectName(&objectName, rIter);

    if (type == MiniatureGalaxyType_Normal) {
        pArchiveList->addArchive(objectName);
        pArchiveList->addArchive("MiniatureGalaxyUnknown");
    } else if (type == MiniatureGalaxyType_Hatena) {
        pArchiveList->addArchive(objectName);
        pArchiveList->addArchive("MiniHatenaGalaxy");
    } else if (type == MiniatureGalaxyType_Koopa) {
        // TODO: Should be replaced with ::isUseKoopaFaceModel
        const char* pStageName;
        u32 miniLen = strlen("Mini");
        bool isUnknownKoopa = true;
        pStageName = objectName + miniLen;

        if (!MR::isEqualSubString(objectName, "KoopaBattleVs") && MR::isAlreadyVisitedStage(pStageName)) {
            isUnknownKoopa = false;
        }

        pArchiveList->addArchive(isUnknownKoopa ? "MiniKoopaGalaxy" : objectName);
        // pArchiveList->addArchive(::isUseKoopaFaceModel(objectName, ::getGalayNameFromObjectName(objectName)) ? "MiniKoopaGalaxy" : objectName);
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

    mRotation.y += ::cRotateSpeedY;
    mRotation.y = MR::repeatDegree(mRotation.y);

    updateNamePlate();
}

void MiniatureGalaxy::calcAndSetBaseMtx() {
    // FIXME: stack, lots of inlines
    TPos3f rotMtx;
    TVec3f zero(0.0f);
    SphereSelectorFunction::calcHandledRotateMtx(zero, &rotMtx);

    TPos3f baseMtx = rotMtx;
    baseMtx.set(rotMtx);
    MR::rotateMtxLocalYDegree(baseMtx, mRotation.y);
    baseMtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, baseMtx);

    TVec3f frontVec;
    frontVec.sub(MR::getCamPos(), mPosition);

    TVec3f upVec;
    upVec.set(MR::getCamYdir());

    if (!MR::normalizeOrZero(&frontVec) && !MR::isSameDirection(frontVec, upVec)) {
        MR::makeMtxFrontUpPos(&mPosMtx, frontVec, upVec, mPosition);
    }

    TVec3f vec44;
    rotMtx.getYDir(vec44);

    TVec3f vec38;
    vec38.scale(-7000.0f, vec44);

    vec38 += mPosition;

    TVec3f vec2C;
    vec2C.scale(vec44.dot(mPosition), vec44);

    vec38 -= vec2C;

    mShadowBaseMtx.set(rotMtx);
    mShadowBaseMtx.setTrans(vec38);

    MR::setBaseTRMtx(mShadowModel, mShadowBaseMtx);

    mProjmapEffectMtxSetter->updateMtxUseBaseMtx();
}

bool MiniatureGalaxy::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::trySyncAppearMsgSelectStart(this, msg)) {
        return true;
    }

    if (SphereSelectorFunction::isMsgTargetSelected(msg)) {
        if (isNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvWait))) {
            if (mState == MiniatureGalaxyState_New) {
                SphereSelectorFunction::invalidatePointing();

                setNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvOpen));

                return false;
            }

            if (mState == MiniatureGalaxyState_Open) {
                SphereSelectorFunction::confirmStart();

                setNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvSelected));

                return true;
            }
        }
    } else {
        if (SphereSelectorFunction::isMsgConfirmStart(msg) && !isNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvSelected))) {
            setNerve(&NrvMiniatureGalaxy::MiniatureGalaxyNrvNotSelected::sInstance);
            return true;
        }

        if (SphereSelectorFunction::isMsgConfirmCancel(msg)) {
            setNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvWait));
            return true;
        }

        if (SphereSelectorFunction::isMsgConfirmed(msg)) {
            setNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvConfirmed));
            return true;
        }
    }

    return false;
}

void MiniatureGalaxy::initPartsModel() {
    const char* pModelName = "MiniatureGalaxyUnknown";

    if (mType == MiniatureGalaxyType_Hatena) {
        pModelName = "MiniHatenaGalaxy";
    } else if (mType == MiniatureGalaxyType_Koopa) {
        pModelName = "MiniatureGalaxyUnknownKoopa";
    }

    mUnknownModel = ::createGalaxyPart("Unknownモデル", pModelName, getBaseMtx(), false);
    ::initGalaxyPart(mUnknownModel);

    mShadowModel = ::createGalaxyPart("影モデル", "MiniatureGalaxyShadow", mShadowBaseMtx, true);
    mProjmapEffectMtxSetter = MR::initDLMakerProjmapEffectMtxSetter(mShadowModel);
    MR::newDifferedDLBuffer(mShadowModel);
    ::initGalaxyPart(mShadowModel);

    mSelectModel = ::createGalaxyPart("選択時モデル", "MiniatureGalaxySelect", mPosMtx, false);
    ::initGalaxyPart(mSelectModel);

    if (mType == MiniatureGalaxyType_Hatena) {
        return;
    }

    mStarPlateModel = ::createGalaxyPart("スター数モデル", "MiniatureGalaxyStarNumber", mPosMtx, true);

    s32 powerStarNum = MR::getPowerStarNumToOpenGalaxy(mGalaxyName);
    if (powerStarNum < 10) {
        MR::initDLMakerTexMtx(mStarPlateModel, "StarNumber1")->setTexMtx(0, &mOnesTexMtx);

        MR::hideMaterial(mStarPlateModel, "StarNumber01");
        MR::hideMaterial(mStarPlateModel, "StarNumber10");
    } else {
        MR::initDLMakerTexMtx(mStarPlateModel, "StarNumber01")->setTexMtx(0, &mOnesTexMtx);
        MR::initDLMakerTexMtx(mStarPlateModel, "StarNumber10")->setTexMtx(0, &mTensTexMtx);

        MR::hideMaterial(mStarPlateModel, "StarNumber1");
    }

    mOnesTexMtx.mTexMtxInfo.mSRT.mTranslationY = powerStarNum % 10 * 0.1f;
    mTensTexMtx.mTexMtxInfo.mSRT.mTranslationY = powerStarNum / 10 * 0.1f;

    MR::newDifferedDLBuffer(mStarPlateModel);
    ::initGalaxyPart(mStarPlateModel);
}

void MiniatureGalaxy::setScale(f32 scale) {
    mScale.setAll< f32 >(scale);
    MR::setStarPointerTargetRadius3d(this, ::cPointingRadius * scale);

    mShadowModel->mScale.setAll< f32 >(scale);

    if (isNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvOpen))) {
        return;
    }

    mUnknownModel->mScale.setAll< f32 >(scale);

    if (mStarPlateModel != nullptr) {
        mStarPlateModel->mScale.setAll< f32 >(scale);
    }
}

void MiniatureGalaxy::tryZoomIn(bool isDisappear) {
    if (mCanZoomIn == 1 && mZoomLevel == ::cZoomFrame) {
        mCanZoomIn = 0;
        mZoomLevel = 0;
    }

    if (isDisappear) {
        disappearSelectModel();
    } else {
        appearSelectModel();
    }
}

void MiniatureGalaxy::tryZoomOut() {
    if (mCanZoomIn == 0 && mZoomLevel == ::cZoomFrame) {
        mCanZoomIn = 1;
        mZoomLevel = 0;
    }

    disappearSelectModel();
}

void MiniatureGalaxy::updateZoomInOut() {
    if (mZoomLevel == ::cZoomFrame) {
        return;
    }

    // FIXME
    mZoomLevel++;
    f32 zoomRate = static_cast< f32 >(mZoomLevel) / ::cZoomFrame;

    if (mCanZoomIn == 0) {
        setScale(MR::getLinerValue(MR::getScaleWithReactionValueZeroToOne(zoomRate, 1.5f, -0.08f), ::cZoomOutScale, ::cZoomInScale, ::cZoomInScale));

        if (mZoomLevel != 1) {
            return;
        }

        switch (mState) {
        case MiniatureGalaxyState_Hatena:
        case MiniatureGalaxyState_Unknown:
            playNeedStarME();

            break;
        case MiniatureGalaxyState_New:
        case MiniatureGalaxyState_Open:
            playPointedME();

            break;
        }
    } else {
        setScale(MR::getEaseOutValue(zoomRate, ::cZoomInScale, ::cZoomOutScale, ::cZoomInScale));
    }
}

void MiniatureGalaxy::updateNamePlate() {
    if (MR::isDead(mNamePlate)) {
        return;
    }

    TVec3f newPosition;
    newPosition.add(mPosition, ::cNamePlateOffset);
    mNamePlate->setPos3D(newPosition);
}

void MiniatureGalaxy::appearUnknownModel(const char* brkName) {
    mUnknownModel->appear();
    MR::startBrk(mUnknownModel, brkName);

    if (mType == MiniatureGalaxyType_Normal) {
        MR::startBtk(mUnknownModel, "MiniatureGalaxyUnknown");
    } else if (mType == MiniatureGalaxyType_Hatena) {
        MR::startBtk(mUnknownModel, "MiniHatenaGalaxy");
    }

    if (mStarPlateModel != nullptr) {
        mStarPlateModel->appear();
        MR::startBrk(mStarPlateModel, brkName);
        MR::startBck(mStarPlateModel, "MiniatureGalaxyStarNumber", nullptr);
        MR::setBckFrameAndStop(mStarPlateModel, mType == MiniatureGalaxyType_Koopa ? 1.0f : 0.0f);
    }

    hideUnknownModel();
}

void MiniatureGalaxy::showUnknownModel() {
    MR::hideModelAndOnCalcAnimIfShown(this);
    MR::showModelIfHidden(mUnknownModel);

    if (mStarPlateModel != nullptr) {
        MR::showModelIfHidden(mStarPlateModel);
    }

    if (mType == MiniatureGalaxyType_Koopa) {
        MR::emitEffect(mUnknownModel, "EyeLight");
    }
}

void MiniatureGalaxy::showSelectModel() {
    MR::showModelIfHidden(mSelectModel);
    MR::startBrk(mSelectModel, "Disappear");
    MR::setBrkFrame(mSelectModel, MR::getBrkFrameMax(mSelectModel));
}

void MiniatureGalaxy::hideUnknownModel() {
    MR::hideModelAndOnCalcAnimIfShown(this);
    MR::hideModelIfShown(mUnknownModel);

    if (mStarPlateModel != nullptr) {
        MR::hideModelIfShown(mStarPlateModel);
    }

    if (mType == MiniatureGalaxyType_Koopa) {
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
    if (MR::isDemoActive()) {
        return;
    }

    if (mState == MiniatureGalaxyState_Hatena) {
        mNamePlate->showUnknown(a1);
    } else if (mState == MiniatureGalaxyState_Unknown) {
        mNamePlate->showUnknown(a1);
    } else if (mState == MiniatureGalaxyState_New) {
        mNamePlate->showNew(a1);
    } else if (mState == MiniatureGalaxyState_Open) {
        mNamePlate->showOpen(a1);
    }
}

bool MiniatureGalaxy::isUseKoopaFaceModel() const {
    if (mType != MiniatureGalaxyType_Koopa) {
        return false;
    }

    return ::isUseKoopaFaceModel(mObjectName, mGalaxyName);
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
        case MiniatureGalaxyState_Hatena:
        case MiniatureGalaxyState_Unknown:
        case MiniatureGalaxyState_New:
            showUnknownModel();

            break;
        case MiniatureGalaxyState_Open:
            MR::showModelIfHidden(this);

            if (isUseKoopaFaceModel()) {
                MR::emitEffect(this, "EyeLight");
            }

            break;
        }

        showSelectModel();
    }

    SphereSelectorFunction::tryRegisterPointingTarget(this, Unknown_0);

    if (SphereSelectorFunction::isPointingTarget(this)) {
        tryZoomIn(false);
        showNamePlate(true);
    } else {
        tryZoomOut();

        if (SphereSelectorFunction::isSelectWait() && SphereSelectorFunction::isValidPointing() &&
            MiniatureGalaxyFunction::getPointingMiniatureGalaxy() == nullptr) {
            showNamePlate(false);
        }
    }

    mOrbit->moveCoord();
}

void MiniatureGalaxy::exeNotSelected() {
    if (MR::isFirstStep(this)) {
        switch (mState) {
        case MiniatureGalaxyState_Hatena:
        case MiniatureGalaxyState_Unknown:
        case MiniatureGalaxyState_New:
            hideUnknownModel();

            break;
        case MiniatureGalaxyState_Open:
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
        MR::startSystemSE("SE_SY_NEW_GALAXY_OPEN_SIGN");

        mCanZoomIn = 0;
        mZoomLevel = ::cZoomFrame;
        setScale(::cZoomInScale);

        disappearSelectModel();

        MR::onGameEventFlagGalaxyOpen(mGalaxyName);
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(mGalaxyName);

        if (accessor.isExistGrandStar()) {
            MR::stopStageBGM(30);
        }
    }

    if (MR::isLessStep(this, ::cOpenMiniatureAppearStep)) {
        MR::startSystemLevelSE("SE_SY_LV_NEW_GALAXY_OPENING");
    }

    if (MR::isStep(this, ::cOpenMiniatureAppearStep)) {
        MR::startSystemSE("SE_SY_NEW_GALAXY_OPEN");
        MR::emitEffect(mUnknownModel, "MiniatureGalaxyNewOpen");
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }

    if (MR::isStep(this, ::cOpenDisappearStep)) {
        mState = MiniatureGalaxyState_Open;
        MR::showModelIfHidden(this);

        if (isUseKoopaFaceModel()) {
            MR::emitEffect(this, "EyeLight");
        }

        mUnknownModel->kill();

        if (mStarPlateModel != nullptr) {
            mStarPlateModel->kill();
        }

        mCanZoomIn = 1;
        mZoomLevel = ::cZoomFrame;
        setScale(::cZoomOutScale);

        if (MR::makeGalaxyStatusAccessor(mGalaxyName).isExistGrandStar()) {
            MR::startStageBGM("BGM_KOOPA_APPEAR", false);
            MR::setNextStageBGM("MBGM_ASTRO_DOME_2");
        }
    }

    if (MR::isLessStep(this, ::cOpenDisappearStep)) {
        MR::tryRumblePadVeryWeak(this, WPAD_CHAN0);
    }

    if (MR::isStep(this, ::cOpenTotalFrame)) {
        SphereSelectorFunction::validatePointing();

        if (MR::makeGalaxyStatusAccessor(mGalaxyName).isExistGrandStar()) {
            MR::startSystemSE("SE_SY_KOOPA_LAUGH");
        }

        setNerve(GET_NERVE(MiniatureGalaxy, MiniatureGalaxyNrvWait));
    }
}

void MiniatureGalaxy::exeSelected() {
    tryZoomIn(true);
}

void MiniatureGalaxy::exeConfirmed() {
}
