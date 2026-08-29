#include "Game/MapObj/ElectricRail.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/ShadowVolumeDrawer.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const char* cSensorNameTable[] = {"body0", "body1", "body2", "body3", "body4", "body5", "body6", "body7"};
    // const f32 cStepInterval;
    // const f32 cPointSensorRadius;
    // const f32 cRailRadius;
    // const f32 cSeparateInterval;
    // const f32 cSeparateTexRate;
    // const f32 cRailShadowWidth;
    // const f32 cRailDisappearFrame;
    // const f32 cShadowVolumeHeight;
};  // namespace

namespace NrvElectricRail {
    NEW_NERVE(ElectricRailNrvWait, ElectricRail, Wait);
    NEW_NERVE(ElectricRailNrvDisappear, ElectricRail, Disappear);
    NEW_NERVE(ElectricRailNrvDisappeared, ElectricRail, Disappeared);
};  // namespace NrvElectricRail

ElectricRailPoint::~ElectricRailPoint() {
}

ElectricRailPoint::ElectricRailPoint(const char* pName) : LiveActor(pName), mHasShadow(true), mIsActive(true) {
}

void ElectricRailPoint::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("ElectricRailPoint", nullptr, 0);
    MR::connectToSceneMapObjDecoration(this);

    if (mHasShadow) {
        MR::initShadowVolumeSphere(this, 35.0);

        MR::onCalcShadowOneTime(this, 0);
        MR::onCalcShadowDropGravityOneTime(this, 0);
    }

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 35.0f, TVec3f(0.0f, 0.0f, 0.0f));

    makeActorAppeared();
}

