#include "Game/Map/FileSelectItem.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Map/FileSelectIconID.hpp"
#include "Game/Map/FileSelectItemDelegator.hpp"
#include "Game/Map/FileSelectModel.hpp"
#include "Game/NPC/MiiFaceParts.hpp"
#include "Game/NPC/MiiFacePartsHolder.hpp"
#include "Game/NPC/MiiFaceRecipe.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Screen/FileSelectNumber.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    NEW_NERVE(FileSelectItemNrvNewWait, FileSelectItem, NewWait);
    NEW_NERVE(FileSelectItemNrvExistWait, FileSelectItem, ExistWait);
    NEW_NERVE(FileSelectItemNrvFormat, FileSelectItem, Format);
    NEW_NERVE(FileSelectItemNrvChangeFellow, FileSelectItem, ChangeFellow);
    NEW_NERVE(FileSelectItemNrvChangeMii, FileSelectItem, ChangeMii);

    bool checkCollisionOfPointAndCylinder(const TVec3f& rVec1, const TVec3f& rVec2, const TVec3f& rVec3, f32 f1) {
        f32 length = rVec3.length();
        TVec3f vecCopy(rVec3);
        MR::normalize(&vecCopy);

        TVec3f diff(rVec1 - rVec2);

        f32 dot = vecCopy.dot(diff);
        if (dot < 0.0f || dot > length) {
            return false;
        }

        vecCopy.scale(dot);
        return !(vecCopy.distance(diff) > f1);
    }

    const char* sFellowModel[5] = {"FileSelectDataMario", "FileSelectDataLuigi", "FileSelectDataYoshi", "FileSelectDataKinopio",
                                   "FileSelectDataPeach"};

    static const Vec sDataInfoOffset = {0.0f, 2150.0f, 0.0f};
};  // namespace

namespace FileSelectItemSub {
    NEW_NERVE(ScaleControllerNrvToSmall, ScaleController, ToSmall);
    NEW_NERVE(ScaleControllerNrvSmall, ScaleController, Small);
    NEW_NERVE(ScaleControllerNrvToBig, ScaleController, ToBig);
    NEW_NERVE(ScaleControllerNrvBig, ScaleController, Big);
    NEW_NERVE(BlinkControllerNrvOpen, BlinkController, Open);
    NEW_NERVE(BlinkControllerNrvShut, BlinkController, Shut);
    NEW_NERVE(BlinkControllerNrvSleep, BlinkController, Sleep);
    NEW_NERVE(BlinkControllerNrvBlink, BlinkController, Blink);
};  // namespace FileSelectItemSub

void FORCE_SCALE() {
    TVec3f vec;
    vec.scale(1.0f);
}

FileSelectItem::FileSelectItem(s32 a1, bool a2, const FileSelectIconID& rID, const char* pName) : LiveActor(pName) {
    _8C = a2;
    mPlanetMapObj = nullptr;
    mIconID = new FileSelectIconID(rID);
    mFaceParts = nullptr;
    _A0 = nullptr;
    _134.set(0.0f);
    _140 = a1;
    _144 = 0;
    mIsInvalidateSelect = false;
    _146 = 0;
    _147 = 0;
    mScaleCtrl = new FileSelectItemSub::ScaleController();
    mBlinkCtrl = new FileSelectItemSub::BlinkController(this);
    mDelegator = nullptr;
    _154 = 1;
    _155 = 0;
    _156 = 0;
    _158.x = 0.0f;
    _158.y = 0.0f;
    _160 = 0.0f;
    mIsInvalidRotate = true;
    _165 = 0;
    _168 = 0;
    _16C = 0;
    _A4.identity();
    _D4.identity();
    _104.identity();

    mModels = new FileSelectModel*[5];

    for (u32 i = 0; i < 5; i++) {
        mModels[i] = nullptr;
    }
}

void FileSelectItem::init(const JMapInfoIter& rIter) {
    MR::connectToSceneMapObjMovement(this);
    MR::createSceneObj(SceneObj_MiiFacePartsHolder);
    createNew();
    createFellows();
    createMii();
    createNumber();
    MR::initStarPointerTarget(this, 1000.0f, TVec3f(0.0f, 900.0f, 0.0f));
    MR::invalidateClipping(this);
    initNerve(GET_NERVE_ANON(FileSelectItemNrvNewWait));
    MR::createCenterScreenBlur();
    makeActorAppeared();
}

