#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>

void SpinDriverPathDrawer_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

namespace {
    inline u32 packPathColor(const TVec3f& rColor, const f32& rAlpha) {
        u32 result = 0;
        reinterpret_cast< u8* >(&result)[0] = static_cast< s32 >(255.0f * MR::abs(rColor.x));
        reinterpret_cast< u8* >(&result)[1] = static_cast< s32 >(255.0f * MR::abs(rColor.y));
        reinterpret_cast< u8* >(&result)[2] = static_cast< s32 >(255.0f * MR::abs(rColor.z));
        reinterpret_cast< u8* >(&result)[3] = static_cast< s32 >(255.0f * MR::abs(rAlpha));
        return result;
    }

    static const f32 sDrawMinInterval = 0.0f;
    static const f32 sDrawCurveLimit = 0.0f;
    static const f32 sLineWidth = 0.0f;
};  // namespace

SpinDriverPathDrawInit::SpinDriverPathDrawInit()
    : NameObj("スピンドライバーレール描画初期化"), mOrangeTexture(), mGreenTexture(), mPinkTexture(), mMaskTexture(), mIsPathAtOpa() {
    MR::registerPreDrawFunction(MR::Functor(this, &SpinDriverPathDrawInit::initDraw), MR::DrawType_SpinDriverPathDrawer);

    mOrangeTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "NormalColor.bti"), 0);
    mGreenTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "Green.bti"), 0);
    mPinkTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "Pink.bti"), 0);
    mMaskTexture = new JUTTexture(MR::loadTexFromArc("SpinDriverPath.arc", "Mask.bti"), 0);
}

SpinDriverPathDrawer::SpinDriverPathDrawer(SpinDriverShootPath* pShootPath)
    : LiveActor("パス描画"), mShootPath(pShootPath), _90(), _94(), _98(), _9C(), mPositionCount(), _A4(), _A8(), _AC(), _B0(), mColor(-1), _B8(),
      mFadeScale(1.0f), mMaskLength(5000.0f) {
    MR::createSceneObj(SceneObj_SpinDriverPathDrawInit);
}

void SpinDriverPathDrawInit::initDraw() {
    TDDraw::cameraInit3D();

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 30, GX_FALSE, 125);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 33, GX_FALSE, 125);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_CPREV, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, Color8(0));
    GXSetFogRangeAdj(GX_FALSE, 0, nullptr);
    mMaskTexture->load(GX_TEXMAP1);
}

namespace MR {
    void setSpinDriverPathColorNormal() {
        MR::getSceneObj< SpinDriverPathDrawInit >(SceneObj_SpinDriverPathDrawInit)->mOrangeTexture->load(GX_TEXMAP0);
    }

    void setSpinDriverPathColorGreen() {
        MR::getSceneObj< SpinDriverPathDrawInit >(SceneObj_SpinDriverPathDrawInit)->mGreenTexture->load(GX_TEXMAP0);
    }

    void setSpinDriverPathColorPink() {
        MR::getSceneObj< SpinDriverPathDrawInit >(SceneObj_SpinDriverPathDrawInit)->mPinkTexture->load(GX_TEXMAP0);
    }

    bool isDrawSpinDriverPathAtOpa() {
        if (!MR::isExistSceneObj(SceneObj_SpinDriverPathDrawInit)) {
            return false;
        }

        return MR::getSceneObj< SpinDriverPathDrawInit >(SceneObj_SpinDriverPathDrawInit)->mIsPathAtOpa;
    }

    void onDrawSpinDriverPathAtOpa() {
        MR::getSceneObj< SpinDriverPathDrawInit >(SceneObj_SpinDriverPathDrawInit)->mIsPathAtOpa = true;
    }

    void offDrawSpinDriverPathAtOpa() {
        MR::getSceneObj< SpinDriverPathDrawInit >(SceneObj_SpinDriverPathDrawInit)->mIsPathAtOpa = false;
    }
};  // namespace MR