void ElectricRailPoint::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mIsActive) {
        MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

ElectricRailSeparator::ElectricRailSeparator() : _0(gZeroVec), mSide(gZeroVec), mUp(gZeroVec), mFront(gZeroVec), _30(gZeroVec) {
}

void ElectricRailSeparator::setup(const TVec3f& rVec1, const TVec3f& rVec2, const TVec3f& rVec3) {
    _30.set(rVec3);
    _0.set(rVec1);
    mFront.set(rVec2);

    mFront.negate();

    TVec3f upVec = -rVec3;

    if (MR::isSameDirection(upVec, mFront)) {
        MR::makeAxisCrossPlane(&mSide, &mUp, mFront);
    } else {
        MR::makeAxisFrontUp(&mSide, &mUp, mFront, upVec);
    }
}

ElectricRailShadowDrawer::ElectricRailShadowDrawer(const LiveActor* pActor, ElectricRailSeparator* pSeparator, s32 count)
    : ShadowVolumeDrawer("影描画[電撃レールボリューム]"), mHost(pActor), mDisplayListBuffer(), mDisplayListBufferSize() {
    MR::ProhibitSchedulerAndInterrupts pScheduler = MR::ProhibitSchedulerAndInterrupts(false);
    mDisplayListBufferSize = 0;

    mDisplayListBufferSize += ElectricRailFunction::calcDisplayListSize(12, count * 4 + 2);
    mDisplayListBufferSize += ElectricRailFunction::calcDisplayListSize(12, count * 2);
    mDisplayListBufferSize += ElectricRailFunction::calcDisplayListSize(12, count * 2);

    mDisplayListBuffer = new (32) u8[mDisplayListBufferSize];

    GDLObj pObj;
    GDInitGDLObj(&pObj, mDisplayListBuffer, mDisplayListBufferSize);
    __GDCurrentDL = &pObj;

    drawShadowVolumeShape(pSeparator, count);

    GDPadCurr32();

    mDisplayListBufferSize = pObj.ptr - pObj.start;

    DCStoreRange(mDisplayListBuffer, mDisplayListBufferSize);
}

void ElectricRailShadowDrawer::loadModelDrawMtx() const {
    GXLoadPosMtxImm(MR::getCameraViewMtx(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA6, 0);
}

void ElectricRailShadowDrawer::drawShape() const {
    GXCallDisplayList(mDisplayListBuffer, mDisplayListBufferSize);
}

bool ElectricRailShadowDrawer::isDraw() const {
    return MR::isValidDraw(mHost);
}

void ElectricRailShadowDrawer::drawShadowVolumeShape(ElectricRailSeparator* pSeparator, s32 val) const {
    GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, val * 4 + 2);

    TVec3f vec24, vec30, vec3C, vec48;

    for (s32 i = 0; i < val; i++) {
        ElectricRailSeparator* separator = &pSeparator[i];

        vec3C.scale(20.0f, separator->mSide);

        vec48.scale(300.0f, separator->_30);

        vec24.sub(separator->_0, vec3C);

        vec30.add(vec24, vec48);

        GDPosition3f32(vec24.x, vec24.y, vec24.z);
        GDPosition3f32(vec30.x, vec30.y, vec30.z);
    }

    for (s32 i = 0; i < val; i++) {
        ElectricRailSeparator* separator = &pSeparator[val - i - 1];

        vec3C.scale(20.0f, separator->mSide);

        vec48.scale(300.0f, separator->_30);

        vec24.add(separator->_0, vec3C);

        vec30.add(vec24, vec48);

        GDPosition3f32(vec24.x, vec24.y, vec24.z);
        GDPosition3f32(vec30.x, vec30.y, vec30.z);
    }

    vec3C.scale(20.0f, pSeparator->mSide);

    vec48.scale(300.0f, pSeparator->_30);

    vec24.sub(pSeparator->_0, vec3C);

    vec30.add(vec24, vec48);

    GDPosition3f32(vec24.x, vec24.y, vec24.z);
    GDPosition3f32(vec30.x, vec30.y, vec30.z);

    GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, val * 2);

    for (s32 i = 0; i < val; i++) {
        ElectricRailSeparator* separator = &pSeparator[i];

        vec3C.scale(20.0f, separator->mSide);

        vec24.add(separator->_0, vec3C);

        vec30.sub(separator->_0, vec3C);

        GDPosition3f32(vec24.x, vec24.y, vec24.z);
        GDPosition3f32(vec30.x, vec30.y, vec30.z);
    }

    GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, val * 2);

    for (s32 i = 0; i < val; i++) {
        ElectricRailSeparator* separator = &pSeparator[val - i - 1];

        vec3C.scale(20.0f, separator->mSide);

        vec48.scale(300.0f, separator->_30);

        vec24.add(separator->_0, vec3C);
        vec24.add(vec24, vec48);

        vec30.sub(separator->_0, vec3C);
        vec30.add(vec30, vec48);

        GDPosition3f32(vec24.x, vec24.y, vec24.z);
        GDPosition3f32(vec30.x, vec30.y, vec30.z);
    }

    GDEnd();
}

ElectricRail::ElectricRail(const char* name)
    : LiveActor(name), mPoints(), mPointCount(), _94(), mSeparators(), mDisplayListBuffer(), mDisplayListSize(), mRailHeight(1), mEaseIn(), mShadowDrawer(),
      mIsCalcGravity() {
}

void ElectricRail::init(const JMapInfoIter& iter) {
    s32 railType = 0;
    MR::getJMapInfoArg3NoInit(iter, &railType);
    ElectricRailFunction::registerRail(this, static_cast< ElectricRailType >(railType));

    MR::connectToSceneMapObjMovement(this);

    initMapToolInfo(iter);
    initRailRider(iter);

    f32 radius;
    MR::calcRailClippingInfo(&mPosition, &radius, this, 100.0f, 500.0f);
    MR::setClippingTypeSphere(this, radius, &mPosition);

    MR::getJMapInfoArg4NoInit(iter, &mIsCalcGravity);

    if (mIsCalcGravity && !MR::calcGravityVectorOrZero(this, mPosition, &mGravity, nullptr, 0)) {
        MR::calcDropShadowVector(this, mPosition, &mGravity, nullptr, 0);
    }

    initSensor();

    MR::setClippingFar200m(this);

    initPoints();
    initSeparators();
    initDisplayList();
    initShadow(iter);
    initSound(4, 1);

    mSoundObject->setTrans(_94);

    if (MR::tryRegisterDemoCast(this, iter)) {
        for (s32 i = 0; i < mPointCount; i++) {
            MR::tryRegisterDemoCast(&mPoints[i], iter);
        }

        MR::registerDemoActionNerve(this, GET_NERVE(ElectricRail, ElectricRailNrvDisappear), nullptr);
    }

    initNerve(GET_NERVE(ElectricRail, ElectricRailNrvWait));

    if (MR::isExistStageSwitchSleep(iter)) {
        MR::useStageSwitchSleep(this, iter);
        makeActorDead();
    } else
        makeActorAppeared();
}

