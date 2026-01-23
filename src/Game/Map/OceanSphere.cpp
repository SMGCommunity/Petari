#include "Game/Map/OceanSphere.hpp"
#include "Game/Map/OceanSpherePoint.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"
#include <JSystem/JGeometry.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <math_types.hpp>
#include <revolution/gd/GDBase.h>
#include <revolution/gx.h>
#include <revolution/gx/GXVert.h>
#include <revolution/mtx.h>
#include <revolution/os/OSCache.h>
#include <cmath>

extern const u8 sOceanSphereTevReg0FaceR;
extern const u8 sOceanSphereTevReg0FaceG;
extern const u8 sOceanSphereTevReg0FaceB;
extern const u8 sOceanSphereTevReg0FaceA;
extern const u8 sOceanSphereTevReg1FrontDefaultR;
extern const u8 sOceanSphereTevReg1FrontDefaultG;
extern const u8 sOceanSphereTevReg1FrontDefaultB;
extern const u8 sOceanSphereTevReg1FrontDefaultA;
extern const u8 sOceanSphereTevReg1FrontTearR;
extern const u8 sOceanSphereTevReg1FrontTearG;
extern const u8 sOceanSphereTevReg1FrontTearB;
extern const u8 sOceanSphereTevReg1FrontTearA;
extern const u8 sOceanSphereTevReg2FaceR;
extern const u8 sOceanSphereTevReg2FaceG;
extern const u8 sOceanSphereTevReg2FaceB;
extern const u8 sOceanSphereTevReg2FaceA;
extern const u8 sOceanSphereKColor0FaceR;
extern const u8 sOceanSphereKColor0FaceG;
extern const u8 sOceanSphereKColor0FaceB;
extern const u8 sOceanSphereKColor0FaceA;
extern const u8 sOceanSphereTevReg0BackR;
extern const u8 sOceanSphereTevReg0BackG;
extern const u8 sOceanSphereTevReg0BackB;
extern const u8 sOceanSphereTevReg0BackA;
extern const u8 sOceanSphereTevReg1BackDefaultR;
extern const u8 sOceanSphereTevReg1BackDefaultG;
extern const u8 sOceanSphereTevReg1BackDefaultB;
extern const u8 sOceanSphereTevReg1BackDefaultA;
extern const u8 sOceanSphereTevReg1BackTearR;
extern const u8 sOceanSphereTevReg1BackTearG;
extern const u8 sOceanSphereTevReg1BackTearB;
extern const u8 sOceanSphereTevReg1BackTearA;
extern const u8 sOceanSphereTevReg2BackR;
extern const u8 sOceanSphereTevReg2BackG;
extern const u8 sOceanSphereTevReg2BackB;
extern const u8 sOceanSphereTevReg2BackA;
extern const u8 sOceanSphereKColor0BackR;
extern const u8 sOceanSphereKColor0BackG;
extern const u8 sOceanSphereKColor0BackB;
extern const u8 sOceanSphereKColor0BackA;
extern const u8 sOceanSphereTevReg0EnvR;
extern const u8 sOceanSphereTevReg0EnvG;
extern const u8 sOceanSphereTevReg0EnvB;
extern const u8 sOceanSphereTevReg0EnvA;
extern const u8 sOceanSphereTevReg1EnvR;
extern const u8 sOceanSphereTevReg1EnvG;
extern const u8 sOceanSphereTevReg1EnvB;
extern const u8 sOceanSphereTevReg1EnvA;
extern const u8 sOceanSphereFogFaceR;
extern const u8 sOceanSphereFogFaceG;
extern const u8 sOceanSphereFogFaceB;
extern const u8 sOceanSphereFogFaceA;
extern const u8 sOceanSphereFogBackR;
extern const u8 sOceanSphereFogBackG;
extern const u8 sOceanSphereFogBackB;
extern const u8 sOceanSphereFogBackA;

struct SinCosPair {
    f32 sin;
    f32 cos;
};

extern SinCosPair lbl_8060FC80[];

inline f32 yy(f32 y) {
    return y * y;
}

inline f32 cosLapRad(f32 v) {
    if (v < 0.0f) {
        v = -v;
    }

    v *= ((1 << 14) / TWO_PI);
    return lbl_8060FC80[(u16)v & ((1 << 14) - 1)].cos;
}

template <>
void TRot3f::setRotate(const TVec3f& rAxis, f32 angle) {
    TVec3f v;
    v.set< f32 >(rAxis);
    v.length();
    PSVECNormalize(&v, &v);
    f32 angley = sin(angle), anglex = cos(angle);
    f32 x, y, z;
    y = v.y;
    x = v.x;
    z = v.z;
    mMtx[0][0] = anglex + (1.0f - anglex) * yy(x);
    mMtx[0][1] = (1.0f - anglex) * x * y - angley * z;
    mMtx[0][2] = (1.0f - anglex) * x * z + angley * y;
    mMtx[1][0] = (1.0f - anglex) * x * y + angley * z;
    mMtx[1][1] = anglex + (1.0f - anglex) * yy(y);
    mMtx[1][2] = (1.0f - anglex) * y * z - angley * x;
    mMtx[2][0] = (1.0f - anglex) * x * z - angley * y;
    mMtx[2][1] = (1.0f - anglex) * y * z + angley * x;
    mMtx[2][2] = anglex + (1.0f - anglex) * yy(z);
}

