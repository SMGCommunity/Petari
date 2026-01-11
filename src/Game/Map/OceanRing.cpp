#include "Game/Map/OceanRing.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/Map/OceanRingBloomDrawer.hpp"
#include "Game/Map/OceanRingDrawer.hpp"
#include "Game/Map/OceanRingPipe.hpp"
#include "Game/Map/WaterAreaHolder.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"

namespace {
    static f32 sEdgePointNum = 2.0f;
};

OceanRing::OceanRing(const char* pName) : LiveActor(pName) {
    mWaterPointNum = 0;
    mSegCount = 0;
    mStride = 15;
    mWaterPoints = nullptr;
    mWidthMax = 0.0f;
    mObjArg1 = 30;
    mWaveTheta1 = 0.0f;
    mWaveTheta2 = 0.0f;
    mWaveHeight1 = 80.0f;
    mWaveHeight2 = 100.0f;
    _B4 = 0;
    mNearPosToPlayer = 0.0f;
    mNearestPos.x = 0.0f;
    mNearestPos.y = 0.0f;
    mNearestPos.z = 0.0f;
    mNearestDir.x = 0.0f;
    mNearestDir.y = 0.0f;
    mNearestDir.z = 1.0f;
    mRingDrawer = nullptr;
    _108.x = 0.0f;
    _108.y = 0.0f;
    _108.z = 0.0f;
    mNearestToWatchCam.x = 0.0f;
    mNearestToWatchCam.y = 0.0f;
    mNearestToWatchCam.z = 0.0f;
    mSoundObj1 = nullptr;
    mNerarestToCam.x = 0.0f;
    mNerarestToCam.y = 0.0f;
    mNerarestToCam.z = 0.0f;
    mSoundObj2 = nullptr;
    mOceanRingPipe = nullptr;
    mBloomDrawer = nullptr;
    mBox.i.zero();
    mBox.f.zero();
    mClippingBox.i.zero();
    mClippingBox.f.zero();
}

void OceanRing::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_OceanRing);
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    initPoints();
    mRingDrawer = new OceanRingDrawer(this);
    calcClippingBox();
    WaterAreaFunction::entryOceanRing(this);
    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == 0) {
        mOceanRingPipe = new OceanRingPipe(this, mWidthMax, 1200.0f);
        mOceanRingPipe->init(rIter);
    } else {
        if (arg0 == 2) {
            mWaveHeight1 = 20.0f;
            mWaveHeight2 = 30.0f;
        } else {
            mWaveHeight1 = 50.0f;
            mWaveHeight2 = 80.0f;
        }

        MR::loadTexFromArc("OceanRing.arc", "WaterPipeInside.bti");
    }

    MR::getJMapInfoArg1NoInit(rIter, &mObjArg1);
    mBloomDrawer = new OceanRingBloomDrawer(this);
    mBloomDrawer->initWithoutIter();
    mSoundObj1 = new AudSoundObject(&mNearestToWatchCam, 4, MR::getCurrentHeap());
    mSoundObj2 = new AudSoundObject(&mNerarestToCam, 4, MR::getCurrentHeap());
    MR::registerDemoSimpleCastAll(this);
    makeActorAppeared();
}

void OceanRing::initAfterPlacement() {
    for (s32 i = 0; i < mWaterPointNum; i++) {
        mWaterPoints[i]->initAfterPlacement();
    }
}

bool OceanRing::isInWater(const TVec3f& rVec) const {
    if (!mClippingBox.intersectsPoint(rVec)) {
        return false;
    }

    TVec3f nearPos;
    f32 pos = calcNearestPos(rVec, &nearPos, nullptr, nullptr);
    f32 w = (mWidthMax * calcCurrentWidthRate(pos));

    if (PSVECDistance(&nearPos, &rVec) > w) {
        return false;
    }

    TVec3f stack_14(rVec);
    stack_14.sub(nearPos);
    TVec3f gravVec;
    MR::calcGravityVector(this, rVec, &gravVec, nullptr, 0);
    return !(stack_14.dot(gravVec) < 0.0f);
}