void SpinDriverPathDrawer::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_SpinDriverPathDrawer);

    initPositionList(75.0f, 20.0f);
    initClipping();
    initPathEnd();

    makeActorDead();
}

s32 SpinDriverPathDrawer::calcPositionCount(f32 f1, f32 f2) const {
    s32 totalSteps = mShootPath->getTotalLength() / f1;

    TVec3f pos0;
    mShootPath->calcPosition(&pos0, 0.0f);

    TVec3f dir0;
    mShootPath->calcDirection(&dir0, 0.0f, 0.01f);

    s32 count = 2;

    for (s32 i = 1; i < totalSteps; i++) {
        f32 posStep = static_cast< f32 >(i) / totalSteps;

        TVec3f posI;
        mShootPath->calcPosition(&posI, posStep);

        TVec3f pos;
        pos.killElement(posI - pos0, dir0);

        if (pos.length() > f2) {
            pos0 = posI;
            mShootPath->calcDirection(&dir0, posStep, 0.01f);

            count++;
        }
    }

    return count;
}

void SpinDriverPathDrawer::initPositionList(f32 f1, f32 f2) {
    mPositionCount = calcPositionCount(f1, f2);
    _90 = new (32) TVec3f[mPositionCount];
    _94 = new (32) TVec3f[mPositionCount];
    _98 = new (32) TVec3f[mPositionCount];
    _9C = new f32[mPositionCount];

    s32 totalSteps = mShootPath->getTotalLength() / f1;

    TVec3f pos0;
    mShootPath->calcPosition(&pos0, 0.0f);

    TVec3f dir0;
    mShootPath->calcDirection(&dir0, 0.0f, 0.01f);

    _90[0] = pos0;
    _9C[0] = 0.0f;

    MR::makeAxisVerticalZX(&_98[0], dir0);

    _94[0].cross(_98[0], dir0);
    MR::normalize(&_94[0]);

    s32 count = 1;

    for (s32 i = 1; i < totalSteps; i++) {
        f32 posStep = static_cast< f32 >(i) / totalSteps;

        TVec3f posI;
        mShootPath->calcPosition(&posI, posStep);

        TVec3f pos;
        pos.killElement(posI - pos0, dir0);

        if (pos.length() > f2) {
            pos0 = posI;

            mShootPath->calcDirection(&dir0, posStep, 0.01f);

            if (MR::abs(dir0.dot(_94[count - 1])) > MR::abs(dir0.dot(_98[count - 1]))) {
                MR::makeAxisFrontSide(&_98[count], &_94[count], dir0, _94[count - 1]);
            } else {
                MR::makeAxisFrontUp(&_94[count], &_98[count], dir0, _98[count - 1]);
            }

            _90[count] = posI;
            _9C[count] = posStep;

            count++;
        }
    }

    mShootPath->calcPosition(&_90[count], 1.0f);
    _94[count] = _94[count - 1];
    _98[count] = _98[count - 1];
    _9C[count] = 1.0f;
}

void SpinDriverPathDrawer::initClipping() {
    f32 radius = 0.0f;
    mShootPath->calcClippingInfo(&mPosition, &radius, 150.0f, 150.0f);
    MR::setClippingTypeSphere(this, radius);

    MR::validateClipping(this);
    MR::setClippingFarMax(this);
}

void SpinDriverPathDrawer::initPathEnd() {
    s32 arg1 = -1;
    MR::getRailArg1WithInit(mShootPath->mRailRider, &arg1);

    if (arg1 >= 0) {
        _B8 = arg1 / 1000.0f;
    } else {
        _B8 = 0.0f;
    }
}

f32 SpinDriverPathDrawer::calcDrawCode() const {
    if (_B8 <= 0.0f) {
        return _AC;
    }

    f32 val = 1.0f - _B8;
    return _AC < val ? _AC : val;
}

