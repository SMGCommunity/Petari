#include "Game/MapObj/ElectricRailMoving.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include "revolution/gd/GDBase.h"
#include "revolution/gx/GXEnum.h"

ElectricRailMoving::ElectricRailMoving(const char* pName) : LiveActor(pName) {
}

void ElectricRailMoving::initPoints() {
    _D8 = 2 * mSegmentNum;

    if (!MR::isLoopRail(this)) {
        _D8 += 2;
    }

    _D8 *= mStackHeight;
    mMovingPoints = new ElectricRailMovingPoint[_D8];

    if (!MR::isLoopRail(this)) {
        u32 v4 = _D8 - 2 * mStackHeight;
        TVec3f startPos, endPos;
        MR::calcRailStartPointPos(&startPos, this);
        MR::calcRailEndPointPos(&endPos, this);

        mMovingPoints[v4++].mPosition.set< f32 >(startPos);
        u32 v6 = v4 + 1;
        mMovingPoints[v4].mPosition.set< f32 >(endPos);

        if (mStackHeight > 1) {
            TVec3f g;
            MR::calcGravityVector(this, startPos, &g, nullptr, 0);
            g *= -100.0f;

            TVec3f v16;
            v16.set< f32 >(startPos);
            u32 v8 = v6;

            for (s32 i = 1; i < mStackHeight; i++) {
                v16 += g;
                mMovingPoints[v6++].mPosition.set< f32 >(v16);
            }

            TVec3f v18;
            MR::calcGravityVector(this, v18, &g, nullptr, 0);
            v16.set< f32 >(g * -100.0f);

            for (s32 i = 1; i < mStackHeight; i++) {
                mMovingPoints[v6++].mPosition.set< f32 >(g + v16);
            }
        }
    }

    updatePointPos();
    s32 v14 = 0;
    s32 v15 = 0;

    while (v14 < _D8) {
        if (v14 % (2 * mStackHeight) >= 2) {
            mMovingPoints[v15]._8C = 0;
        }

        mMovingPoints[v15].initWithoutIter();
        v14++;
        v15++;
    }
}

void ElectricRailMoving::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts interrupt(false);
    _E0 = ElectricRailFunction::calcDisplayListSize(20, _E4 * 4 * mStackHeight);
    _DC = new (32) u8[_E0];

    GDLObj dl;
    GDInitGDLObj(&dl, _DC, _E0);
    __GDCurrentDL = &dl;

    drawPlane(30.0f, 30.0f, -30.0f, -30.0f);
    drawPlane(-30.0f, 30.0f, 30.0f, -30.0f);
    GDPadCurr32();
    _E0 = dl.ptr - dl.start;
    DCStoreRange(_DC, _E0);
}

void ElectricRailMoving::drawRailGX(f32 a1) const {
    drawPlaneGX(a1, a1, -a1, -a1);
    drawPlaneGX(-a1, a1, a1, -a1);
}

void ElectricRailMoving::drawPlane(f32 a2, f32 a3, f32 a4, f32 a5) const {
    for (s32 i = 0; i < mStackHeight; i++) {
        u32 vertexFormat = (2 * _E4) & 0xFFFE;

        GDOverflowCheck(1);
        *__GDCurrentDL->ptr++ = 0x98;

        GDOverflowCheck(2);
        *__GDCurrentDL->ptr++ = vertexFormat >> 8;
        *__GDCurrentDL->ptr++ = vertexFormat;

        for (s32 j = 0; j < _E4; j++) {
            setVertexAttribute(j, i, a2, a3, a4, a5, GDPosition3f32, GDTexCoord2f32);
        }
    }
}

void ElectricRailMoving::drawPlaneGX(f32 a1, f32 a2, f32 a3, f32 a4) const {
    for (s32 i = 0; i < mStackHeight; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, _E4 * 2);

        for (s32 j = 0; j < _E4; j++) {
            setVertexAttribute(j, i, a1, a2, a3, a4, GXPosition3f32, GXTexCoord2f32);
        }
    }
}

void ElectricRailMoving::setVertexAttribute(int a2, int a3, f32 a4, f32 a5, f32 a6, f32 a7, PosAttrFunc posAttr, TexAttrFunc texAttr) const {
    f32 railLength = MR::getRailTotalLength(this);

    f32 coord = a2;
    if (100.0 * coord >= railLength) {
        coord = coord;
    } else {
        railLength = 100.0 * coord;
    }

    TVec3f position = _E8[a2];
    TVec3f railDirection;
    TVec3f gravity;

    MR::calcRailDirectionAtCoord(&railDirection, this, railLength);
    MR::calcGravityVector(this, position, &gravity, 0, 0);

    gravity = -gravity;

    if (a3 > 0) {
        TVec3f offset = gravity * (100.0f * a3);
        position += offset;
    }

    TVec3f axis1;
    TVec3f axis2;
    TVec3f negRailDirection = -railDirection;

    if (MR::isSameDirection(gravity, negRailDirection, 0.01f)) {
        MR::makeAxisCrossPlane(&axis1, &axis2, negRailDirection);
    } else {
        MR::makeAxisFrontUp(&axis1, &axis2, negRailDirection, gravity);
    }

    posAttr(position.x + axis1.x * a4 + axis2.x * a5, position.y + axis1.y * a4 + axis2.y * a5, position.z + axis1.z * a4 + axis2.z * a5);
    texAttr(0.25f * railLength / 100.0f, 0.0f);
    posAttr(position.x + axis1.x * a6 + axis2.x * a7, position.y + axis1.y * a6 + axis2.y * a7, position.z + axis1.z * a6 + axis2.z * a7);
    texAttr(0.25f * railLength / 100.0f, 1.0f);
}

void ElectricRailMoving::updateHitSensorPos() {
    f32 nearestRailPos = MR::calcNearestRailPos(mSensorOffsets, this, *MR::getPlayerCenterPos());

    if (mStackHeight > 1) {
        TVec3f v9;
        MR::calcGravityVector(this, mSensorOffsets[0], &v9, nullptr, 0);
        v9 *= -100.0f;

        s32 v5 = 1;

        for (s32 i = 1; i < mStackHeight; i++) {
            mSensorOffsets[i - 1] += v9;
        }
    }

    bool isValid = isValidCoord(nearestRailPos);

    for (s32 i = 0; i < mStackHeight; i++) {
        if (isValid) {
            MR::getSensorWithIndex(this, i)->validate();
        } else {
            MR::getSensorWithIndex(this, i)->invalidate();
        }
    }
}

ElectricRailMoving::~ElectricRailMoving() {
}