void ElectricRail::initAfterPlacement() {
    if (!MR::isValidSwitchA(this)) {
        return;
    }

    if (!MR::isOnSwitchA(this)) {
        return;
    }

    MR::invalidateHitSensors(this);

    for (s32 i = 0; i < mPointCount; i++) {
        mPoints[i].mIsActive = false;
    }

    setNerve(GET_NERVE(ElectricRail, ElectricRailNrvDisappeared));
}

void ElectricRail::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    if (isNerve(GET_NERVE(ElectricRail, ElectricRailNrvWait))) {
        GXCallDisplayList(mDisplayListBuffer, mDisplayListSize);
    } else {
        drawRailGX(mEaseIn);
    }
}

void ElectricRail::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    for (s32 i = 0; i < mPointCount; i++) {
        mPoints[i].makeActorAppeared();
    }
}

void ElectricRail::makeActorDead() {
    LiveActor::makeActorDead();

    for (s32 i = 0; i < mPointCount; i++) {
        mPoints[i].makeActorDead();
    }
}

void ElectricRail::disappear() {
    if (isNerve(GET_NERVE(ElectricRail, ElectricRailNrvWait))) {
        setNerve(GET_NERVE(ElectricRail, ElectricRailNrvDisappear));
    }
}

void ElectricRail::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (MR::isPlayerSquat()) {
        return;
    }

    MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
}

void ElectricRail::initMapToolInfo(const JMapInfoIter& iter) {
    MR::initDefaultPos(this, iter);

    MR::getJMapInfoArg0NoInit(iter, &mRailHeight);

    if (MR::useStageSwitchReadA(this, iter)) {
        MR::listenStageSwitchOnA(this, MR::Functor_Inline(this, &ElectricRail::disappear));
    }
}

void ElectricRail::initSensor() {
    initHitSensor(mRailHeight);

    _94 = new TVec3f[mRailHeight];

    for (s32 i = 0; i < mRailHeight; i++) {
        MR::addHitSensorPosMapObj(this, ::cSensorNameTable[i], 8, ElectricRailFunction::getHitSensorRadius(), _94, TVec3f(0.0f, 0.0f, 0.0f));
    }

    updateHitSensorPos();
}

void ElectricRail::initPoints() {
    s32 railPointNum = MR::getRailPointNum(this);
    mPointCount = 0;

    s32 curRail = 0;

    while (curRail < railPointNum) {
        s32 curRailPointArg = -1;

        bool ret = MR::getRailPointArg0NoInit(this, curRail, &curRailPointArg);

        if (!ret) {
            mPointCount++;
        }
        curRail++;
    }

    mPointCount *= mRailHeight;
    mPoints = new ElectricRailPoint[mPointCount];

    s32 curPointIdx = 0;
    s32 curRailPoint = 0;
    s32 flag = 0;

    while (curRailPoint < MR::getRailPointNum(this)) {
        s32 tempArg = -1;
        bool ret = MR::getRailPointArg0NoInit(this, curRailPoint, &tempArg);

        if (!ret) {
            ElectricRailPoint* point = &mPoints[curPointIdx];
            TVec3f pos;
            MR::calcRailPointPos(&pos, this, curRailPoint);
            point->mPosition.set< f32 >(pos);
            curPointIdx++;

            if (mRailHeight > 1) {
                TVec3f outGrav;
                calcGravity(&outGrav, pos);
                outGrav.scale(-100.0f);

                s32 curHeight = 1;

                while (curHeight < mRailHeight) {
                    ElectricRailPoint* curPointInRail = &mPoints[curPointIdx];
                    pos.add(outGrav);
                    curPointInRail->mPosition.set< f32 >(pos);
                    curPointInRail->mHasShadow = flag;
                    curPointIdx++;
                    curHeight++;
                }
            }
        }

        curRailPoint++;
    }

    s32 curPoint = 0;

    while (curPoint < mPointCount) {
        mPoints[curPoint].initWithoutIter();
        curPoint++;
    }
}