f32 SpinDriverPathDrawer::calcDrawBaseCode() const {
    if (_B8 <= 0.0f) {
        return _B0;
    }

    f32 val = _B0;

    if (val < (1.0f - _B8)) {
        return val;
    }

    return 1.0f - _B8;
}

void SpinDriverPathDrawer::setCoord(f32 coord) {
    _AC = coord;

    if (_B0 < coord) {
        _B0 = coord;
    }
}

void SpinDriverPathDrawer::setColorNormal() {
    mColor = -1;
}

void SpinDriverPathDrawer::setColorGreen() {
    mColor = 0;
}

void SpinDriverPathDrawer::setColorPink() {
    mColor = 1;
}

void SpinDriverPathDrawer::setMaskLength(f32 mask) {
    if (mask < 1.0f) {
        mMaskLength = 1.0f;
    } else {
        mMaskLength = mask;
    }
}

void SpinDriverPathDrawer::setFadeScale(f32 fade) {
    if (fade < 0.001f) {
        mFadeScale = 0.001f;
    } else {
        mFadeScale = fade;
    }
}

void SpinDriverPathDrawer::draw() const {
    switch (mColor) {
    case -1:
        MR::setSpinDriverPathColorNormal();

        break;
    case 0:
        MR::setSpinDriverPathColorGreen();

        break;
    case 1:
        MR::setSpinDriverPathColorPink();

        break;
    default:
        MR::setSpinDriverPathColorNormal();

        break;
    }

    f32 drawCode = calcDrawCode();
    f32 drawBaseCode = calcDrawBaseCode();
    f32 maskSteps = mShootPath->getTotalLength() / mMaskLength;

    TPos3f mtx;
    mtx.identity();
    mtx[1][1] = -maskSteps;
    mtx[1][3] = 0.5f + maskSteps * drawCode;

    GXLoadTexMtxImm(mtx, 33, GX_MTX2x4);

    mtx.identity();
    mtx[1][1] = 1.0f / mFadeScale;
    mtx[1][3] = 1.0f - drawBaseCode * mtx[1][1];

    GXLoadTexMtxImm(mtx, 30, GX_MTX2x4);

    for (u32 i = 1; i < mPositionCount; i++) {
        f32 alpha;
        f32 green;
        alpha = 2.0f * _9C[i] - 1.0f;
        green = 1.0f - _9C[i];
        const f32 square = alpha * alpha;
        alpha = square * square;
        u32 color = ::packPathColor(TVec3f(1.0f, green, 0.0f), 1.0f - alpha);

        TVec3f prev90(_90[i - 1]), curr90(_90[i]);
        TVec3f prev94(_94[i - 1] * 100.0f), curr94(_94[i] * 100.0f);
        TVec3f prev98(_98[i - 1] * 100.0f), curr98(_98[i] * 100.0f);

        f32 prevVal = _9C[i - 1];
        f32 val = _9C[i];

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 6);

        sendPoint(prev90 - prev98, 0.0f, prevVal);
        sendPoint(curr90 - curr98, 0.0f, val);
        sendPoint(prev90, 0.5f, prevVal);
        sendPoint(curr90, 0.5f, val);
        sendPoint(prev90 + prev98, 1.0f, prevVal);
        sendPoint(curr90 + curr98, 1.0f, val);

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 6);

        sendPoint(prev90 - prev94, 0.0f, prevVal);
        sendPoint(curr90 - curr94, 0.0f, val);
        sendPoint(prev90, 0.5f, prevVal);
        sendPoint(curr90, 0.5f, val);
        sendPoint(prev90 + prev94, 1.0f, prevVal);
        sendPoint(curr90 + curr94, 1.0f, val);
    }
}

void SpinDriverPathDrawer::sendPoint(const TVec3f& rA1, f32 a2, f32 a3) const {
    GXWGFifo.f32 = rA1.x;
    GXWGFifo.f32 = rA1.y;
    GXWGFifo.f32 = rA1.z;
    GXWGFifo.f32 = a2;
    GXWGFifo.f32 = a3;
}
