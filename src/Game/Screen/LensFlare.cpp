#include "Game/Screen/LensFlare.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BrightObj.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/DrawSyncManager.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/TriggerChecker.hpp"

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
    : LiveActor(pName), _8C(0.0f), _90(0.0f), _94(0.0f), _98(new TriggerChecker()), _9C(new TriggerChecker()) {
    initModelManagerWithAnm(pArcName, nullptr, false);
    MR::connectToScene3DModelFor2D(this);
    MR::invalidateClipping(this);
    initNerve(&::LensFlareModelNrvKill::sInstance);
    kill();
}

void LensFlareModel::update(bool param1, bool param2) {
    _98->update(param1);

    if (_98->getOnTrigger()) {
        notifyInArea();
    } else if (_98->getOffTrigger()) {
        setNerve(&::LensFlareModelNrvFadeOut::sInstance);
    }

    _9C->update(param2 && param1);

    if (_9C->getOnTrigger()) {
        if (isNerve(&::LensFlareModelNrvHide::sInstance)) {
            setNerve(&::LensFlareModelNrvShow::sInstance);
        }
    } else if (_9C->getOffTrigger() && (isNerve(&::LensFlareModelNrvShow::sInstance) || isNerve(&::LensFlareModelNrvFadeIn::sInstance))) {
        setNerve(&::LensFlareModelNrvHide::sInstance);
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

        setNerve(&::LensFlareModelNrvShow::sInstance);
    }
}

void LensFlareModel::exeFadeOut() {
    _90 -= _94;

    if (_90 <= 0.0f) {
        _90 = 0.0f;

        setNerve(&::LensFlareModelNrvKill::sInstance);
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
        setNerve(&::LensFlareModelNrvFadeIn::sInstance);
    } else {
        setNerve(&::LensFlareModelNrvHide::sInstance);
    }
}

LensFlareRing::LensFlareRing() : LensFlareModel("レンズフレアリング", "LensFlare"), _A0(0.0f) {
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
    MR::setBrkFrame(this, (1.0f - _8C * _90) * MR::getBrkCtrl(this)->mEnd);

    if (MR::isBckStopped(this)) {
        MR::startBckWithInterpole(this, "LensFlare", 0);
    }

    MR::setBckFrameAndStop(this, _A0 * MR::getBckCtrl(this)->mEnd);
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
    MR::setBrkFrame(this, (1.0f - _8C * _90) * MR::getBrkCtrl(this)->mEnd);
}

LensFlareLine::LensFlareLine() : LensFlareModel("グレア（ライン）", "GlareLine") {
    _94 = 0.05f;
}

void LensFlareLine::appearAnim() {}

void LensFlareLine::controlAnim() {
    MR::startBrk(this, "GlareLine");
    MR::setBrkRate(this, 0.0f);
    MR::setBrkFrame(this, (1.0f - _8C * _90) * MR::getBrkCtrl(this)->mEnd);
}

LensFlareDirector::LensFlareDirector()
    : NameObj("レンズフレア管理"), mRing(nullptr), mGlow(nullptr), mLine(nullptr), mBrightObjArray(), _60(0.0f, 0.0f), _68(0.0f), _6C(0.0f, 0.0f),
      _74(0.0f), _78(0.0f), _7C(0), mDrawSyncTokenIndex(0), mBrightCamInfo(nullptr) {
    // _7C = DrawSyncManager::sInstance->setCallback(3, 2, &_C);
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

    // mBrightCamInfo->write(mDrawSyncTokenIndex, MR::getCameraViewMtx(), MR::getCameraProjectionMtx(), MR::getCamYdir(), MR::getCamPos());
}

// LensFlareDirector::drawSyncCallback

void LensFlareDirector::pauseOff() {
    MR::requestMovementOn(this);
    MR::requestMovementOn(mRing);
    MR::requestMovementOn(mGlow);
    MR::requestMovementOn(mLine);
}

void LensFlareDirector::setDrawSyncToken() {
    DrawSyncManager::sInstance->pushBreakPoint();
    // GXSetDrawSync(_7C + mDrawSyncTokenIndex);

    mDrawSyncTokenIndex = !mDrawSyncTokenIndex;
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

// LensFlareDirector::checkBrightObj
// LensFlareDirector::controlFlare

namespace MR {
    void addBrightObj(BrightObjBase* pBrightObj) {
        if (!MR::isExistSceneObj(SceneObj_LensFlareDirector)) {
            MR::createSceneObj(SceneObj_LensFlareDirector);
        }

        getLensFlareDirector()->mBrightObjArray.push_back(pBrightObj);
    }

    void setLensFlareDrawSyncToken() {
        if (MR::isExistSceneObj(SceneObj_LensFlareDirector)) {
            getLensFlareDirector()->setDrawSyncToken();
        }
    }

    u16 getLensFlareDrawSyncTokenIndex() {
        return getLensFlareDirector()->mDrawSyncTokenIndex;
    }
};  // namespace MR
