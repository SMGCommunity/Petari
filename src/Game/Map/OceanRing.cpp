#include "Game/Map/OceanRing.hpp"
#include "Game/Map/OceanRingDrawer.hpp"
#include "Game/Map/OceanRingPipe.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"

void OceanRing::initAfterPlacement() {
    for (s32 i = 0; i < mWaterPointNum; i++) {
        mWaterPoints[i]->initAfterPlacement();
    }
}

bool OceanRing::isInWater(const TVec3f &rVec) const {
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
bool OceanRing::calcWaterInfo(const TVec3f &a1, const TVec3f &a2, WaterInfo *pInfo) const {
    TVec3f v23, v24;
    f32 nearestPos = calcNearestPos(a1, &v24, &v23, nullptr);
    f32 v9 = (mWidthMax * calcCurrentWidthRate(nearestPos));
    TVec3f streamVec;
    calcStreamVec(a1, nearestPos, &streamVec);
    pInfo->mStreamVec.set<f32>(streamVec);
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
    pInfo->mSurfaceNormal.set<f32>(-v20);
    TVec3f v14(a2);
    v14.scale(v10);
    TVec3f v15(a1);
    v15.add(v14);
    pInfo->mSurfacePos.set<f32>(v15);
    TVec3f v19(a1);
    v19.sub(v24);

    if (MR::isNearZero(v19, 0.001f)) {
        pInfo->mEdgeDistance = v9;
        TVec3f v18(a2);
        v18.scale(v9);
        v18.add(v24);
        pInfo->mEdgePos.set<f32>(v18);
    }
    else {
        MR::normalize(&v19);
        v19.scale(v9);
        v19.add(v24);
        pInfo->mEdgeDistance = PSVECDistance(&v19, &a1);
        pInfo->mEdgePos.set<f32>(v19);
    }

    return true;
}

// https://decomp.me/scratch/7aoXp
f32 OceanRing::calcNearestPos(const TVec3f &a1, TVec3f *a2, TVec3f *a3, TVec3f *a4) const {
    WaterPoint* point = mWaterPoints[7];
    f32 origDist = PSVECDistance(&point->mOrigPos, &a1);
    u32 v12 = 0;
    u32 v13 = 1;

    for (s32 i = 15; ; i += 15) {
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
        a2->set<f32>(point->mOrigPos);
        v33 = v12;
    }
    else {
        WaterPoint* v20 = mWaterPoints[0xF * v12 - 8];
        WaterPoint* v21 = mWaterPoints[0xF * v12 + 0x16];
        f32 v22 = PSVECDistance(&v20->mOrigPos, &a1);
        v33 = v12;

        if (v22 < PSVECDistance(&v21->mOrigPos, &a1)) {
            v18 = (200.0f * ((v33 - 4.503601774854144e15f) + MR::calcPerpendicFootToLine(a2, a1, point->mOrigPos, v20->mOrigPos)));
        }
        else {
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

/*
f32 OceanRing::calcWaveHeight(const TVec3f &a1, f32 a2, TVec3f *a3) const {
    MR::setRailCoord((OceanRing*)this, a2);
    TVec3f v43(a1);
    v43.sub(MR::getRailPos(this));
    TVec3f v42(0.0f, -1.0f, 0.0f);
    MR::calcGravityVector(this, a1, &v42, nullptr, 0);
    TVec3f v30(-v42);
    TVec3f v41;
    PSVECCrossProduct((const Vec*)&MR::getRailDirection(this), &v30, &v41);
    f32 dot = v43.dot(v41);
    u32 v11 = mStride - 1;
    f32 v12 = (1200.0f + dot);
    f32 v44 = (7.0f + (dot / 171.42857f));
    s32 v13 = MR::clamp(v44, 0, v11);
    u32 v14 = mSegCount - 1;
    f32 v46 = (a2 / 200.0f);
    s32 v15 = MR::clamp(v46, 0, v14);
    WaterPoint* point = getPoint(v13, v15);
    f32 v17 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, v12, a2);
    TVec3f v36(v41);
    v36.scale(-10.0f);
    TVec3f v37(v41);
    v37.scale(10.0f);
    TVec3f v35(MR::getRailDirection(this));
    v35.scale(10.0f);
    TVec3f v33(MR::getRailDirection(this));
    v33.scale(-10.0f);
    point = getPoint(v13, v15);
    f32 v21 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, (v12 - 10.0f), a2);
    v36.y += v21;
    point = getPoint(v13, v15);
    f32 v23 = point->calcHeight(mWaveTheta1, mWaveTheta2, mWaveHeight1, mWaveHeight2, (10.0f + v12), a2);
    v35.y += v23;
}
*/

void OceanRing::calcStreamVec(const TVec3f &a1, f32 a2, TVec3f *pStreamVec) const {
    pStreamVec->zero();
    f32 v8 = (mWidthMax * calcCurrentWidthRate(a2));
    f32 v9 = (10.0f * calcCurrentFlowSpeedRate(a2));
    TVec3f near(mNearestPos);
    near.sub(a1);
    if (PSVECMag(&near) < v8) {
        pStreamVec->set<f32>(mNearestDir);
        pStreamVec->scale(v9);
    }
}
/*
void OceanRing::movement() {
    mWaveTheta1 += -0.039999999f;
    mWaveTheta2 += -0.059999999f;
    updatePoints();
    mRingDrawer->update();
    f32 nearPos = calcNearestPos(MR::getCameraWatch)
}
*/

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

// OceanRing::initPoints
// OceanRing::updatePoints
// OceanRing::updatePointsInLine

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

// OceanRing::calcClippingBox

void OceanRing::draw() const {
    if (MR::isValidDraw(this)) {
        mRingDrawer->draw();
    }
}

OceanRing::~OceanRing() {
    
}

WaterPoint* OceanRing::getPoint(int a1, int a2) const {
    return mWaterPoints[a1 + a2 * mStride];
}