void FileSelectItem::appear() {
    LiveActor::appear();

    if (_8C) {
        killAllModels();
        mPlanetMapObj->makeActorAppeared();
        setNerve(GET_NERVE_ANON(FileSelectItemNrvNewWait));
    } else {
        if (mIconID->isMii()) {
            killAllModels();
            mFaceParts->makeActorAppeared();
        } else {
            appearFellowModel();
        }

        setNerve(GET_NERVE_ANON(FileSelectItemNrvExistWait));
    }
}

void FileSelectItem::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    _144 = 0;
    mIsInvalidateSelect = false;
}

void FileSelectItem::makeActorDead() {
    LiveActor::makeActorDead();
    mPlanetMapObj->makeActorDead();

    for (s32 i = 0; i < 5; i++) {
        mModels[i]->makeActorDead();
    }

    mFaceParts->makeActorDead();
}

bool FileSelectItem::isNew() const {
    return isNerve(GET_NERVE_ANON(FileSelectItemNrvNewWait));
}

bool FileSelectItem::isExist() const {
    return isNerve(GET_NERVE_ANON(FileSelectItemNrvExistWait));
}

void FileSelectItem::format() {
    setNerve(GET_NERVE_ANON(FileSelectItemNrvFormat));
    deleteCompleteEffect();
    _8C = 1;
}

void FileSelectItem::change(const FileSelectIconID& rID, bool a2) {
    mIconID->set(rID);

    if (rID.isMii()) {
        setNerve(GET_NERVE_ANON(FileSelectItemNrvChangeMii));
    } else {
        setNerve(GET_NERVE_ANON(FileSelectItemNrvChangeFellow));
    }

    deleteCompleteEffect();
    _147 = a2;
    _165 = 0;
    _8C = 0;
}

void FileSelectItem::forceChange(const FileSelectIconID& rID, bool a2) {
    deleteCompleteEffect();
    mIconID->set(rID);
    if (rID.isMii()) {
        mFaceParts->changeFaceModel(MiiFaceRecipe(RFLDataSource_Official, mIconID->getMiiIndex(), RFLResolution_256, 33));
        killAllModels();
        mFaceParts->makeActorAppeared();
    } else {
        appearFellowModel();
    }

    _147 = a2;
    emitCompleteEffect();
    setNerve(GET_NERVE_ANON(FileSelectItemNrvExistWait));
    _8C = 0;
}

void FileSelectItem::invalidateSelect() {
    if (_144) {
        offPointing();
    }

    mIsInvalidateSelect = true;
}

void FileSelectItem::validateSelect() {
    mIsInvalidateSelect = false;
}

void FileSelectItem::appearIndex() {
    _A0->appear();
}

void FileSelectItem::disappearIndex() {
    _A0->disappear();
}

void FileSelectItem::copyIconID(FileSelectIconID* pId) {
    pId->set(*mIconID);
}

void FileSelectItem::setSelectDelegator(FileSelectItemDelegatorBase* pDele) {
    mDelegator = pDele;
}

void FileSelectItem::onPointing() {
    if (!mIsInvalidateSelect) {
        if (isNerve(GET_NERVE_ANON(FileSelectItemNrvNewWait))) {
            playPointedNotUsingME();
        } else {
            playPointedME();
        }

        _A0->onSelectIn();
        _144 = 1;
        mScaleCtrl->setNerve(GET_NERVE_DIRECT(FileSelectItemSub, ScaleControllerNrvToBig));
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
    }
}

void FileSelectItem::offPointing() {
    if (!mIsInvalidateSelect) {
        _A0->onSelectOut();
        _144 = 0;
        mScaleCtrl->setNerve(GET_NERVE_DIRECT(FileSelectItemSub, ScaleControllerNrvToSmall));
    }
}

void FileSelectItem::validateRotate() {
    mIsInvalidRotate = false;
}

void FileSelectItem::turnToFront(s32 angle) {
    _168 = angle;
    _16C = 0;
}

void FileSelectItem::exeFormat() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isLessStep(this, 40)) {
        MR::startSystemLevelSE("SE_SY_LV_FILE_SEL_MORPHBLUR");
    }

    if (MR::isStep(this, 40)) {
        MR::startSystemSE("SE_SY_FILE_SEL_MORPH_DUMMY");
        emitVanish();
        killAllModels();
        mPlanetMapObj->makeActorAppeared();
        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }

    if (MR::isGreaterEqualStep(this, 40)) {
        mRotation.y = 0.0f;
        _160 = 0.0f;
    }

    if (MR::isStep(this, 60)) {
        setNerve(GET_NERVE_ANON(FileSelectItemNrvNewWait));
    }
}

