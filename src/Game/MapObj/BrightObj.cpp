#include "Game/MapObj/BrightObj.hpp"
#include "Game/MapObj/Sun.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/LensFlare.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>

namespace {
    const static f32 sSunRadius = 3000.0f;
    const static f32 sSunModelScale = 100.0f;

    bool calcScreenPosition(TVec2f* pDst, const TVec3f& rTrans, const TPos3f& rViewMtx, const TProj3f& rProjMtx) {
        TVec3f vec;
        rViewMtx.mult(rTrans, vec);

        TVec3f newPos;
        rProjMtx.mult(vec, newPos);

        bool ret;
        if (1.0f < MR::abs(newPos.x) || 1.0f < MR::abs(newPos.y)) {
            ret = false;
        } else {
            ret = 0.0f < newPos.z == false;
        }

        f32 width = MR::getScreenWidth();
        f32 height = JUTGetVideoManager()->getEfbHeight();
        TVec3f screenPos(0.5f * width + 0.5f * newPos.x * width, 0.5f * height + 0.5f * -newPos.y * height, newPos.z);

        pDst->x = screenPos.x;
        pDst->y = screenPos.y;

        return ret;
    }
};  // namespace

BrightInfo::BrightInfo() {
    reset();
}

void BrightInfo::write(const TVec2f& rBrightnessCenter, const TVec2f& rRealCenter, f32 brightness) {
    mBrightnessCenter[mIndex].set(rBrightnessCenter);
    mRealCenter[mIndex].set(rRealCenter);
    mBright[mIndex] = brightness;

    u32 val = mIndex;
    if (++val > 2) {
        val = 0;
    }

    if (val != mCurrIndex) {
        mIndex = val;
    }
}

void BrightInfo::endRead() {
    if (mCurrIndex == mIndex) {
        return;
    }

    mCurrIndex++;

    if (mCurrIndex > 2) {
        mCurrIndex = 0;
    }
}

void BrightInfo::reset() {
    mIndex = 0;
    mCurrIndex = 0;

    for (u32 i = 0; i < 3; i++) {
        mBrightnessCenter[i].zero();
        mRealCenter[i].zero();
        mBright[i] = 0.0f;
    }
}

BrightDrawInfo::BrightDrawInfo() {
    for (u32 idx = 0; idx < 2; idx++) {
        mPosition[idx].zero();
        mRadius[idx] = 0.0f;
    }
}

void BrightDrawInfo::write(u16 index, const TVec3f& rPosition, f32 radius) {
    mPosition[index].set(rPosition);
    mRadius[index] = radius;
}

BrightCamInfo::BrightCamInfo() {
    for (u32 idx = 0; idx < ARRAY_SIZE(mViewMtx); idx++) {
        mViewMtx[idx].identity();

        /* set mtx to identity mtx */
        mProjectionMtx[idx][2][3] = 0.0f;
        mProjectionMtx[idx][1][3] = 0.0f;
        mProjectionMtx[idx][0][3] = 0.0f;
        mProjectionMtx[idx][3][2] = 0.0f;
        mProjectionMtx[idx][1][2] = 0.0f;
        mProjectionMtx[idx][0][2] = 0.0f;
        mProjectionMtx[idx][3][1] = 0.0f;
        mProjectionMtx[idx][2][1] = 0.0f;
        mProjectionMtx[idx][0][1] = 0.0f;
        mProjectionMtx[idx][3][0] = 0.0f;
        mProjectionMtx[idx][2][0] = 0.0f;
        mProjectionMtx[idx][1][0] = 0.0f;
        mProjectionMtx[idx][3][3] = 1.0f;
        mProjectionMtx[idx][2][2] = 1.0f;
        mProjectionMtx[idx][1][1] = 1.0f;
        mProjectionMtx[idx][0][0] = 1.0f;

        mCameraDir[idx].set< f32 >(0.0f, 1.0f, 0.0f);
        mCameraPos[idx].zero();
    }
}

void BrightCamInfo::write(u16 index, const TPos3f& rViewMtx, const TProj3f& rProjMtx, const TVec3f& rCamDir, const TVec3f& rCamPos) {
    mViewMtx[index].set(rViewMtx);
    mProjectionMtx[index].set(rProjMtx);
    mCameraDir[index].set(rCamDir);
    mCameraPos[index].set(rCamPos);
}