// https://decomp.me/scratch/bZJMh
bool OceanRing::calcWaterInfo(const TVec3f& a1, const TVec3f& a2, WaterInfo* pInfo) const {
    TVec3f v23, v24;
    f32 nearestPos = calcNearestPos(a1, &v24, &v23, nullptr);
    f32 v9 = (mWidthMax * calcCurrentWidthRate(nearestPos));
    TVec3f streamVec;
    calcStreamVec(a1, nearestPos, &streamVec);
    pInfo->mStreamVec.set< f32 >(streamVec);
    pInfo->mWaveHeight = calcWaveHeight(a1, nearestPos, &pInfo->mSurfaceNormal);
    TVec3f v21(a1);
    v21.sub(v24);
    f32 v10 = MR::vecKillElement(v21, -a2, &v21);
    f32 v11 = (((PSVECMag(&v21) / v9) * 3.1415927f) * 0.5f);

    if (v11 < 0.0f) {
        v11 = -v11;
    }

    f32 v12 = JMath::sSinCosTable.cosShort(v11);
    pInfo->mCamWaterDepth = -v10;
    pInfo->_4 = v12 + v10;
    TVec3f v20(a2);
    MR::calcGravityVector(this, a1, &v20, nullptr, 0);
    pInfo->mSurfaceNormal.set< f32 >(-v20);
    TVec3f v14(a2);
    v14.scale(v10);
    TVec3f v15(a1);
    v15.add(v14);
    pInfo->mSurfacePos.set< f32 >(v15);
    TVec3f v19(a1);
    v19.sub(v24);

    if (MR::isNearZero(v19)) {
        pInfo->mEdgeDistance = v9;
        TVec3f v18(a2);
        v18.scale(v9);
        v18.add(v24);
        pInfo->mEdgePos.set< f32 >(v18);
    } else {
        MR::normalize(&v19);
        v19.scale(v9);
        v19.add(v24);
        pInfo->mEdgeDistance = PSVECDistance(&v19, &a1);
        pInfo->mEdgePos.set< f32 >(v19);
    }

    return true;
}

// https://decomp.me/scratch/7aoXp
f32 OceanRing::calcNearestPos(const TVec3f& a1, TVec3f* a2, TVec3f* a3, TVec3f* a4) const {
    WaterPoint* point = mWaterPoints[7];
    f32 origDist = PSVECDistance(&point->mOrigPos, &a1);
    u32 v12 = 0;
    u32 v13 = 1;

    for (s32 i = 15;; i += 15) {
        if (v13 >= mSegCount) {
            break;
        }

        WaterPoint* pnt = mWaterPoints[i + 7];
        f32 dist = PSVECDistance(&pnt->mOrigPos, &a1);

        if (dist < origDist) {
            origDist = dist;
            point = pnt;
            v12 = v13;
        }

        v13++;
    }

    f32 v18 = 0.0f;
    s32 v33;
    if (v12 <= 0 || v12 >= mSegCount - 1) {
        a2->set< f32 >(point->mOrigPos);
        v33 = v12;
    } else {
        WaterPoint* v20 = mWaterPoints[0xF * v12 - 8];
        WaterPoint* v21 = mWaterPoints[0xF * v12 + 0x16];
        f32 v22 = PSVECDistance(&v20->mOrigPos, &a1);
        v33 = v12;

        if (v22 < PSVECDistance(&v21->mOrigPos, &a1)) {
            v18 = (200.0f * ((v33 - 4.503601774854144e15f) + MR::calcPerpendicFootToLine(a2, a1, point->mOrigPos, v20->mOrigPos)));
        } else {
            v18 = (200.0f * ((v33 - 4.503601774854144e15f) + MR::calcPerpendicFootToLine(a2, a1, point->mOrigPos, v21->mOrigPos)));
        }
    }

    if (a4) {
        TVec3f v31, v32;
        MR::calcRailPosAndDirectionAtCoord(&v31, &v32, this, v18);
        TVec3f v30(0.0f, -1.0f, 0.0f);
        MR::calcGravityVector(this, v31, &v30, 0, 0);
        TVec3f v29;
        TVec3f v26(-v30);
        PSVECCrossProduct(&MR::getRailDirection(this), (const Vec*)&v26, (Vec*)&v29);
        f32 v24 = (mWidthMax * calcCurrentWidthRate(v18));
        TVec3f v28(v29);
        v28.scale(-v24);
        v28.add(*a2);
        TVec3f v27(v29);
        v27.scale(v24);
        v27.add(*a2);
        MR::calcPerpendicFootToLineInside(a4, a1, v28, v27);
    }

    if (a3) {
        MR::calcRailDirectionAtCoord(a3, this, v18);
    }

    return v18;
}