namespace {
    Vec sAxisX = {1.0f, 0.0f, 0.0f};
    Vec sAxisY = {0.0f, 1.0f, 0.0f};
    Vec sAxisZ = {0.0f, 0.0f, 1.0f};

    const char sWaterWaveArc[] = "WaterWave.arc";
    const char sOceanSphereBti[] = "OceanSphere.bti";
    const char sOceanSphereEnvRefBti[] = "OceanSphereEnvRef.bti";
    const char sSkullSharkGalaxy[] = "SkullSharkGalaxy";
    const char sTearDropGalaxy[] = "TearDropGalaxy";
    const char sDemoPartName[] = "\x97\x4E\x82\xAB\x90\x85\x8F\xE3\x8F\xB8";

    extern TVec3f sPosAxisX;
    extern TVec3f sPosAxisY;
    extern TVec3f sPosAxisZ;
};

namespace NrvOceanSphere {
    NEW_NERVE(OceanSphereNrvWait, OceanSphere, Wait);
    NEW_NERVE(OceanSphereNrvRiseUp, OceanSphere, RiseUp);
};

class OceanSpherePlane {
public:
    OceanSpherePlane(s32 pointCount, const TVec3f* pCenter, const TVec3f& rAxis1, const TVec3f& rAxis2, const TVec2f& rTex1,
        const TVec2f& rTex2, const TVec2f& rTex3);

    void update(f32, f32, f32);
    OceanSpherePoint* getPoint(int, int) const;

    s32 mGridPointCount;
    s32 mAxisPointCount;
    OceanSpherePoint** mPoints;
};

class OceanSpherePlaneEdge {
public:
    OceanSpherePlaneEdge(s32 pointCount, const TVec3f* pCenter, const TVec3f& rAxis1, const TVec3f& rAxis2, const TVec2f& rTex1,
        const TVec2f& rTex2);

    void update(f32, f32, f32);

    s32 mPointCount;
    OceanSpherePoint** mPoints;
};

namespace {
    u32 calcDisplayListSize(u32 count, u32 stride) {
        u32 t = (count + 1) * count;
        t &= ~1U;
        t -= count;
        t--;
        u32 size = stride * t;
        t--;
        size += t * 3;
        u32 blocks = (size >> 2) & 0x07FFFFFF;
        return (blocks + 2) << 5;
    }

    void beginDrawPolygon(int idx, s32 pointCount, bool useGD) {
        if (useGD) {
            GDOverflowCheck(1);
            *__GDCurrentDL->ptr++ = GX_TRIANGLESTRIP;
            GDOverflowCheck(2);
            u16 vtxCount = (u16)(((pointCount - 1 - idx) * 2 + 1));
            *__GDCurrentDL->ptr++ = (u8)(vtxCount >> 8);
            *__GDCurrentDL->ptr++ = (u8)vtxCount;
        } else {
            u16 vtxCount = (u16)(((pointCount - 1 - idx) * 2 + 1));
            GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, vtxCount);
        }
    }
};

OceanSpherePlane::OceanSpherePlane(s32 pointCount, const TVec3f* pCenter, const TVec3f& rAxis1, const TVec3f& rAxis2,
    const TVec2f& rTex1, const TVec2f& rTex2, const TVec2f& rTex3) {
    mAxisPointCount = pointCount - 2;
    mGridPointCount = mAxisPointCount * mAxisPointCount;
    mPoints = nullptr;
    mPoints = new OceanSpherePoint*[mGridPointCount];

    TRot3f rot;
    rot.identity();
    TVec2f texCenter;
    texCenter.x = 0.5f;
    texCenter.y = 0.5f;

    for (s32 row = 1; row < pointCount - 1; row++) {
        f32 rowRate = row / (pointCount - 1.0f);
        rot.setRotate(rAxis2, rowRate * 1.5707964f);
        TVec3f vec(rAxis1);
        rot.mult(vec, vec);
        f32 rowInv = 1.0f - rowRate;
        TVec2f texTempA = rTex1 * rowRate;
        TVec2f texTempB = texCenter * rowInv;
        TVec2f texA = texTempA + texTempB;
        TVec2f texTempC = rTex2 * rowRate;
        TVec2f texTempD = texCenter * rowInv;
        TVec2f texB = texTempC + texTempD;
        TVec2f texTempE = rTex3 * rowRate;
        TVec2f texTempF = texCenter * rowInv;
        TVec2f texC = texTempE + texTempF;

        for (s32 col = 1; col <= (2 * row - 1); col++) {
            rot.setRotate(rAxis1, (col / (row * 1.0f)) * 3.1415927f);
            TVec3f normal(vec);
            rot.mult(normal, normal);

            if (col <= row) {
                f32 rate = col / (row * 1.0f);
                f32 rateInv = 1.0f - rate;
                TVec2f texTemp1 = texB * rateInv;
                TVec2f texTemp2 = texA * rate;
                TVec2f tex = texTemp1 + texTemp2;
                s32 idx = (row - 1) * mAxisPointCount + (col - 1);
                mPoints[idx] = new OceanSpherePoint(pCenter, normal, row, col, tex);
            } else {
                f32 rate = (col - row) / (row * 1.0f);
                f32 rateInv = 1.0f - rate;
                TVec2f texTemp1 = texC * rate;
                TVec2f texTemp2 = texA * rateInv;
                TVec2f tex = texTemp1 + texTemp2;
                s32 idx = (row - 1) * (mAxisPointCount + 1) - mAxisPointCount * (col - row);
                mPoints[idx] = new OceanSpherePoint(pCenter, normal, row, col, tex);
            }
        }
    }
}