void ElectricRail::initSeparators() {
    mSeparatorCount = static_cast< s32 >(MR::getRailTotalLength(this) / 200.0f) + 1;
    mSeparators = new ElectricRailSeparator[mSeparatorCount];

    TVec3f railPos, gravity;

    for (s32 i = 0; i < mSeparatorCount - 1; i++) {
        MR::setRailCoord(this, i * 200.0f);

        railPos.set(MR::getRailPos(this));

        calcGravity(&gravity, railPos);

        if (MR::isNearZero(gravity)) {
            gravity.set< f32 >(0.0f, -1.0f, 0.0f);
        }

        mSeparators[i].setup(railPos, MR::getRailDirection(this), gravity);
    }

    if (MR::isLoopRail(this)) {
        MR::moveCoordToStartPos(this);
    } else {
        MR::moveCoordToEndPos(this);
    }

    railPos.set(MR::getRailPos(this));

    calcGravity(&gravity, railPos);

    mSeparators[mSeparatorCount - 1].setup(railPos, MR::getRailDirection(this), gravity);

    MR::moveCoordToStartPos(this);
}

void ElectricRail::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibitScheduler(0);

    mDisplayListSize = ElectricRailFunction::calcDisplayListSize(20, mSeparatorCount * 4 * mRailHeight);

    mDisplayListBuffer = new (32) u8[mDisplayListSize];

    GDLObj obj;
    GDInitGDLObj(&obj, mDisplayListBuffer, mDisplayListSize);
    __GDCurrentDL = &obj;

    drawPlane(30.0, 30.0, -30.0, -30.0);
    drawPlane(-30.0, 30.0, 30.0, -30.0);
    GDPadCurr32();

    mDisplayListSize = obj.ptr - obj.start;
    DCStoreRange(mDisplayListBuffer, mDisplayListSize);
}

void ElectricRail::drawRailGX(f32 a1) const {
    drawPlaneGX(a1, a1, -a1, -a1);
    drawPlaneGX(-a1, a1, a1, -a1);
}

void ElectricRail::drawPlane(f32 a1, f32 a2, f32 a3, f32 a4) const {
    for (s32 i = 0; i < mRailHeight; i++) {
        GDBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * mSeparatorCount);

        for (s32 j = 0; j < mSeparatorCount; j++) {
            ElectricRailSeparator* s = &mSeparators[j];
            TVec3f v21(s->_0);

            f32 v15 = j / 2.0f;

            TVec3f v20(s->mUp);
            v20.scale(100.0f * i);
            v21.add(v21, v20);

            GDPosition3f32(((v21.x + (s->mSide.x * a1)) + (s->_0.x * a2)), ((v21.y + (s->mSide.y * a1)) + (s->_0.y * a2)),
                           ((v21.z + (s->mSide.z * a1)) + (s->_0.z * a2)));

            GDWrite_f32(v15);
            GDWrite_f32(0.0f);

            GDPosition3f32(((v21.x + (s->mSide.x * a3)) + (s->_0.x * a4)), ((v21.y + (s->mSide.y * a3)) + (s->_0.y * a4)),
                           ((v21.z + (s->mSide.z * a3)) + (s->_0.z * a4)));

            GDWrite_f32(v15);
            GDWrite_f32(0.0f);
        }
    }
}