f32 OceanRing::calcWaveHeight(const TVec3f& a1, f32 a2, TVec3f* a3) const {
    MR::setRailCoord((OceanRing*)this, a2);
    TVec3f v43(a1);
    v43.sub(MR::getRailPos(this));
    TVec3f v42(0.0f, -1.0f, 0.0f);
    MR::calcGravityVector(this, a1, &v42, nullptr, 0);
    TVec3f v30(-v42);
    TVec3f v37;
    PSVECCrossProduct((const Vec*)&MR::getRailDirection(this), &v30, &v37);
    f32 dot = v43.dot(v37);
    u32 v11 = mStride - 1;
    f32 v12 = (1200.0f + dot);
    s32 v44 = (7.0f + (dot / 171.42857f));
    s32 v13 = MR::clamp(v44, 0, v11);
    u32 v14 = mSegCount - 1;
    s32 v46 = (a2 / 200.0f);
    s32 v15 = MR::clamp(v46, 0, v14);
    WaterPoint* point = getPoint(v13, v15);
    f32 v17 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, v12, a2);
    TVec3f v36(v37);
    v36.scale(-10.0f);
    TVec3f v35(v37);
    v35.scale(10.0f);
    TVec3f v34(MR::getRailDirection(this));
    v34.scale(10.0f);
    TVec3f v33(MR::getRailDirection(this));
    v33.scale(-10.0f);
    point = getPoint(v13, v15);
    f32 v21 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, (v12 - 10.0f), a2);
    v36.y += v21;
    point = getPoint(v13, v15);
    f32 v23 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, (10.0f + v12), a2);
    v35.y += v23;
    point = getPoint(v13, v15);
    f32 v25 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, v12, (10.0f + a2));
    v34.y += v25;
    point = getPoint(v13, v15);
    f32 v27 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, v12, a2 - 10.0f);
    v33.y += v27;
    TVec3f v32(v35);
    v32.sub(v36);
    TVec3f v31(v33);
    v31.sub(v34);
    PSVECCrossProduct(&v31, &v32, a3);

    if (MR::isNearZero(*a3)) {
        a3->set< f32 >(-v42);
    } else {
        MR::normalize(a3);
    }

    return v17;
}

void OceanRing::calcStreamVec(const TVec3f& a1, f32 a2, TVec3f* pStreamVec) const {
    pStreamVec->zero();
    f32 v8 = (mWidthMax * calcCurrentWidthRate(a2));
    f32 v9 = (10.0f * calcCurrentFlowSpeedRate(a2));
    TVec3f near(mNearestPos);
    near.sub(a1);
    if (PSVECMag(&near) < v8) {
        pStreamVec->set< f32 >(mNearestDir);
        pStreamVec->scale(v9);
    }
}

void OceanRing::movement() {
    mWaveTheta1 += -0.04f;
    mWaveTheta2 += -0.06f;
    updatePoints();
    mRingDrawer->update();
    TVec3f nearPos;
    calcNearestPos(*MR::getCameraWatchPos(), &nearPos, nullptr, &mNearestToWatchCam);
    MR::startSoundObjectLevel(mSoundObj1, "SE_AT_LV_OCEAN_RING", -1);
    mSoundObj1->process();
    calcNearestPos(MR::getCamPos(), &nearPos, nullptr, &mNerarestToCam);
    MR::startSoundObjectLevel(mSoundObj2, "SE_AT_LV_OCEAN_RING_SUB", -1);
    mSoundObj2->process();
}