void OceanSpherePlane::update(f32 radius, f32 wave1Time, f32 wave2Time) {
    for (s32 row = 0; row < mAxisPointCount; row++) {
        for (s32 col = 0; col < mAxisPointCount; col++) {
            s32 idx = (row * mAxisPointCount) + col;
            mPoints[idx]->updatePos(radius, wave1Time, wave2Time);
        }
    }
}

OceanSpherePoint* OceanSpherePlane::getPoint(int row, int col) const {
    return mPoints[(row * mAxisPointCount) + col];
}

OceanSpherePlaneEdge::OceanSpherePlaneEdge(s32 pointCount, const TVec3f* pCenter, const TVec3f& rAxis1, const TVec3f& rAxis2,
    const TVec2f& rTex1, const TVec2f& rTex2) {
    s32 pointCountMinus2 = pointCount - 2;
    s32 pointCountMinus1 = pointCount - 1;
    mPointCount = pointCountMinus2;
    mPoints = nullptr;
    mPoints = new OceanSpherePoint*[mPointCount];
    TVec3f axis;
    PSVECCrossProduct(&rAxis1, &rAxis2, &axis);
    MR::normalize(&axis);
    TRot3f rot;
    rot.identity();
    rot.setRotate(axis, (1.0f / pointCountMinus1) * 1.5707964f);
    TVec3f vec(rAxis1);

    s32 idx = 0;
    for (s32 i = 0; i < mPointCount; i++) {
        rot.mult(vec, vec);
        f32 rate = (i + 1) / (pointCountMinus1 * 1.0f);
        f32 rateInv = 1.0f - rate;
        TVec2f temp1 = rTex2 * rate;
        TVec2f temp2 = rTex1 * rateInv;
        TVec2f temp3 = temp2 + temp1;
        TVec2f localTex;
        localTex.x = temp3.x;
        localTex.y = temp3.y;
        mPoints[idx] = new OceanSpherePoint(pCenter, vec, i, i, localTex);
        idx++;
    }
}

void OceanSpherePlaneEdge::update(f32 radius, f32 wave1Time, f32 wave2Time) {
    for (s32 i = 0; i < mPointCount; i++) {
        mPoints[i]->updatePos(radius, wave1Time, wave2Time);
    }
}

OceanSphere::OceanSphere(const char* pName)
    : LiveActor(pName),
      mRadius(0.0f),
      mRadiusTarget(0.0f),
      mPointCount(0),
      mAxisPointPX(nullptr),
      mAxisPointNX(nullptr),
      mAxisPointPY(nullptr),
      mAxisPointNY(nullptr),
      mAxisPointPZ(nullptr),
      mAxisPointNZ(nullptr),
      mPlaneLeftUpper(nullptr),
      mPlaneRightUpper(nullptr),
      mPlaneLeftLower(nullptr),
      mPlaneRightLower(nullptr),
      mEdge0(nullptr),
      mEdge1(nullptr),
      mEdge2(nullptr),
      mEdge3(nullptr),
      mEdge4(nullptr),
      mEdge5(nullptr),
      mEdge6(nullptr),
      mEdge7(nullptr),
      mWaveTime1(0.0f),
      mWaveTime2(0.0f),
      mTexOffs0X(0.0f),
      mTexOffs0Y(0.0f),
      mTexOffs1X(0.0f),
      mTexOffs1Y(0.0f),
      mWaterTex(nullptr),
      mWaterEnvTex(nullptr) {
    GXColor front;
    front.r = sOceanSphereTevReg1FrontDefaultR;
    front.g = sOceanSphereTevReg1FrontDefaultG;
    front.b = sOceanSphereTevReg1FrontDefaultB;
    front.a = sOceanSphereTevReg1FrontDefaultA;
    mTevReg1Front = front;

    GXColor back;
    back.r = sOceanSphereTevReg1BackDefaultR;
    back.g = sOceanSphereTevReg1BackDefaultG;
    back.b = sOceanSphereTevReg1BackDefaultB;
    back.a = sOceanSphereTevReg1BackDefaultA;
    mTevReg1Back = back;

    mUseDisplayList = false;
    mDispListFaceLen = 0;
    mDispListFace = nullptr;
    mDispListBackLen = 0;
    mDispListBack = nullptr;
    mAlwaysUseRealDrawing = false;
    mIsCameraInside = false;
    mIsStartPosCamera = true;
    mIsTearDrop = false;
    mEnableStartPosCameraSwitch = false;
}