void FileSelectItem::exeChangeFellow() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isLessStep(this, 40)) {
        MR::startSystemLevelSE("SE_SY_LV_FILE_SEL_MORPHBLUR");
    }

    if (MR::isStep(this, 40)) {
        MR::startSystemSE("SE_SY_FILE_SEL_MORPH_MARIO");
        appearFellowModel();
        emitCompleteEffect();

        if (_165) {
            emitCopy();
        } else {
            emitOpen();
        }

        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }

    if (MR::isGreaterEqualStep(this, 40)) {
        mRotation.y = 0.0f;
        _160 = 0.0f;
    }

    if (MR::isStep(this, 150)) {
        setNerve(GET_NERVE_ANON(FileSelectItemNrvExistWait));
    }
}

void FileSelectItem::exeChangeMii() {
    if (MR::isFirstStep(this)) {
    }

    if (MR::isLessStep(this, 40)) {
        MR::startSystemLevelSE("SE_SY_LV_FILE_SEL_MORPHBLUR");
    }

    if (MR::isStep(this, 39)) {
        mFaceParts->changeFaceModel(MiiFaceRecipe(RFLDataSource_Official, mIconID->getMiiIndex(), RFLResolution_256, 33));
    }

    if (MR::isStep(this, 40)) {
        MR::startSystemSE("SE_SY_FILE_SEL_MORPH_MARIO");
        killAllModels();
        mFaceParts->makeActorAppeared();
        emitCompleteEffect();

        if (_165) {
            emitCopy();
        } else {
            emitOpen();
        }

        MR::tryRumblePadStrong(this, WPAD_CHAN0);
        MR::shakeCameraNormal();
    }

    if (MR::isGreaterEqualStep(this, 40)) {
        mRotation.y = 0.0f;
        _160 = 0.0f;
    }

    if (MR::isStep(this, 150)) {
        setNerve(GET_NERVE_ANON(FileSelectItemNrvExistWait));
    }
}

void FileSelectItem::control() {
    updatePointing();
    updateRotate();

    TPos3f mtx;
    if (mRotation.x == 0.0f && mRotation.z == 0.0f) {
        MR::makeMtxTransRotateY(mtx, this);
    } else {
        MR::makeMtxTR(mtx, this);
    }

    TVec3f yDir;
    mtx.getYDir(yDir);

    TVec3f trans;
    mtx.getTrans(trans);
    _A4.set(mtx);
    _A4.setTrans(yDir * 30.0f + trans * 30.0f);

    _D4.set(mtx);
    _104.set(mtx);

    mScaleCtrl->updateNerve();

    mPlanetMapObj->mScale.setAll< f32 >(30.0f * mScaleCtrl->_8);

    for (s32 i = 0; i < 5; i++) {
        mModels[i]->mScale.setAll< f32 >(30.0f * mScaleCtrl->_8);
    }

    mFaceParts->mScale.setAll< f32 >(30.0f * mScaleCtrl->_8);

    mBlinkCtrl->updateNerve();

    TVec3f screenPos;
    TVec3f newPos;
    newPos.add(mPosition, sDataInfoOffset);
    MR::calcScreenPosition(&screenPos, newPos);
    _A0->setTrans(screenPos);
}

void FileSelectItem::createNew() {
    mPlanetMapObj = MR::createPartsModelMapObj(this, "ニューフェイス", "FileSelectDataPlanet", _A4);
    mPlanetMapObj->mScale.set(30.0f);
    mPlanetMapObj->makeActorDead();
}

void FileSelectItem::createFellows() {
    for (u32 i = 0; i < 5; i++) {
        mModels[i] = new FileSelectModel(::sFellowModel[i], _D4, "キャラフェイス");
    }
}

