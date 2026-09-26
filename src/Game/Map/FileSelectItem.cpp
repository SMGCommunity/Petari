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

void FileSelectItem_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
    (void)1000.0f;
    (void)900.0f;
    (void)30.0f;
    (void)27.0f;
    (void)360.0f;
    (void)-180.0f;
    (void)0.03f;
    (void)-25.0f;
    (void)25.0f;
    (void)0.98f;
    (void)-0.5f;
    (void)1.2f;
    (void)10.0f;
    (void)6.0f;
}

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

void FileSelectItem_DUMMY() {
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
    _A4.setTrans(trans + yDir * 30.0f * 30.0f);

    _D4.set(mtx);
    _104.set(mtx);

    mScaleCtrl->updateNerve();

    mPlanetMapObj->mScale.setAll< f32 >(30.0f * mScaleCtrl->getScale());

    for (s32 i = 0; i < 5; i++) {
        mModels[i]->mScale.setAll< f32 >(30.0f * mScaleCtrl->getScale());
    }

    mFaceParts->mScale.setAll< f32 >(27.0f * mScaleCtrl->getScale());

    mBlinkCtrl->updateNerve();

    TVec3f newPos;
    TVec3f screenPos;
    newPos.add(mPosition, ::sDataInfoOffset);
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

namespace {
    struct PointerSweepTriangle {
        TVec3f mVertexA;
        TVec3f mVertexB;
        TVec3f mVertexC;
        TVec3f mEdgeAB;
        TVec3f mEdgeBC;
        TVec3f mEdgeCA;
        TVec3f mNormal;

        void set(const TVec3f& rCameraPosition, const TVec3f& rCurrentPoint, const TVec3f& rPreviousPoint) {
            mVertexA = rCameraPosition;
            mVertexB = rCurrentPoint;
            mVertexC = rPreviousPoint;

            mEdgeAB = rCurrentPoint - rCameraPosition;
            mEdgeBC = rPreviousPoint - rCurrentPoint;
            mEdgeCA = rCameraPosition - rPreviousPoint;
            mNormal.cross(mEdgeBC, mEdgeAB);
            MR::normalize(&mNormal);
        }

        PointerSweepTriangle(const TVec3f& rA, const TVec3f& rB, const TVec3f& rC) {
            set(rA, rB, rC);
        }

        bool contains(const TVec3f& rPoint) const {
            bool containsPoint;
            TVec3f cross;
            cross.cross(rPoint - mVertexA, mEdgeAB);

            if (cross.dot(mNormal) < 0.0f) {
                containsPoint = false;
            } else {
                cross.cross(rPoint - mVertexB, mEdgeBC);

                if (cross.dot(mNormal) < 0.0f) {
                    containsPoint = false;
                } else {
                    cross.cross(rPoint - mVertexC, mEdgeCA);

                    containsPoint = !(cross.dot(mNormal) < 0.0f);
                }
            }

            return containsPoint;
        }

        void project(TVec3f* pResult, const TVec3f& rCenter, f32 distance) const {
            TVec3f offset(mNormal);
            offset.scale(distance);
            *pResult = rCenter - offset;
        }
    };

    inline bool checkCollisionOfPointAndTriangle(const TVec3f& rCenter, const TVec3f& rCameraPosition, const TVec3f& rCurrentPoint,
                                                 const TVec3f& rPreviousPoint) {
        PointerSweepTriangle triangle(rCameraPosition, rCurrentPoint, rPreviousPoint);

        TVec3f delta(rCenter - rCameraPosition);
        f32 planeDistance = triangle.mNormal.dot(delta);

        bool containsPoint;
        bool collides;

        if (MR::abs(planeDistance) >= 900.0f) {
            collides = false;
        } else {
            TVec3f projected;
            triangle.project(&projected, rCenter, planeDistance);
            containsPoint = triangle.contains(projected);
            if (containsPoint) {
                collides = true;
            } else if (triangle.mVertexA.distance(rCenter) <= 900.0f) {
                collides = true;
            } else if (triangle.mVertexB.distance(rCenter) <= 900.0f) {
                collides = true;
            } else if (triangle.mVertexC.distance(rCenter) <= 900.0f) {
                collides = true;
            } else if (::checkCollisionOfPointAndCylinder(rCenter, triangle.mVertexA, triangle.mEdgeAB, 900.0f)) {
                collides = true;
            } else if (::checkCollisionOfPointAndCylinder(rCenter, triangle.mVertexB, triangle.mEdgeBC, 900.0f)) {
                collides = true;
            } else {
                collides = ::checkCollisionOfPointAndCylinder(rCenter, triangle.mVertexC, triangle.mEdgeCA, 900.0f);
            }
        }

        return collides;
    }

}  // namespace

void FileSelectItem::updateRotate() {
    if (mIsInvalidRotate) {
        return;
    }

    const s32 duration = _168;
    if (duration > 0) {
        _160 = 0.0f;

        _16C++;
        const s32 step = _16C;
        f32 rate = static_cast< f32 >(step) / duration;
        rate *= rate;
        if (rate > 1.0f) {
            rate = 1.0f;
        }

        if (rate < 0.0f) {
            rate = 0.0f;
        }

        if (step >= duration) {
            _16C = 0;
            _168 = 0;
        }

        f32 angle = MR::repeat(mRotation.y, -180.0f, 360.0f);
        mRotation.y = angle - angle * rate;
        FileSelectItemSub::BlinkController* pBlink = mBlinkCtrl;
        pBlink->open();
        pBlink->setNerve(GET_NERVE_DIRECT(FileSelectItemSub, BlinkControllerNrvOpen));
        return;
    } else if (MR::isStarPointerInScreen(0)) {
        TVec3f center = mPosition + TVec3f(0.0f, 900.0f, 0.0f);
        TVec2f screenPos(*MR::getStarPointerScreenPosition(0));

        if (_154) {
            _158 = screenPos;
            _156 = _155;
            _154 = 0;
        }

        TVec3f cameraPosition = MR::getCamPos();
        TVec3f cameraToCenter = center - cameraPosition;

        f32 pointerDistance = (900.0f + cameraToCenter.length());
        if (JGeometry::TUtil< f32 >::sqrt(screenPos.squareDist(_158)) < 2.0f) {
            TVec3f pointerPosition;
            MR::calcWorldPositionFromScreen(&pointerPosition, screenPos, pointerDistance);
            TVec3f pointerDirection = pointerPosition - cameraPosition;
            MR::normalize(&pointerDirection);
            TVec3f crossDirection = cameraToCenter.cross(pointerDirection);

            if (crossDirection.length() < 900.0f) {
                _155 = 1;
            }
        } else {
            TVec3f currentPoint;
            MR::calcWorldPositionFromScreen(&currentPoint, screenPos, pointerDistance);
            TVec3f previousPoint;
            MR::calcWorldPositionFromScreen(&previousPoint, _158, pointerDistance);
            bool collides = ::checkCollisionOfPointAndTriangle(center, cameraPosition, currentPoint, previousPoint);

            if (collides) {
                _155 = 1;
            }
        }

        if (_155) {
            TVec2f delta = screenPos - _158;
            _160 += 0.03f * delta.x / mScale.x;
            _160 = MR::clamp(_160, -25.0f, 25.0f);
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