void OceanSphere::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_OceanSphere);
    MR::initDefaultPos(this, rIter);
    mRadius = 100.0f * mScale.x;
    mRadiusTarget = mRadius;
    initPoints();
    updatePoints();
    initDisplayList();
    WaterAreaFunction::entryOceanSphere(this);

    mWaterTex = new JUTTexture(MR::loadTexFromArc(sWaterWaveArc, sOceanSphereBti), 0);
    mWaterEnvTex = new JUTTexture(MR::loadTexFromArc(sWaterWaveArc, sOceanSphereEnvRefBti), 0);

    MR::setClippingTypeSphere(this, mRadius);
    MR::setClippingFarMax(this);
    initNerve(&NrvOceanSphere::OceanSphereNrvWait::sInstance);

    if (MR::isEqualStageName(sSkullSharkGalaxy)) {
        mEnableStartPosCameraSwitch = true;
        mAlwaysUseRealDrawing = true;
    }

    if (MR::isEqualStageName(sTearDropGalaxy)) {
        mIsTearDrop = true;
        mAlwaysUseRealDrawing = true;
        mIsStartPosCamera = false;
        GXColor front;
        front = (GXColor){sOceanSphereTevReg1FrontTearR, sOceanSphereTevReg1FrontTearG, sOceanSphereTevReg1FrontTearB, sOceanSphereTevReg1FrontTearA};
        mTevReg1Front = front;
        GXColor back;
        back = (GXColor){sOceanSphereTevReg1BackTearR, sOceanSphereTevReg1BackTearG, sOceanSphereTevReg1BackTearB, sOceanSphereTevReg1BackTearA};
        mTevReg1Back = back;
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        makeActorDead();
    } else {
        MR::registerDemoSimpleCastAll(this);
        makeActorAppeared();
    }
}

void OceanSphere::appear() {
    LiveActor::appear();
    mRadius = 1000.0f;
    setNerve(&NrvOceanSphere::OceanSphereNrvRiseUp::sInstance);
}

void OceanSphere::exeWait() {}

void OceanSphere::exeRiseUp() {
    f32 rate = MR::calcDemoPartStepRate(sDemoPartName);
    mRadius = 1000.0f + (rate * (mRadiusTarget - 1000.0f));

    if (MR::isDemoPartLastStep(sDemoPartName)) {
        setNerve(&NrvOceanSphere::OceanSphereNrvWait::sInstance);
    }
}

bool OceanSphere::isInWater(const TVec3f& rPos) const {
    if (MR::isDead(this)) {
        return false;
    }

    return PSVECDistance(&rPos, &mPosition) <= mRadius;
}

bool OceanSphere::calcWaterInfo(const TVec3f& rPos, const TVec3f& rGravity, WaterInfo* pInfo) const {
    TVec3f v = rPos - mPosition;
    TVec3f negGrav = -rGravity;
    f32 f30 = MR::vecKillElement(v, negGrav, &v);
    f32 theta = (v.length() / mRadius) * 0.5f * 3.1415927f;
    f32 cosv = cosLapRad(theta);
    pInfo->mCamWaterDepth = mRadius * cosv - f30;
    pInfo->_4 = mRadius * cosv + f30;
    TVec3f normal = rPos - mPosition;
    MR::normalizeOrZero(&normal);
    pInfo->mSurfaceNormal.set(normal);
    TVec3f scaled(normal);
    scaled.x *= mRadius;
    scaled.y *= mRadius;
    scaled.z *= mRadius;
    TVec3f surface = mPosition + scaled;
    pInfo->mSurfacePos.set(surface);
    return true;
}