void FileSelectItem::createMii() {
    if (_8C || !mIconID->isMii()) {
        mFaceParts = MiiFacePartsHolder::createPartsFromDefault("Miiフェイス", 0);
    } else {
        mFaceParts = MiiFacePartsHolder::createPartsFromReceipe("Miiフェイス",
                                                                MiiFaceRecipe(RFLDataSource_Official, mIconID->getMiiIndex(), RFLResolution_256, 33));
    }

    mFaceParts->initFixedPosition(_104, TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    mFaceParts->mScale.set(27.0f);
    mFaceParts->initEffectKeeper(0, "FileSelectDataMii", false);
    MR::invalidateClipping(mFaceParts);
    mFaceParts->makeActorDead();
}

void FileSelectItem::createNumber() {
    _A0 = new FileSelectNumber("ファイル番号");
    _A0->initWithoutIter();
    _A0->setNumber(_140);
}

void FileSelectItem::updatePointing() {
    if (mIsInvalidateSelect) {
        if (MR::isStarPointerPointing1PWithoutCheckZ(this, nullptr, false, false)) {
            if (mDelegator != nullptr) {
                mDelegator->notify(this, 2);
            }
        }
    } else if (MR::isStarPointerPointingFileSelect(this)) {
        if (mDelegator != nullptr) {
            mDelegator->notify(this, 0);
        }
    }

    if (!mIsInvalidateSelect && _144 && MR::testDPDMenuPadDecideTrigger()) {
        if (mDelegator != nullptr) {
            mDelegator->notify(this, 1);
            mScaleCtrl->setNerve(GET_NERVE_DIRECT(FileSelectItemSub, ScaleControllerNrvToSmall));
        }
    }
}

// still quite a ways to go with this one.
void FileSelectItem::updateRotate() {
    if (mIsInvalidRotate) {
        return;
    }

    if (_168 > 0) {
        _160 = 0.0f;

        f32 v6 = (f32)++_16C / (_168);
        v6 *= v6;
        if (v6 > 1.0f) {
            v6 = 1.0f;
        }

        if (v6 < 0.0f) {
            v6 = 0.0f;
        }

        if (_16C >= _168) {
            _16C = 0;
            _168 = 0;
        }

        mRotation.y = MR::repeat(mRotation.y, -180.0f, 360.0f);
        mBlinkCtrl->open();
        mBlinkCtrl->setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvOpen));
    } else if (MR::isStarPointerInScreen(0)) {
        TVec3f v43 = mPosition + TVec3f(0.0f, 900.0f, 0.0f);
        TVec2f screenPos(*MR::getStarPointerScreenPosition(0));

        if (_154) {
            _158 = screenPos;
            _156 = _155;
            _154 = 0;
        }

        TVec3f v42 = MR::getCamPos();  // 0x100
        TVec3f stack_F4 = v43 - v42;

        f32 v11 = (900.0f + stack_F4.length());
        if (JGeometry::TUtil< f32 >::sqrt(screenPos.squareDist(_158)) < 2.0f) {
            TVec3f v40;
            MR::calcWorldPositionFromScreen(&v40, screenPos, v11);
            TVec3f v39 = v40 - v42;
            MR::normalize(&v39);
            TVec3f v38 = stack_F4.cross(v39);

            if (v38.length() < 900.0f) {
                _155 = 1;
            }
        } else {
            TVec3f v37;
            MR::calcWorldPositionFromScreen(&v37, screenPos, v11);
            TVec3f v36;
            MR::calcWorldPositionFromScreen(&v36, _158, v11);
            TVec3f v44;
            v44 = v42;  // v44 = 0x118 / v42 = 0x100
            TVec3f v45;
            v45 = v37;  // v45 = 0x124 / v37 = 0xC4
            TVec3f v46;
            v46 = v36;  // v46 = 0x130 / v36 = 0xB8

            TVec3f v47;
            v47 = v37 - v42;  // v47 = 0x13C / v26 = 0x40

            TVec3f v48;
            v48 = v36 - v37;  // v48 = 0x148 / v27 = 0x4C

            TVec3f v49;
            v49 = v42 - v36;
            TVec3f v50 = v48.cross(v47);  // 0x160
            MR::normalize(&v50);

            f32 v12 = v50.dot(v43 - v42);

            bool v14;
            bool v13;

            if (MR::abs(v12) >= 900.0f) {
                v13 = false;
            } else {
                TVec3f v29;
                v29 = v43 - v50 * v12;
                TVec3f v25 = v29 - v44;
                TVec3f v34 = v25.cross(v47);

                if (v34.dot(v50) < 0.0f) {
                    v14 = 0;
                } else {
                    v34.cross(v25 - v45, v48);

                    if (v34.dot(v50) < 0.0f) {
                        v14 = 0;
                    } else {
                        v34.cross(v25 - v46, v49);

                        v14 = !(v34.dot(v50) < 0.0f);
                    }
                }
                if (v14) {
                    v13 = 1;
                } else if (v44.distance(v43) <= 900.0f) {
                    v13 = 1;
                } else if (v45.distance(v43) <= 900.0f) {
                    v13 = 1;
                } else if (v46.distance(v43) <= 900.0f) {
                    v13 = 1;
                } else if (checkCollisionOfPointAndCylinder(v43, v44, v47, 900.0f)) {
                    v13 = 1;
                } else if (checkCollisionOfPointAndCylinder(v43, v45, v48, 900.0f)) {
                    v13 = 1;
                } else {
                    v13 = checkCollisionOfPointAndCylinder(v43, v46, v49, 900.0f);
                }
            }

            if (v13) {
                _155 = 1;
            }
        }

        if (_155) {
            f32 v15 = -25.0f;
            f32 v21 = screenPos.x - _158.x;
            f32 v20 = screenPos.y - _158.y;
            f32 v16 = (_160 + (0.029f * v21) / mScale.x);
            _160 += (0.029f * v21) / mScale.x;

            if (v16 >= -25.0f) {
                v15 = 25.0f;

                if (v16 <= 25.0f) {
                    v15 = v16;
                }
            }

            _160 = v15;
        }

        _156 = _155;
        _158 = screenPos;
    } else {
        _154 = 1;
    }

    _160 *= 0.98f;

    if (_160 >= 0.0f && _160 < 0.5f) {
        _160 = 0.5f;
    }

    if (_160 < 0.0f && _160 > -0.5f) {
        _160 = -0.5f;
    }

    mRotation.y = MR::repeat(mRotation.y + _160, 0.0f, 360.0f);
    _155 = 0;
}