void ElectricRail::drawPlaneGX(f32 a1, f32 a2, f32 a3, f32 a4) const {
    for (s32 i = 0; i < mRailHeight; i++) {
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 2 * mSeparatorCount);

        for (s32 j = 0; j < mSeparatorCount; j++) {
            ElectricRailSeparator* s = &mSeparators[j];
            TVec3f v21(s->_0);
            f32 v15 = j / 2.0f;

            TVec3f v20(s->mUp);
            v20.scale(100.0f * i);
            v21.add(v21, v20);

            GXPosition3f32(((v21.x + (s->mSide.x * a1)) + (s->_0.x * a2)), ((v21.y + (s->mSide.y * a1)) + (s->_0.y * a2)),
                           ((v21.z + (s->mSide.z * a1)) + (s->_0.z * a2)));

            GXCmd1f32(v15);
            GXCmd1f32(0.0f);

            GXPosition3f32(((v21.x + (s->mSide.x * a3)) + (s->_0.x * a4)), ((v21.y + (s->mSide.y * a3)) + (s->_0.y * a4)),
                           ((v21.z + (s->mSide.z * a3)) + (s->_0.z * a4)));

            GXCmd1f32(v15);
            GXCmd1f32(0.0f);
        }
    }
}

void ElectricRail::initShadow(const JMapInfoIter& iter) {
    f32 arg1;
    MR::getJMapInfoArg1WithInit(iter, &arg1);

    f32 arg2;
    MR::getJMapInfoArg2WithInit(iter, &arg2);

    if (!(0.0f < arg1 || 0.0f < arg2)) {
        return;
    }

    bool flag = 0.0f < arg2;

    f32 dropLength;

    if (flag) {
        MR::initShadowController(this, 2);
        dropLength = arg2;
    } else {
        MR::initShadowController(this, 3);
        dropLength = arg1;
    }

    MR::addShadowVolumeCylinder(this, "start", 20.0);
    MR::addShadowVolumeCylinder(this, "end", 20.0);

    TVec3f startPos;
    MR::calcRailStartPos(&startPos, this);

    TVec3f endPos;
    MR::calcRailEndPos(&endPos, this);

    MR::setShadowDropPosition(this, "start", startPos);
    MR::setShadowDropPosition(this, "end", endPos);

    MR::setShadowDropLength(this, "start", dropLength);
    MR::setShadowDropLength(this, "end", dropLength);

    MR::onCalcShadowDropPrivateGravity(this, "start");
    MR::onCalcShadowDropPrivateGravity(this, "end");

    if (flag) {
        mShadowDrawer = new ElectricRailShadowDrawer(this, mSeparators, mSeparatorCount);
    } else {
        MR::addShadowVolumeLine(this, "line", this, "start", 20.0, this, "end", 20.0);
    }
}

void ElectricRail::updateHitSensorPos() {
    MR::calcNearestRailPos(_94, this, *MR::getPlayerCenterPos());

    if (mRailHeight <= 1) {
        return;
    }

    TVec3f gravity;
    calcGravity(&gravity, *_94);
    gravity.scale(-100.0);

    for (s32 i = 1; i < mRailHeight; i++) {
        _94[i].add(_94[i - 1], gravity);
    }
}

void ElectricRail::calcGravity(TVec3f* pOut, const TVec3f& a2) const {
    if (mIsCalcGravity) {
        pOut->set< f32 >(mGravity);
    } else if (!MR::calcGravityVectorOrZero(this, a2, pOut, nullptr, 0)) {
        MR::calcDropShadowVector(this, a2, pOut, nullptr, 0);
    }
}

void ElectricRail::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ELEC_RAIL_VANISH");
        MR::invalidateHitSensors(this);

        for (s32 i = 0; i < mPointCount; i++) {
            mPoints[i].mIsActive = false;
        }
    }

    mEaseIn = MR::calcNerveEaseInValue(this, 20, 30.0, 0.0);

    if (MR::isStep(this, 20)) {
        setNerve(GET_NERVE(ElectricRail, ElectricRailNrvDisappeared));
    }
}

void ElectricRail::exeWait() {
    updateHitSensorPos();
    MR::startLevelSound(this, "SE_OJ_LV_ELEC_RAIL_HAM");
}

void ElectricRail::exeDisappeared() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
    }
}