void OceanSphere::initPoints() {
    const TVec3f* axisBase = &sPosAxisX;

    if (mRadius <= 300.0f) {
        mPointCount = 8;
    } else if (mRadius <= 500.0f) {
        mPointCount = 10;
    } else if (mRadius <= 1000.0f) {
        mPointCount = 15;
    } else {
        mPointCount = 20;
    }

    OceanSpherePoint* point;

    // mAxisPointPX: tex = (0.0f, 1.0f), axis = axisBase[0]
    point = new OceanSpherePoint(&mPosition, axisBase[0], 1.0f, 1.0f, TVec2f(0.0f, 1.0f));
    mAxisPointPX = point;

    // mAxisPointNX: tex = (1.0f, 0.0f), axis = -axisBase[0]
    point = new OceanSpherePoint(&mPosition, -axisBase[0], 1.0f, 1.0f, TVec2f(1.0f, 0.0f));
    mAxisPointNX = point;

    // mAxisPointPY: tex = (0.5f, 0.5f), axis = axisBase[1]
    point = new OceanSpherePoint(&mPosition, axisBase[1], 1.0f, 1.0f, TVec2f(0.5f, 0.5f));
    mAxisPointPY = point;

    // mAxisPointNY: tex = (0.5f, 0.5f), axis = -axisBase[1]
    point = new OceanSpherePoint(&mPosition, -axisBase[1], 1.0f, 1.0f, TVec2f(0.5f, 0.5f));
    mAxisPointNY = point;

    // mAxisPointPZ: tex = (0.0f, 0.0f), axis = axisBase[2]
    point = new OceanSpherePoint(&mPosition, axisBase[2], 1.0f, 1.0f, TVec2f(0.0f, 0.0f));
    mAxisPointPZ = point;

    // mAxisPointNZ: tex = (1.0f, 1.0f), axis = -axisBase[2]
    point = new OceanSpherePoint(&mPosition, -axisBase[2], 1.0f, 1.0f, TVec2f(1.0f, 1.0f));
    mAxisPointNZ = point;

    OceanSpherePlane* plane;

    // mPlaneLeftUpper: tex1=(1.0f, 0.0f), tex2=(1.0f, 1.0f), tex3=(0.0f, 0.0f), axis1=Y, axis2=-X
    plane = new OceanSpherePlane(mPointCount, &mPosition, axisBase[1], -axisBase[0],
        TVec2f(1.0f, 0.0f), TVec2f(1.0f, 1.0f), TVec2f(0.0f, 0.0f));
    mPlaneLeftUpper = plane;

    // mPlaneRightUpper: tex1=(0.0f, 1.0f), tex2=(0.0f, 0.0f), tex3=(1.0f, 1.0f), axis1=Y, axis2=X
    plane = new OceanSpherePlane(mPointCount, &mPosition, axisBase[1], axisBase[0],
        TVec2f(0.0f, 1.0f), TVec2f(0.0f, 0.0f), TVec2f(1.0f, 1.0f));
    mPlaneRightUpper = plane;

    // mPlaneLeftLower: tex1=(1.0f, 0.0f), tex2=(0.0f, 0.0f), tex3=(1.0f, 1.0f), axis1=-X, axis2=-Y
    plane = new OceanSpherePlane(mPointCount, &mPosition, -axisBase[0], -axisBase[1],
        TVec2f(1.0f, 0.0f), TVec2f(0.0f, 0.0f), TVec2f(1.0f, 1.0f));
    mPlaneLeftLower = plane;

    // mPlaneRightLower: tex1=(0.0f, 1.0f), tex2=(1.0f, 1.0f), tex3=(0.0f, 0.0f), axis1=-Y, axis2=X
    plane = new OceanSpherePlane(mPointCount, &mPosition, -axisBase[1], axisBase[0],
        TVec2f(0.0f, 1.0f), TVec2f(1.0f, 1.0f), TVec2f(0.0f, 0.0f));
    mPlaneRightLower = plane;

    // mEdge0: tex1=(0.5f, 0.5f), tex2=(0.0f, 0.0f), axis1=Y, axis2=Z
    mEdge0 = new OceanSpherePlaneEdge(mPointCount, &mPosition, axisBase[1], axisBase[2],
        TVec2f(0.5f, 0.5f), TVec2f(0.0f, 0.0f));

    // mEdge1: tex1=(0.5f, 0.5f), tex2=(1.0f, 1.0f), axis1=Y, axis2=-Z
    mEdge1 = new OceanSpherePlaneEdge(mPointCount, &mPosition, axisBase[1], -axisBase[2],
        TVec2f(0.5f, 0.5f), TVec2f(1.0f, 1.0f));

    // mEdge2: tex1=(0.5f, 0.5f), tex2=(0.0f, 0.0f), axis1=-Y, axis2=Z
    mEdge2 = new OceanSpherePlaneEdge(mPointCount, &mPosition, -axisBase[1], axisBase[2],
        TVec2f(0.5f, 0.5f), TVec2f(0.0f, 0.0f));

    // mEdge3: tex1=(0.5f, 0.5f), tex2=(1.0f, 1.0f), axis1=-Z, axis2=-Y
    mEdge3 = new OceanSpherePlaneEdge(mPointCount, &mPosition, -axisBase[2], -axisBase[1],
        TVec2f(0.5f, 0.5f), TVec2f(1.0f, 1.0f));

    // mEdge4: tex1=(0.0f, 0.0f), tex2=(1.0f, 0.0f), axis1=Z, axis2=-X
    mEdge4 = new OceanSpherePlaneEdge(mPointCount, &mPosition, axisBase[2], -axisBase[0],
        TVec2f(0.0f, 0.0f), TVec2f(1.0f, 0.0f));

    // mEdge5: tex1=(0.0f, 0.0f), tex2=(0.0f, 1.0f), axis1=Z, axis2=X
    mEdge5 = new OceanSpherePlaneEdge(mPointCount, &mPosition, axisBase[2], axisBase[0],
        TVec2f(0.0f, 0.0f), TVec2f(0.0f, 1.0f));

    // mEdge6: tex1=(1.0f, 1.0f), tex2=(1.0f, 0.0f), axis1=-X, axis2=-Z
    mEdge6 = new OceanSpherePlaneEdge(mPointCount, &mPosition, -axisBase[0], -axisBase[2],
        TVec2f(1.0f, 1.0f), TVec2f(1.0f, 0.0f));

    // mEdge7: tex1=(1.0f, 1.0f), tex2=(0.0f, 1.0f), axis1=-Z, axis2=X
    mEdge7 = new OceanSpherePlaneEdge(mPointCount, &mPosition, -axisBase[2], axisBase[0],
        TVec2f(1.0f, 1.0f), TVec2f(0.0f, 1.0f));
}

void OceanSphere::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);
    u32 sizeFace = calcDisplayListSize(mPointCount, 0x1C);
    mDispListFace = new (0x20) u8[sizeFace];
    DCInvalidateRange(mDispListFace, sizeFace);
    GDLObj obj;
    GDInitGDLObj(&obj, mDispListFace, sizeFace);
    __GDCurrentDL = &obj;
    drawSphere(false, true);
    GDPadCurr32();
    mDispListFaceLen = obj.ptr - obj.start;
    DCStoreRange(mDispListFace, sizeFace);
    u32 sizeBack = calcDisplayListSize(mPointCount, 0x18);
    mDispListBack = new (0x20) u8[sizeBack];
    DCInvalidateRange(mDispListBack, sizeBack);
    GDInitGDLObj(&obj, mDispListBack, sizeBack);
    __GDCurrentDL = &obj;
    drawSphere(true, true);
    GDPadCurr32();
    mDispListBackLen = obj.ptr - obj.start;
    DCStoreRange(mDispListBack, sizeBack);
}