void FileSelectItem::playPointedME() {
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

void FileSelectItem::playPointedNotUsingME() {
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

void FileSelectItem::appearFellowModel() {
    killAllModels();
    mModels[mIconID->getFellowID()]->makeActorAppeared();
}

void FileSelectItem::killAllModels() {
    mPlanetMapObj->makeActorDead();

    for (s32 i = 0; i < 5; i++) {
        mModels[i]->makeActorDead();
    }

    mFaceParts->makeActorDead();
}

void FileSelectItem::emitOpen() {
    if (!MR::isDead(mPlanetMapObj)) {
        MR::emitEffect(mPlanetMapObj, "Open");
    }

    for (s32 i = 0; i < 5; i++) {
        if (!MR::isDead(mModels[i])) {
            mModels[i]->emitOpen();
        }
    }

    if (!MR::isDead(mFaceParts)) {
        MR::emitEffect(mFaceParts, "Open");
    }
}

void FileSelectItem::emitVanish() {
    if (!MR::isDead(mPlanetMapObj)) {
        MR::emitEffect(mPlanetMapObj, "Vanish");
    }

    for (s32 i = 0; i < 5; i++) {
        if (!MR::isDead(mModels[i])) {
            mModels[i]->emitVanish();
        }
    }

    if (!MR::isDead(mFaceParts)) {
        MR::emitEffect(mFaceParts, "Vanish");
    }
}

void FileSelectItem::emitCopy() {
    if (!MR::isDead(mPlanetMapObj)) {
        MR::emitEffect(mPlanetMapObj, "Copy");
    }

    for (s32 i = 0; i < 5; i++) {
        if (!MR::isDead(mModels[i])) {
            mModels[i]->emitCopy();
        }
    }

    if (!MR::isDead(mFaceParts)) {
        MR::emitEffect(mFaceParts, "Copy");
    }
}

void FileSelectItem::emitCompleteEffect() {
    if (!_147) {
        return;
    }

    for (s32 i = 0; i < 5; i++) {
        if (!MR::isDead(mModels[i])) {
            mModels[i]->emitCompleteEffect();
        }
    }

    if (!MR::isDead(mFaceParts)) {
        MR::emitEffect(mFaceParts, "Complete");
    }
}

void FileSelectItem::deleteCompleteEffect() {
    for (s32 i = 0; i < 5; i++) {
        mModels[i]->deleteCompleteEffect();
    }

    MR::deleteEffect(mFaceParts, "Complete");
}

namespace FileSelectItemSub {

    ScaleController::ScaleController() : NerveExecutor("ファイルセレクタアイコンサイズ管理") {
        _8 = 1.0f;
        initNerve(GET_NERVE_DIRECT(FileSelectItemSub, ScaleControllerNrvSmall));
    }

    void ScaleController::exeToSmall() {
        if (MR::isLessEqualStep(this, 30)) {
            f32 v = (getNerveStep() / 30.0f);
            _8 += (v * (1.0f - _8));
        }

        MR::setNerveAtStep(this, GET_NERVE_DIRECT(FileSelectItemSub, ScaleControllerNrvSmall), 30);
    }

    void ScaleController::exeToBig() {
        if (MR::isLessEqualStep(this, 30)) {
            f32 v = (getNerveStep() / 30.0f);
            _8 += v * (1.2f - _8);
        }

        MR::setNerveAtStep(this, GET_NERVE_DIRECT(FileSelectItemSub, ScaleControllerNrvBig), 30);
    }

    BlinkController::BlinkController(FileSelectItem* pItem) : NerveExecutor("ファイルセレクタアイコン瞬き管理") {
        mItem = pItem;
        _C = 0;
        _10 = 0;
        initNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvOpen));
    }

    void BlinkController::exeOpen() {
        if (MR::isFirstStep(this)) {
            _C = MR::getRandom(180l, 300l);
            _10 = 0;
        }

        f32 v2 = MR::abs(mItem->_160);

        if (v2 > 10.0f) {
            _10 += 2;
        } else {
            if (v2 > 6.0f) {
                _10++;
            } else {
                _10 = 0;
            }
        }

        if (_10 > 180) {
            setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvSleep));
        } else {
            if (MR::isGreaterEqualStep(this, _C)) {
                setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvShut));
            }
        }
    }

    void BlinkController::exeShut() {
        if (MR::isFirstStep(this)) {
            shut();
        }

        if (MR::isGreaterEqualStep(this, 10)) {
            open();
            setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvOpen));
        }
    }

    void BlinkController::exeSleep() {
        if (MR::isFirstStep(this)) {
            sleep();
            _10 = 0;
        }

        _10++;

        if (MR::abs(mItem->_160) > 2.0f) {
            _10 = 0;
        }

        if (_10 > 60) {
            setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvBlink));
        }
    }

    void BlinkController::exeBlink() {
        if (MR::isFirstStep(this) && mItem->mIconID->isFellow()) {
            mItem->mModels[mItem->mIconID->getFellowID()]->blink();
        }

        if (getNerveStep() % 8 < 4) {
            mItem->mFaceParts->changeExpressionBlink();

        } else {
            mItem->mFaceParts->changeExpressionNormal();
        }

        if (mItem->mIconID->isFellow()) {
            FileSelectIconID::EFellowID id = mItem->mIconID->getFellowID();

            if (mItem->mModels[id]->isOpen()) {
                setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvOpen));
                return;
            }
        }

        if (mItem->mIconID->isMii()) {
            if (MR::isGreaterEqualStep(this, 40)) {
                mItem->mFaceParts->changeExpressionNormal();
                setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvOpen));
            }
        }
    }

    void BlinkController::shut() {
        if (!mItem->_8C) {
            if (mItem->mIconID->isMii()) {
                mItem->mFaceParts->changeExpressionBlink();
            } else {
                mItem->mModels[mItem->mIconID->getFellowID()]->blinkOnce();
            }
        }
    }

    void BlinkController::open() {
        if (!mItem->_8C) {
            if (mItem->mIconID->isMii()) {
                mItem->mFaceParts->changeExpressionNormal();
            } else {
                mItem->mModels[mItem->mIconID->getFellowID()]->open();
            }
        }
    }

    void BlinkController::sleep() {
        if (!mItem->_8C) {
            if (mItem->mIconID->isMii()) {
                mItem->mFaceParts->changeExpressionBlink();
            } else {
                mItem->mModels[mItem->mIconID->getFellowID()]->close();
            }
        }
    }

    void ScaleController::exeBig() {
        _8 = 1.2f;
    }

    void ScaleController::exeSmall() {
        _8 = 1.0f;
    }
};  // namespace FileSelectItemSub

void FileSelectItem::exeExistWait() {
}

void FileSelectItem::exeNewWait() {
}

FileSelectItem::~FileSelectItem() {
}