BrightObjBase::BrightObjBase() : mInfo(), mBrightCenter(0.0f, 0.0f), mNowCenter(0.0f, 0.0f), mBrightness(), mIsNotVisible(true), mDrawInfo() {
    MR::addBrightObj(this);
}

void BrightObjBase::checkVisibilityOfSphere(u16 index, const BrightCamInfo& rCamInfo) {
    GXPokeAlphaRead(GX_READ_NONE);

    TVec3f drawPos(mDrawInfo.mPosition[index]);
    TVec3f camDir(rCamInfo.mCameraDir[index]);
    TVec3f drawDir(rCamInfo.mCameraPos[index] - drawPos);

    if (MR::isNearZero(drawDir)) {
        mIsNotVisible = true;
        mBrightness = 0.0f;

        return;
    }

    MR::normalize(&drawDir);

    TVec3f vec110;
    vec110.cross(camDir, drawDir);
    camDir.cross(drawDir, vec110);

    TPos3f mtx;
    mtx.setTR(vec110, camDir, drawDir, drawPos);

    CheckArg args = CheckArg();
    ::calcScreenPosition(&args.mCenter, drawPos, rCamInfo.mViewMtx[index], rCamInfo.mProjectionMtx[index]);
    checkVisible(&args, drawPos, rCamInfo.mViewMtx[index], rCamInfo.mProjectionMtx[index]);

    for (u32 idx = 0; idx < 8; idx++) {
        f32 angle = idx / 4.0f * MR::pi();
        TVec3f vec11C(MR::cos(angle), MR::sin(angle), 0.0f);
        vec11C.scale(mDrawInfo.mRadius[index] * 0.4f);

        mtx.mult(vec11C, vec11C);

        checkVisible(&args, vec11C, rCamInfo.mViewMtx[index], rCamInfo.mProjectionMtx[index]);
    }

    for (u32 idx = 0; idx < 8; idx++) {
        f32 angle = (idx + 0.5f) / 4.0f * MR::pi();
        TVec3f vec128(MR::cos(angle), MR::sin(angle), 0.0f);
        vec128.scale(mDrawInfo.mRadius[index] * 0.7f);

        mtx.mult(vec128, vec128);

        checkVisible(&args, vec128, rCamInfo.mViewMtx[index], rCamInfo.mProjectionMtx[index]);
    }

    setResult(args);
}

void BrightObjBase::checkVisible(BrightObjBase::CheckArg* pArg, const TVec3f& rTrans, const TPos3f& rViewMtx, const TProj3f& rProjMtx) {
    TVec2f screenPos;
    if (::calcScreenPosition(&screenPos, rTrans, rViewMtx, rProjMtx)) {
        TVec2f frameBufferPos;
        MR::convertScreenPosToFrameBufferPos(&frameBufferPos, screenPos);

        u32 color;
        GXPeekARGB(frameBufferPos.x, frameBufferPos.y, &color);

        if ((color & 0xFF000000) >= 0xF0000000) {
            pArg->mCurrBrightness++;
            pArg->mObjCenter += screenPos;
        }
    }

    pArg->mMaxBrightness++;
}

void BrightObjBase::setResult(const BrightObjBase::CheckArg& rArg) {
    f32 ratio = static_cast< f32 >(rArg.mCurrBrightness) / rArg.mMaxBrightness;

    if (rArg.mCurrBrightness != 0) {
        mBrightCenter.set(TVec2f(rArg.mObjCenter * (1.0f / rArg.mCurrBrightness)));
        mBrightness = ratio;
        mNowCenter.set(rArg.mCenter);
        mIsNotVisible = false;
    } else {
        mBrightness = ratio;
        mIsNotVisible = true;
    }

    mInfo.write(mBrightCenter, rArg.mCenter, mBrightness);
}

void BrightObjBase::drawSphere(const TVec3f& rPos, float radius) const {
    TPos3f viewMtx(MR::getCameraViewMtx());

    TPos3f mtx;
    mtx.identity33();
    mtx.setTrans(rPos);
    mtx.concat(viewMtx, mtx);

    TDDraw::setup(0, 0, 1);
    TDDraw::loadViewMtx(mtx);

    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetDstAlpha(GX_FALSE, 0);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);

    TDDraw::drawSphere(TVec3f(0, 0, 0), radius * 1.1f, 0xFF000000, 0x10);

    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    TDDraw::drawSphere(TVec3f(0, 0, 0), radius, 0x00FF00FF, 0x10);

    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
}