void OceanSphere::control() {
    mIsCameraInside = false;

    if (WaterAreaFunction::getCameraWaterInfo()->mOceanSphere == this) {
        mIsCameraInside = true;

        if (mEnableStartPosCameraSwitch && mIsStartPosCamera && MR::isStartPosCameraEnd()) {
            mIsStartPosCamera = false;
        }
    }

    mWaveTime1 += -0.1f;
    mWaveTime2 += -0.1f;

    if (!mAlwaysUseRealDrawing && MR::calcDistanceToPlayer(this) > 10000.0f) {
        mUseDisplayList = true;
    } else {
        mUseDisplayList = false;
        updatePoints();
    }

    const f32 add = 0.0008f;
    const f32 sub = -0.0008f;

    mTexOffs0X = MR::repeat(add + mTexOffs0X, 0.0f, 1.0f);
    mTexOffs0Y = MR::repeat(add + mTexOffs0Y, 0.0f, 1.0f);
    mTexOffs1X = MR::repeat(add + mTexOffs1X, 0.0f, 1.0f);
    mTexOffs1Y = MR::repeat(sub + mTexOffs1Y, 0.0f, 1.0f);
}

void OceanSphere::updatePoints() {
    mAxisPointPX->updatePos(mRadius, mWaveTime1, mWaveTime2);
    mAxisPointNX->updatePos(mRadius, mWaveTime1, mWaveTime2);
    mAxisPointPY->updatePos(mRadius, mWaveTime1, mWaveTime2);
    mAxisPointNY->updatePos(mRadius, mWaveTime1, mWaveTime2);
    mAxisPointPZ->updatePos(mRadius, mWaveTime1, mWaveTime2);
    mAxisPointNZ->updatePos(mRadius, mWaveTime1, mWaveTime2);

    mPlaneLeftUpper->update(mRadius, mWaveTime1, mWaveTime2);
    mPlaneRightUpper->update(mRadius, mWaveTime1, mWaveTime2);
    mPlaneLeftLower->update(mRadius, mWaveTime1, mWaveTime2);
    mPlaneRightLower->update(mRadius, mWaveTime1, mWaveTime2);

    mEdge0->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge1->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge2->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge3->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge4->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge5->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge6->update(mRadius, mWaveTime1, mWaveTime2);
    mEdge7->update(mRadius, mWaveTime1, mWaveTime2);
}

OceanSpherePoint* OceanSphere::getPlanePointLeftUpper(int row, int col) const {
    int max;

    if (row != 0 && col != 0) {
        max = mPointCount - 1;

        if (row != max && col != max) {
            return mPlaneLeftUpper->getPoint(row - 1, col - 1);
        }
    }

    if (row == 0) {
        if (col == 0) {
            return mAxisPointPY;
        }

        max = mPointCount - 1;
        if (col == max) {
            return mAxisPointNZ;
        }

        return mEdge1->mPoints[col - 1];
    }

    if (col == 0) {
        max = mPointCount - 1;
        if (row == max) {
            return mAxisPointPZ;
        }

        return mEdge0->mPoints[row - 1];
    }

    max = mPointCount - 1;
    if (row == max) {
        if (col == max) {
            return mAxisPointNX;
        }

        return mEdge4->mPoints[col - 1];
    }

    return mEdge6->mPoints[row - 1];
}

OceanSpherePoint* OceanSphere::getPlanePointRightUpper(int row, int col) const {
    int max;

    if (row != 0 && col != 0) {
        max = mPointCount - 1;

        if (row != max && col != max) {
            return mPlaneRightUpper->getPoint(row - 1, col - 1);
        }
    }

    if (row == 0) {
        if (col == 0) {
            return mAxisPointPY;
        }

        max = mPointCount - 1;
        if (col == max) {
            return mAxisPointPZ;
        }

        return mEdge0->mPoints[col - 1];
    }

    if (col == 0) {
        max = mPointCount - 1;
        if (row == max) {
            return mAxisPointNZ;
        }

        return mEdge1->mPoints[row - 1];
    }

    max = mPointCount - 1;
    if (row == max) {
        if (col == max) {
            return mAxisPointPX;
        }

        return mEdge7->mPoints[col - 1];
    }

    return mEdge5->mPoints[row - 1];
}

OceanSpherePoint* OceanSphere::getPlanePointLeftLower(int row, int col) const {
    int max;

    if (row != 0 && col != 0) {
        max = mPointCount - 1;

        if (row != max && col != max) {
            return mPlaneLeftLower->getPoint(row - 1, col - 1);
        }
    }

    if (row == 0) {
        if (col == 0) {
            return mAxisPointNY;
        }

        max = mPointCount - 1;
        if (col == max) {
            return mAxisPointPZ;
        }

        return mEdge2->mPoints[col - 1];
    }

    if (col == 0) {
        max = mPointCount - 1;
        if (row == max) {
            return mAxisPointNZ;
        }

        return mEdge3->mPoints[row - 1];
    }

    max = mPointCount - 1;
    if (row == max) {
        if (col == max) {
            return mAxisPointNX;
        }

        return mEdge6->mPoints[col - 1];
    }

    return mEdge4->mPoints[row - 1];
}

