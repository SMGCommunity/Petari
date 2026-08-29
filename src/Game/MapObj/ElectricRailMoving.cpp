#include "Game/MapObj/ElectricRailMoving.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/Gravity/GravityInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "revolution/gd/GDBase.h"
#include "revolution/gx/GXEnum.h"

namespace NrvElectricRailMoving {
    NEW_NERVE(ElectricRailMovingNrvWait, ElectricRailMoving, Wait);
    NEW_NERVE(ElectricRailMovingNrvDisappear, ElectricRailMoving, Disappear);
    NEW_NERVE(ElectricRailMovingNrvDisappeared, ElectricRailMoving, Disappeared);
};  // namespace NrvElectricRailMoving

ElectricRailMovingPoint::ElectricRailMovingPoint(const char* pName) : LiveActor(pName) {
    _8C = 1;
    _8D = 1;
}

void ElectricRailMovingPoint::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("ElectricRailPoint", nullptr, 0);
    MR::connectToSceneMapObjDecoration(this);

    if (_8C) {
        MR::initShadowVolumeSphere(this, 35.0f);
        MR::onCalcGravity(this);
    }

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 35.0f, TVec3f(0.0f, 0.0f, 0.0f));
    makeActorAppeared();
}

void ElectricRailMovingPoint::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (_8D) {
        MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

ElectricRailMoving::ElectricRailMoving(const char* pName) : LiveActor(pName) {
}

void ElectricRailMoving::init(const JMapInfoIter& rIter) {
    s32 arg = 0;
    MR::getJMapInfoArg4NoInit(rIter, &arg);
    ElectricRailFunction::registerRail(this, (ElectricRailType)(arg + 2));
    MR::connectToSceneMapObjMovement(this);
    initMapToolInfo(rIter);
    initRail(rIter);
    initSensor();
    MR::setClippingFar200m(this);
    MR::setGroupClipping(this, rIter, 16);
    initPoints();
    initDisplayList();
    initSound(4, true);
    mSoundObject->setTrans(mSensorOffsets);
    initNerve(GET_NERVE(ElectricRailMoving, ElectricRailMovingNrvWait));
    makeActorAppeared();
}

void ElectricRailMoving::draw() const {
    if (MR::isValidDraw(this)) {
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x21u, 0, 0x7D);
        GXLoadTexMtxImm(_9C, 0x21, GX_MTX2x4);
        GXColor c = {-1, -1, -1, _CC};
        GXSetTevColor(GX_TEVREG1, c);

        if (isNerve(GET_NERVE(ElectricRailMoving, ElectricRailMovingNrvWait))) {
            GXCallDisplayList(_DC, _E0);
        } else {
            drawRailGX(_F0);
        }
    }
}

void ElectricRailMoving::disappear() {
    setNerve(GET_NERVE(ElectricRailMoving, ElectricRailMovingNrvDisappear));
}

// isTouchRail

void ElectricRailMoving::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (!MR::isPlayerSquat()) {
            MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
        }
    }
}

void ElectricRailMoving::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mSegmentNum);
    MR::getJMapInfoArg1NoInit(rIter, &mMovementSpeed);
    MR::getJMapInfoArg3NoInit(rIter, &mStackHeight);

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &ElectricRailMoving::disappear));
    }
}

//

