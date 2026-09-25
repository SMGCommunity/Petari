#include "Game/Screen/LensFlare.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BrightObj.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/DrawSyncManager.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/TriggerChecker.hpp"

void LensFlare_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
}

namespace {
    NEW_NERVE(LensFlareModelNrvKill, LensFlareModel, Kill);
    NEW_NERVE(LensFlareModelNrvHide, LensFlareModel, Hide);
    NEW_NERVE(LensFlareModelNrvShow, LensFlareModel, Show);
    NEW_NERVE(LensFlareModelNrvFadeIn, LensFlareModel, FadeIn);
    NEW_NERVE(LensFlareModelNrvFadeOut, LensFlareModel, FadeOut);
};  // namespace

namespace {
    LensFlareDirector* getLensFlareDirector() {
        return MR::getSceneObj< LensFlareDirector >(SceneObj_LensFlareDirector);
    }
};  // namespace

LensFlareModel::LensFlareModel(const char* pName, const char* pArcName)
    : LiveActor(pName), _8C(), _90(), _94(), _98(new TriggerChecker()), _9C(new TriggerChecker()) {
    initModelManagerWithAnm(pArcName, nullptr, false);

    MR::connectToScene3DModelFor2D(this);

    MR::invalidateClipping(this);

    initNerve(GET_NERVE_ANON(LensFlareModelNrvKill));

    kill();
}

void LensFlareModel::update(bool param1, bool param2) {
    _98->update(param1);

    if (_98->getOnTrigger()) {
        notifyInArea();
    } else if (_98->getOffTrigger()) {
        setNerve(GET_NERVE_ANON(LensFlareModelNrvFadeOut));
    }

    _9C->update(param2 && param1);

    if (_9C->getOnTrigger()) {
        if (isNerve(GET_NERVE_ANON(LensFlareModelNrvHide))) {
            setNerve(GET_NERVE_ANON(LensFlareModelNrvShow));
        }
    } else if (_9C->getOffTrigger() && (isNerve(GET_NERVE_ANON(LensFlareModelNrvShow)) || isNerve(GET_NERVE_ANON(LensFlareModelNrvFadeIn)))) {
        setNerve(GET_NERVE_ANON(LensFlareModelNrvHide));
    }
}

void LensFlareModel::exeKill() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        kill();
    }
}

void LensFlareModel::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        _90 = 0.0f;
    }
}

void LensFlareModel::exeShow() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        _90 = 1.0f;
    }
}

void LensFlareModel::exeFadeIn() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
    }

    _90 += _94;

    if (_90 >= 1.0f) {
        _90 = 1.0f;
        setNerve(GET_NERVE_ANON(LensFlareModelNrvShow));
    }
}

void LensFlareModel::exeFadeOut() {
    _90 -= _94;

    if (_90 <= 0.0f) {
        _90 = 0.0f;
        setNerve(GET_NERVE_ANON(LensFlareModelNrvKill));
    }
}

void LensFlareModel::appear() {
    LiveActor::appear();
    appearAnim();
}

void LensFlareModel::control() {
    controlAnim();
}

void LensFlareModel::notifyInArea() {
    if (MR::isDead(this)) {
        appear();
    }

    if (_9C->getLevel() > 0.0f) {
        setNerve(GET_NERVE_ANON(LensFlareModelNrvFadeIn));
    } else {
        setNerve(GET_NERVE_ANON(LensFlareModelNrvHide));
    }
}

LensFlareRing::LensFlareRing() : LensFlareModel("レンズフレアリング", "LensFlare"), _A0() {
    mScale.x = 0.135f;
    mScale.y = 0.135f;
    mScale.z = 0.135f;
    _94 = 0.05f;
}

void LensFlareRing::appearAnim() {
    MR::startBckWithInterpole(this, "LensFlare", 0);
}

void LensFlareRing::controlAnim() {
    MR::startBrk(this, "LensFlare");
    MR::setBrkRate(this, 0.0f);
    MR::setBrkFrame(this, (1.0f - _8C * _90) * MR::getBrkCtrl(this)->getEnd());

    if (MR::isBckStopped(this)) {
        MR::startBckWithInterpole(this, "LensFlare", 0);
    }

    f32 frame = _A0 * MR::getBckCtrl(this)->getEnd();
    MR::setBckFrameAndStop(this, frame);
}

LensFlareGlow::LensFlareGlow() : LensFlareModel("グレア（円形）", "GlareGlow") {
    _94 = 0.05f;
}

void LensFlareGlow::appearAnim() {
    MR::startBtk(this, "GlareGlow");
}

void LensFlareGlow::controlAnim() {
    MR::startBrk(this, "GlareGlow");
    MR::setBrkRate(this, 0.0f);
    MR::setBrkFrame(this, (1.0f - _8C * _90) * MR::getBrkCtrl(this)->getEnd());
}

LensFlareLine::LensFlareLine() : LensFlareModel("グレア（ライン）", "GlareLine") {
    _94 = 0.05f;
}

void LensFlareLine::appearAnim() {
}

void LensFlareLine::controlAnim() {
    MR::startBrk(this, "GlareLine");
    MR::setBrkRate(this, 0.0f);
    MR::setBrkFrame(this, (1.0f - _8C * _90) * MR::getBrkCtrl(this)->getEnd());
}

LensFlareDirector::LensFlareDirector()
    : NameObj("レンズフレア管理"), mRing(), mGlow(), mLine(), mBrightObjCount(), _60(0.0f, 0.0f), _68(), _6C(0.0f, 0.0f), _74(0.0f, 0.0f), _7C(),
      mDrawSyncTokenIndex(), mBrightCamInfo() {
    _7C = DrawSyncManager::sInstance->setCallback(3, 2, this);
}