OceanSpherePoint* OceanSphere::getPlanePointRightLower(int row, int col) const {
    int max;

    if (row != 0 && col != 0) {
        max = mPointCount - 1;

        if (row != max && col != max) {
            return mPlaneRightLower->getPoint(row - 1, col - 1);
        }
    }

    if (row == 0) {
        if (col == 0) {
            return mAxisPointNY;
        }

        max = mPointCount - 1;
        if (col == max) {
            return mAxisPointNZ;
        }

        return mEdge3->mPoints[col - 1];
    }

    if (col == 0) {
        max = mPointCount - 1;
        if (row == max) {
            return mAxisPointPZ;
        }

        return mEdge2->mPoints[row - 1];
    }

    max = mPointCount - 1;
    if (row == max) {
        if (col == max) {
            return mAxisPointPX;
        }

        return mEdge5->mPoints[col - 1];
    }

    return mEdge7->mPoints[row - 1];
}

void OceanSphere::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    if (mIsStartPosCamera && !mIsCameraInside) {
        loadMaterialBack();

        if (mUseDisplayList) {
            GXCallDisplayList(mDispListBack, mDispListBackLen);
        } else {
            drawSphere(true, false);
        }
    }

    loadMaterialFace();

    if (mIsStartPosCamera) {
        if (mIsCameraInside) {
            GXSetCullMode(GX_CULL_FRONT);
        }

        if (mUseDisplayList) {
            GXCallDisplayList(mDispListFace, mDispListFaceLen);
        } else {
            drawSphere(false, false);
        }

        return;
    }

    GXSetCullMode(GX_CULL_FRONT);
    drawSphere(false, false);

    if (mIsCameraInside) {
        return;
    }

    GXSetCullMode(GX_CULL_BACK);
    GXColor c0 = {sOceanSphereTevReg0BackR, sOceanSphereTevReg0BackG, sOceanSphereTevReg0BackB, sOceanSphereTevReg0BackA};
    GXSetTevColor(GX_TEVREG0, c0);
    GXSetTevColor(GX_TEVREG1, mTevReg1Back);
    GXColor c2 = {sOceanSphereTevReg2BackR, sOceanSphereTevReg2BackG, sOceanSphereTevReg2BackB, sOceanSphereTevReg2BackA};
    GXSetTevColor(GX_TEVREG2, c2);
    GXColor kc0 = {sOceanSphereKColor0BackR, sOceanSphereKColor0BackG, sOceanSphereKColor0BackB, sOceanSphereKColor0BackA};
    GXSetTevKColor(GX_KCOLOR0, kc0);
    drawSphere(false, false);
}

void OceanSphere::loadMaterialFace() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    mWaterTex->load(GX_TEXMAP0);
    GXSetNumIndStages(0);
    GXSetNumTevStages(4);
    GXSetTevColor(GX_TEVREG0, (GXColor){sOceanSphereTevReg0FaceR, sOceanSphereTevReg0FaceG, sOceanSphereTevReg0FaceB, sOceanSphereTevReg0FaceA});
    GXSetTevColor(GX_TEVREG1, mTevReg1Front);
    GXSetTevColor(GX_TEVREG2, (GXColor){sOceanSphereTevReg2FaceR, sOceanSphereTevReg2FaceG, sOceanSphereTevReg2FaceB, sOceanSphereTevReg2FaceA});
    GXSetTevKColor(GX_KCOLOR0, (GXColor){sOceanSphereKColor0FaceR, sOceanSphereKColor0FaceG, sOceanSphereKColor0FaceB, sOceanSphereKColor0FaceA});
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_A0, GX_CC_C0, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_COMP_R8_EQ, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE3, GX_CC_C1, GX_CC_C2, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE3, GX_CA_A1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetCullMode(GX_CULL_BACK);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f,
        (GXColor){sOceanSphereFogFaceR, sOceanSphereFogFaceG, sOceanSphereFogFaceB, sOceanSphereFogFaceA});
}

void OceanSphere::loadMaterialBack() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_NRM, GX_DIRECT);
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_NRM, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);

    TPos3f pos;
    MtxPtr camMtx = MR::getCameraViewMtx();
    pos.set(camMtx);
    f32 sc = 0.779175f;
    pos.mMtx[0][0] *= sc;
    pos.mMtx[0][1] *= sc;
    pos.mMtx[0][2] *= sc;
    pos.mMtx[1][0] *= sc;
    pos.mMtx[1][1] *= sc;
    pos.mMtx[1][2] *= sc;
    pos.mMtx[2][0] *= sc;
    pos.mMtx[2][1] *= sc;
    pos.mMtx[2][2] *= sc;
    pos.mMtx[0][3] = 0.0f;
    pos.mMtx[1][3] = 0.0f;
    pos.mMtx[2][3] = 0.0f;
    GXLoadTexMtxImm(pos.toMtxPtr(), GX_TEXMTX0, GX_MTX2x4);

    mWaterEnvTex->load(GX_TEXMAP0);
    GXSetNumIndStages(0);
    GXSetNumTevStages(1);
    GXColor c0 = {sOceanSphereTevReg0EnvR, sOceanSphereTevReg0EnvG, sOceanSphereTevReg0EnvB, sOceanSphereTevReg0EnvA};
    GXSetTevColor(GX_TEVREG0, c0);
    GXColor c1 = {sOceanSphereTevReg1EnvR, sOceanSphereTevReg1EnvG, sOceanSphereTevReg1EnvB, sOceanSphereTevReg1EnvA};
    GXSetTevColor(GX_TEVREG1, c1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetCullMode(GX_CULL_FRONT);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f,
        (GXColor){sOceanSphereFogBackR, sOceanSphereFogBackG, sOceanSphereFogBackB, sOceanSphereFogBackA});
}