void ElectricRailMoving::initRail(const JMapInfoIter& rIter) {
    initRailRider(rIter);
    f32 v11;
    MR::calcRailClippingInfo(&mPosition, &v11, this, 100.0f, 500.0f);
    MR::setClippingTypeSphere(this, v11, &mPosition);

    if (!MR::getJMapInfoArg2NoInit(rIter, &mSegmentLength)) {
        mSegmentLength = MR::getRailTotalLength(this) / 2 * mSegmentNum;
    }

    _E4 = (MR::getRailTotalLength(this) / 100.0f) + 2;
    _E8 = new TVec3f[_E4];

    for (s32 i = 0; i < _E4; i++) {
        f32 len = MR::getRailTotalLength(this);
        if ((100.0f * (i)) >= len) {
            len = len;
        } else {
            len = (100.0f * i);
        }

        MR::calcRailPosAtCoord(&_E8[i], this, len);
    }
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
            g.scale(-100.0f);

            TVec3f v16;
            v16.set< f32 >(startPos);
            u32 v8 = v6;

            for (s32 i = 1; i < mStackHeight; i++) {
                v16.add(g);
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

    if (MR::isSameDirection(gravity, negRailDirection)) {
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

bool ElectricRailMoving::updatePointPos() {
    f32 railTotalLength = MR::getRailTotalLength(this);
    s32 segmentNum = mSegmentNum;

    f32 segmentLength = railTotalLength / segmentNum;
    s32 ret = 0;
    f32 ec = _EC;

    for (s32 i = 0; i < mSegmentNum; ++i) {
        f32 coord = getRepeatedCoord(ec);
        TVec3f position;
        calcPointPos(&position, coord);
        f32 previousCoord = getRepeatedCoord(ec - mSegmentLength);
        TVec3f previousPosition;
        calcPointPos(&previousPosition, previousCoord);

        mMovingPoints[ret++].mPosition.set(position);
        mMovingPoints[ret++].mPosition.set(previousPosition);

        if (mStackHeight > 1) {
            TVec3f gravity;

            MR::calcGravityVector(this, position, &gravity, nullptr, 0);
            gravity *= -100.0f;
            TVec3f s = position;

            for (s32 j = 1; j < mStackHeight; j++) {
                s += gravity;
                mMovingPoints[ret++].mPosition.set(s);
            }

            MR::calcGravityVector(this, previousPosition, &gravity, nullptr, 0);
            gravity *= -100.0f;

            s = previousPosition;

            for (s32 j = 1; j < mStackHeight; j++) {
                s += gravity;
                mMovingPoints[ret++].mPosition.set(s);
            }
        }

        if (!MR::isLoopRail(this) && (getRepeatedCoord(ec + mMovementSpeed) - (mSegmentLength + (3.0f * mMovementSpeed))) < 0.0f) {
            ret = 1;
        }

        ec = getRepeatedCoord(ec + segmentLength);
    }

    return ret;
}

void ElectricRailMoving::updatePointPosAndModel() {
    bool updated = updatePointPos();

    if (!MR::isLoopRail(this)) {
        if (updated) {
            for (s32 i = 0; i < 2 * mStackHeight; i++) {
                MR::showModelIfHidden(&mMovingPoints[_D8 - 1 - i]);
            }
        } else {
            for (s32 i = 0; i < 2 * mStackHeight; i++) {
                MR::hideModelIfShown(&mMovingPoints[_D8 - 1 - i]);
            }
        }
    }
}

bool ElectricRailMoving::isValidCoord(f32 coord) const {
    f32 rpt = getRepeatedCoord(coord - _EC);
    f32 len = MR::getRailTotalLength(this);
    f32 m = MR::mod(rpt, len / mSegmentNum);
    return ((len / mSegmentNum) - mSegmentLength) < m;
}

f32 ElectricRailMoving::getRepeatedCoord(f32 coord) const {
    f32 len = MR::getRailTotalLength(this);
    return MR::repeat(coord, 0.0f, len);
}

void ElectricRailMoving::calcPointPos(TVec3f* pOutPos, f32 c) const {
    s32 v5 = (c / 100.0f);
    f32 v6 = MR::mod(c, 100.0f);
    f32 v7 = 100.0f;

    if ((_E4 - 3) < v5) {
        v7 = (MR::getRailTotalLength(this) - (100.0f * v5));
    }

    pOutPos->lerp(_E8[v5], _E8[v5 + 1], (v6 / v7));
}

void ElectricRailMoving::move() {
    _EC = getRepeatedCoord(_EC + mMovementSpeed);

    f32 scale = (100.0f * mSegmentNum) / (0.25f * MR::getRailTotalLength(this));

    _9C.identity();

    _9C.mMtx[0][0] = scale;
    _9C.mMtx[0][3] = -((0.25f * scale * _EC) / 100.0f);
    f32 railTotalLength2 = MR::getRailTotalLength(this);
    f32 segmentLength = railTotalLength2 / mSegmentNum;
    f32 angle = (-TWO_PI * mSegmentLength) / segmentLength;
    f32 sinValue;
    sinValue = JMASinRadian(angle);
    f32 value = (15.0f * sinValue) + ((255.0f * mSegmentLength) / segmentLength);
    s32 color = value;

    if (color < 0) {
        color = 0;
    } else {
        if (color > 255) {
            color = 255;
        }
    }

    _CC = color;

    updatePointPosAndModel();
}

void ElectricRailMoving::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ELEC_RAIL_VANISH");
        MR::invalidateHitSensors(this);

        for (s32 i = 0; i < _D8; i++) {
            mMovingPoints[i]._8D = 0;
        }
    }

    move();
    _F0 = MR::calcNerveEaseInValue(this, 20, 30.0f, 0.0f);

    if (MR::isStep(this, 20)) {
        setNerve(GET_NERVE(ElectricRailMoving, ElectricRailMovingNrvDisappeared));
    }
}

void ElectricRailMoving::exeDisappeared() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
    }
}

void ElectricRailMoving::exeWait() {
    move();
    updateHitSensorPos();
    MR::startLevelSound(this, "SE_OJ_LV_ELEC_RAIL_HAM");
}

ElectricRailMoving::~ElectricRailMoving() {
}

ElectricRailMovingPoint::~ElectricRailMovingPoint() {
}