void OceanRing::startClipped() {
    LiveActor::startClipped();

    if (mOceanRingPipe != nullptr) {
        mOceanRingPipe->startClipped();
    }
}

void OceanRing::endClipped() {
    LiveActor::endClipped();

    if (mOceanRingPipe != nullptr) {
        mOceanRingPipe->endClipped();
    }
}

void OceanRing::initPoints() {
    s32 len = MR::getRailTotalLength(this) / 200.0f;
    mSegCount = len + 1;
    mWaterPointNum = 15 * len;
    f32 totalLength = MR::getRailTotalLength(this);
    mWidthMax = 1200.0f;
    f32 v6 = (totalLength / mSegCount);
    mWaterPoints = new WaterPoint*[mWaterPointNum];
    s32 v7 = 0;

    for (s32 i = 0; i < mSegCount; i++) {
        TVec3f v23(0.0f, 1.0f, 0.0f);
        MR::calcGravityVector(this, MR::getRailPos(this), &v23, nullptr, 0);
        v23.scale(-1.0f);
        TVec3f v22;
        PSVECCrossProduct(&MR::getRailDirection(this), &v23, &v22);
        MR::normalize(&v22);

        f32 railCoord = MR::getRailCoord(this);
        f32 currentWidthRate = calcCurrentWidthRate(railCoord);
        railCoord = MR::getRailCoord(this);
        f32 currentFlowSpeed = calcCurrentFlowSpeedRate(railCoord);
        s32 v15 = v7;
        s32 j = -7;

        for (; j < 7; j++) {
            TVec3f v21(v22);
            v21.scale((currentWidthRate * (171.42857f * j)));
            v21.add(MR::getRailPos(this));
            f32 ease = 1.0f;
            s32 v19 = __abs(j);
            if ((7 - v19) < sEdgePointNum) {
                ease = MR::getEaseOutValue((7 - v19) / sEdgePointNum, 0.0f, 1.0f, 1.0f);
            }

            mWaterPoints[v15] = new WaterPoint(v21, v23, ((j + 7) * 171.42857f), (i * v6), ease, currentFlowSpeed);
            v7++;
            v15++;
        }

        MR::moveCoord(this, v6);
    }
}

void OceanRing::updatePoints() {
    if (!mBox.intersectsPoint(*MR::getPlayerPos())) {
        _B4 = 1;
        return;
    }

    mNearPosToPlayer = calcNearestPos(*MR::getPlayerPos(), &mNearestPos, &mNearestDir, nullptr);
    if (PSVECDistance(&mNearestPos, MR::getPlayerPos()) > 5000.0f) {
        _B4 = 1;
        return;
    }

    _B4 = 0;
    if (mSegCount < mObjArg1 + 8) {
        updatePointsInLine(0, mSegCount - 1, 0, 0);
        return;
    }

    bool flag = true;
    if (MR::getCamZdir().dot(mNearestDir) < 0.0f) {
        flag = false;
    }

    s32 v16 = (mNearPosToPlayer / 200.0f);
    s32 v8, v9;
    if (flag) {
        v8 = v16 - 8;
        v9 = v16 + mObjArg1;
    } else {
        v9 = v16 + 8;
        v8 = v16 - mObjArg1;
    }

    if (!MR::isLoopRail(this)) {
        s32 v10 = MR::clamp(v8, 0, mSegCount - 1);
        s32 v11 = MR::clamp(v9, 0, mSegCount - 1);
        updatePointsInLine(v10, v11, 3, 3);
        return;
    }

    if (v8 >= 0 && v9 < mSegCount) {
        updatePointsInLine(v8, v9, 3, 3);
        return;
    }

    if (v8 >= 0) {
        s32 v13 = mSegCount - 1;
        if (v9 > v13) {
            s32 v12 = mSegCount - 1;
            updatePointsInLine(0, v9, 0, 3);
            updatePointsInLine((v12 + v8) % v12, mSegCount - 1, 3, 0);
        } else {
            updatePointsInLine(v8, v13, 3, 0);
            updatePointsInLine(0, (v13 + v9) % v13, 0, 3);
        }
    }
}