void OceanSphere::drawSphere(bool useEnvMap, bool useGD) const {
    for (s32 i = 0; i < mPointCount - 1; i++) {
        beginDrawPolygon(i, mPointCount, useGD);
        sendVertex(getPlanePointLeftUpper(i, i), useEnvMap, useGD);

        for (s32 j = i + 1; j < mPointCount; j++) {
            const OceanSpherePoint* p0 = getPlanePointLeftUpper(i, j);
            sendVertex(getPlanePointLeftUpper(i + 1, j), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        beginDrawPolygon(i, mPointCount, useGD);

        for (s32 j = 0; j < mPointCount - i - 1; j++) {
            s32 u = mPointCount - 1 - j;
            const OceanSpherePoint* p0 = getPlanePointLeftUpper(u, i + 1);
            sendVertex(getPlanePointLeftUpper(u, i), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        sendVertex(getPlanePointLeftUpper(i, i), useEnvMap, useGD);

        beginDrawPolygon(i, mPointCount, useGD);
        sendVertex(getPlanePointRightUpper(i, i), useEnvMap, useGD);

        for (s32 j = i + 1; j < mPointCount; j++) {
            const OceanSpherePoint* p0 = getPlanePointRightUpper(i, j);
            sendVertex(getPlanePointRightUpper(i + 1, j), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        beginDrawPolygon(i, mPointCount, useGD);

        for (s32 j = 0; j < mPointCount - i - 1; j++) {
            s32 u = mPointCount - 1 - j;
            const OceanSpherePoint* p0 = getPlanePointRightUpper(u, i + 1);
            sendVertex(getPlanePointRightUpper(u, i), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        sendVertex(getPlanePointRightUpper(i, i), useEnvMap, useGD);

        beginDrawPolygon(i, mPointCount, useGD);
        sendVertex(getPlanePointLeftLower(i, i), useEnvMap, useGD);

        for (s32 j = i + 1; j < mPointCount; j++) {
            const OceanSpherePoint* p0 = getPlanePointLeftLower(i, j);
            sendVertex(getPlanePointLeftLower(i + 1, j), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        beginDrawPolygon(i, mPointCount, useGD);

        for (s32 j = 0; j < mPointCount - i - 1; j++) {
            s32 u = mPointCount - 1 - j;
            const OceanSpherePoint* p0 = getPlanePointLeftLower(u, i + 1);
            sendVertex(getPlanePointLeftLower(u, i), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        sendVertex(getPlanePointLeftLower(i, i), useEnvMap, useGD);

        beginDrawPolygon(i, mPointCount, useGD);
        sendVertex(getPlanePointRightLower(i, i), useEnvMap, useGD);

        for (s32 j = i + 1; j < mPointCount; j++) {
            const OceanSpherePoint* p0 = getPlanePointRightLower(i, j);
            sendVertex(getPlanePointRightLower(i + 1, j), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        beginDrawPolygon(i, mPointCount, useGD);

        for (s32 j = 0; j < mPointCount - i - 1; j++) {
            s32 u = mPointCount - 1 - j;
            const OceanSpherePoint* p0 = getPlanePointRightLower(u, i + 1);
            sendVertex(getPlanePointRightLower(u, i), useEnvMap, useGD);
            sendVertex(p0, useEnvMap, useGD);
        }

        sendVertex(getPlanePointRightLower(i, i), useEnvMap, useGD);
    }
}

void OceanSphere::sendVertex(const OceanSpherePoint* pPoint, bool useEnvMap, bool useGD) const {
    if (useGD) {
        f32 posX = pPoint->mPos.x;
        f32 posY = pPoint->mPos.y;
        f32 posZ = pPoint->mPos.z;
        GDWrite_f32(posX);
        GDWrite_f32(posY);
        GDWrite_f32(posZ);

        if (useEnvMap) {
            f32 nrmX = pPoint->mNormal.x;
            f32 nrmY = pPoint->mNormal.y;
            f32 nrmZ = pPoint->mNormal.z;
            GDWrite_f32(nrmX);
            GDWrite_f32(nrmY);
            GDWrite_f32(nrmZ);
        } else {
            f32 texU = pPoint->mTexCoord.x;
            f32 texV = pPoint->mTexCoord.y;
            GDWrite_f32(texU + mTexOffs0X);
            GDWrite_f32(texV + mTexOffs0Y);
            GDWrite_f32(texU + mTexOffs1X);
            GDWrite_f32(texV + mTexOffs1Y);
        }
    } else {
        GXWGFifo.f32 = pPoint->mPos.x;
        GXWGFifo.f32 = pPoint->mPos.y;
        GXWGFifo.f32 = pPoint->mPos.z;

        if (useEnvMap) {
            GXWGFifo.f32 = pPoint->mNormal.x;
            GXWGFifo.f32 = pPoint->mNormal.y;
            GXWGFifo.f32 = pPoint->mNormal.z;
        } else {
            GXTexCoord2f32(pPoint->mTexCoord.x + mTexOffs0X, pPoint->mTexCoord.y + mTexOffs0Y);
            GXTexCoord2f32(pPoint->mTexCoord.x + mTexOffs1X, pPoint->mTexCoord.y + mTexOffs1Y);
        }
    }
}

namespace {
    TVec3f sPosAxisX(sAxisX);
    TVec3f sPosAxisY(sAxisY);
    TVec3f sPosAxisZ(sAxisZ);
};