BrightObj::BrightObj(const char* pName) : LiveActor(pName), BrightObjBase(), mRadius(::sSunModelScale) {
}

void BrightObj::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mRadius);

    MR::invalidateClipping(this);

    MR::connectToScene(this, MR::MovementType_Environment, -1, -1, MR::DrawType_BrightSun);

    makeActorAppeared();
}

void BrightObj::control() {
    f32 radius = mRadius;
    mDrawInfo.write(MR::getLensFlareDrawSyncTokenIndex(), mPosition, radius);
}

void BrightObj::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    if (MR::isHiddenModel(this)) {
        return;
    }

    switch (MR::isClipped(this)) {
    case false:
        drawSphere(mPosition, mRadius);
        break;
    }
}

void BrightObj::calcBrightInfo(u16 index, const BrightCamInfo& rCamInfo) {
    BrightObjBase::checkVisibilityOfSphere(index, rCamInfo);
}

void BrightObj::getNowCenter(TVec2f* pDst) const {
    ::calcScreenPosition(pDst, mPosition, MR::getCameraViewMtx(), MR::getCameraProjectionMtx());
}

BrightSun::BrightSun(const char* pName) : LiveActor(pName), BrightObjBase(), mSun() {
}

void BrightSun::control() {
    TVec3f camPos = MR::getCamPos();

    TPos3f mtx;
    mtx.identity33();
    mtx.setTrans(0.0f, 0.0f, 100000.0f);

    TPos3f mtxTR;
    MR::makeMtxTR(mtxTR, camPos.x, camPos.y, camPos.z, mRotation.x, mRotation.y, mRotation.z);

    mtxTR.concat(mtx);
    mtxTR.getTrans(mPosition);

    controlSunModel();

    mDrawInfo.write(MR::getLensFlareDrawSyncTokenIndex(), mPosition, ::sSunRadius);
}

void BrightSun::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    if (MR::isHiddenModel(this)) {
        return;
    }

    switch (MR::isClipped(this)) {
    case false:
        drawSphere(mPosition, ::sSunRadius);
        break;
    }
}

void BrightSun::calcBrightInfo(u16 index, const BrightCamInfo& rCamInfo) {
    if (MR::isDead(this)) {
        return;
    }

    checkVisibilityOfSphere(index, rCamInfo);
}

void BrightSun::getNowCenter(TVec2f* pDst) const {
    ::calcScreenPosition(pDst, mPosition, MR::getCameraViewMtx(), MR::getCameraProjectionMtx());
}

void BrightSun::controlSunModel() {
    mSun->mPosition.set(mPosition);
    mSun->mScale.set(::sSunModelScale);

    TVec3f camDir(MR::getCamPos() - mPosition);
    MR::normalize(&camDir);

    TVec3f fwdVec(0.0f, 0.0f, 1.0f);
    TPos3f rotateMtx;
    rotateMtx.identity();
    rotateMtx.setRotate(fwdVec, camDir);

    TVec3f rotation;
    if (fwdVec.dot(camDir) < -0.999f) {
        rotation.set< f32 >(0.0f, 180.0f, 0.0f);
    } else {
        rotateMtx.getEulerXYZ(rotation);
        rotation.scale(_180_PI);
    }

    mSun->mRotation.set(rotation);
}

BrightObj::~BrightObj() {
}

BrightSun::~BrightSun() {
}

BrightObjBase::~BrightObjBase() {
}

void BrightObjBase::endRead() {
    mInfo.endRead();
}

const TVec2f* BrightObjBase::getCenter() const {
    return &mInfo.mRealCenter[mInfo.mCurrIndex];
}

const TVec2f* BrightObjBase::getBrightCenter() const {
    return &mInfo.mBrightnessCenter[mInfo.mCurrIndex];
}

f32 BrightObjBase::getBright() const {
    return mInfo.mBright[mInfo.mCurrIndex];
}

void BrightObjBase::getNowCenter(TVec2f* pDst) const {
    pDst->set(mNowCenter);
}

void BrightSun::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);

    MR::invalidateClipping(this);

    MR::connectToScene(this, MR::MovementType_Environment, -1, -1, MR::DrawType_BrightSun);

    mSun = new Sun("太陽");
    mSun->initWithoutIter();

    MR::tryRegisterDemoCast(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}