void OceanRing::updatePointsInLine(s32 a1, s32 a2, s32 a3, s32 a4) {
    s32 v11;
    s32 v12;
    s32 v10;
    s32 i;

    v10 = a1;
    v11 = a1 + a3;
    v12 = a2 - a4;

    while (v10 < a2) {
        f32 v13 = 1.0f;
        if (v10 < v11) {
            v13 = (f32)(v10 - a1) / (f32)a3;
        }

        if (v10 >= v12) {
            v13 = ((f32)(a2 - 1 - v10) / (f32)a4);
        }

        for (i = 0; i < mStride; i++) {
            getPoint(i, v10)->updatePos(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, v13);
        }

        v10++;
    }
}

f32 OceanRing::calcCurrentWidthRate(f32 a1) const {
    f32 v8 = 0.0f;
    f32 v7 = 0.0f;
    MR::setRailCoord((OceanRing*)this, a1);
    MR::calcDistanceToCurrentAndNextRailPoint(this, &v8, &v7);
    f32 rate = 1.0f;
    f32 v4 = (v8 + v7);

    if (v4 < 1.0f) {
        return rate;
    }

    f32 v6 = 12.0f;
    f32 v5 = 12.0f;
    MR::getCurrentRailPointArg1NoInit(this, &v6);
    MR::getNextRailPointArg1NoInit(this, &v5);
    return ((((v6 * v7) + (v5 * v8)) / v4) / 12.0f);
}

f32 OceanRing::calcCurrentFlowSpeedRate(f32 a1) const {
    f32 v9 = 0.0f;
    f32 v8 = 0.0f;
    MR::setRailCoord((OceanRing*)this, a1);
    MR::calcDistanceToCurrentAndNextRailPoint(this, &v9, &v8);
    f32 rate = 1.0f;
    f32 v4 = (v9 + v8);

    if (v4 < 1.0f) {
        return rate;
    }

    f32 v7 = 100.0f;
    f32 v6 = 100.0f;
    MR::getCurrentRailPointArg0NoInit(this, &v7);
    MR::getNextRailPointArg0NoInit(this, &v6);
    return ((((v7 * v8) + (v6 * v9)) / v4) / 100.0f);
}

void OceanRing::calcClippingBox() {
    f32 v1 = 1200.0f;
    for (s32 i = 0; i < MR::getRailPointNum(this); i++) {
        f32 v6 = 0.0f;
        MR::getRailPointArg1NoInit(this, i, &v6);
        f32 v4 = (100.0f * v6);

        if (v4 >= v1) {
            v4 = v4;
        } else {
            v4 = v1;
        }

        v1 = v4;
    }

    MR::calcBoundingBox(this, &mClippingBox, 200.0f);
    TVec3f v9(v1);
    mClippingBox.i.sub(v9);
    mClippingBox.f.add(v9);
    mBox.i.set< f32 >(mClippingBox.i);
    mBox.f.set< f32 >(mClippingBox.f);
    TVec3f v8(5000.0f);
    mBox.i.sub(v8);
    mBox.f.add(v8);
    TVec3f v7(mWidthMax);
    mClippingBox.i.sub(v7);
    mClippingBox.f.add(v7);
    JMAVECLerp(&mClippingBox.f, &mClippingBox.i, &_108, 0.5f);
    f32 dist = PSVECDistance(&_108, &mClippingBox.i);
    MR::setClippingTypeSphere(this, (100.0f + dist), &_108);
    MR::setClippingFarMax(this);
}

void OceanRing::draw() const {
    if (MR::isValidDraw(this)) {
        mRingDrawer->draw();
    }
}

OceanRing::~OceanRing() {}

WaterPoint* OceanRing::getPoint(int a1, int a2) const {
    return mWaterPoints[a1 + a2 * mStride];
}