LensFlareDirector::~LensFlareDirector() {
}

void LensFlareDirector::init(const JMapInfoIter& rIter) {
    mRing = new LensFlareRing();
    mGlow = new LensFlareGlow();
    mLine = new LensFlareLine();
    mBrightCamInfo = new BrightCamInfo();

    MR::connectToSceneMapObjMovement(this);
}

void LensFlareDirector::movement() {
    s32 area = checkArea();
    controlFlare(area, checkBrightObj(area != 0));

    mBrightCamInfo->write(mDrawSyncTokenIndex, MR::getCameraViewMtx(), MR::getCameraProjectionMtx(), MR::getCamYdir(), MR::getCamPos());
}

void LensFlareDirector::drawSyncCallback(u16 val) {
    u32 i;
    s32 val2 = val - _7C;
    for (i = 0; i < mBrightObjCount; i++) {
        mBrightObjArray[i]->calcBrightInfo(val2, *mBrightCamInfo);
    }
}

void LensFlareDirector::pauseOff() {
    MR::requestMovementOn(this);
    MR::requestMovementOn(mRing);
    MR::requestMovementOn(mGlow);
    MR::requestMovementOn(mLine);
}

void LensFlareDirector::setDrawSyncToken() {
    DrawSyncManager::sInstance->pushBreakPoint();

    GXSetDrawSync(_7C + mDrawSyncTokenIndex);

    mDrawSyncTokenIndex = mDrawSyncTokenIndex ^ 0x1;
}

s32 LensFlareDirector::checkArea() {
    s32 area = 0;
    AreaObj* pAreaObj = MR::getAreaObj("LensFlareArea", *MR::getPlayerPos());

    if (pAreaObj != nullptr) {
        area = MR::getAreaObjArg(pAreaObj, 0);

        if (area == -1) {
            area = 0xFFFF;
        }
    }

    return area;
}

bool LensFlareDirector::checkBrightObj(bool b1) {
    bool ret = false;
    if (b1 && mBrightObjCount != 0) {
        TVec3f camPos = MR::getCamPos();

        s32 index = -1;
        for (u32 i = 0; i < mBrightObjCount; i++) {
            if (mBrightObjArray[i]->getBright() > 0.0f) {
                index = i;
                break;
            }
        }

        if (index >= 0) {
            ret = true;

            _60.set(*mBrightObjArray[index]->getBrightCenter());
            _68 = mBrightObjArray[index]->getBright();
            _6C.set(*mBrightObjArray[index]->getCenter());
            mBrightObjArray[index]->getNowCenter(&_74);
        }

        for (u32 i = 0; i < mBrightObjCount; i++) {
            mBrightObjArray[i]->endRead();
        }
    }

    return ret;
}

void LensFlareDirector::controlFlare(s32 area, bool hasBrightObj) {
    mRing->update(area >> 1 & 1, hasBrightObj);
    mGlow->update(area >> 2 & 1, hasBrightObj);
    mLine->update(area >> 3 & 1, hasBrightObj);

    if (!hasBrightObj || area == 0) {
        return;
    }

    TVec2f position(_60 - _6C);
    position += _74;
    TVec3f flarePos(position.x, -position.y, 0.0f);
    mRing->mPosition.set(flarePos);
    mGlow->mPosition.set(flarePos);
    mLine->mPosition.set(flarePos);

    TVec2f screenCenter(MR::getScreenWidth() / 2.0f, static_cast< s32 >(JUTGetVideoManager()->getEfbHeight()) / 2.0f);

    f32 screenRadius = screenCenter.length();
    TVec2f offset(screenCenter - position);
    f32 distanceRate = offset.length() / screenRadius;

    TVec3f rotation(0.0f, 0.0f, 0.0f);
    TVec2f direction(offset.x, offset.y);

    if (MR::isNearZero(direction)) {
        rotation.z = 0.0f;
    } else {
        MR::normalize(&direction);
        rotation.z = MR::calcRotateY(direction.x, direction.y);
    }

    mRing->mRotation.set(rotation);
    f32 ringFrame = distanceRate;
    if (ringFrame > 1.0f) {
        ringFrame = 1.0f;
    }

    mRing->_A0 = ringFrame;
    mRing->_8C = MR::clamp((1.0f - distanceRate) * _68, 0.0f, 1.0f);
    mGlow->_8C = MR::clamp((1.0f - distanceRate) * _68, 0.0f, 1.0f);
    mLine->_8C = MR::clamp((1.0f - distanceRate) * _68, 0.0f, 1.0f);
}

namespace MR {
    void addBrightObj(BrightObjBase* pBrightObj) {
        if (!MR::isExistSceneObj(SceneObj_LensFlareDirector)) {
            MR::createSceneObj(SceneObj_LensFlareDirector);
        }

        LensFlareDirector* pDirector = ::getLensFlareDirector();
        pDirector->mBrightObjArray[pDirector->mBrightObjCount++] = pBrightObj;
    }

    void setLensFlareDrawSyncToken() {
        if (MR::isExistSceneObj(SceneObj_LensFlareDirector)) {
            ::getLensFlareDirector()->setDrawSyncToken();
        }
    }

    u32 getLensFlareDrawSyncTokenIndex() {
        return ::getLensFlareDirector()->mDrawSyncTokenIndex;
    }
};  // namespace MR

TVec2f LensFlare_FORCE_MATCH(const TVec2f& rA, const TVec2f& rB) {
    return rA - rB;
}
