#include "Game/MapObj/ElectricRail.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace NrvElectricRail {
    NEW_NERVE(ElectricRailNrvWait, ElectricRail, Wait);
    NEW_NERVE(ElectricRailNrvDisappear, ElectricRail, Disappear);
    NEW_NERVE(ElectricRailNrvDisappeared, ElectricRail, Disappeared);
};  // namespace NrvElectricRail

ElectricRailPoint::ElectricRailPoint(const char* pName) : LiveActor(pName) {
    _8C = 1;
    _8D = 1;
}

ElectricRailPoint::~ElectricRailPoint() {
}

void ElectricRailPoint::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("ElectricRailPoint", 0, 0);
    MR::connectToSceneMapObjDecoration(this);

    if (_8C) {
        MR::initShadowVolumeSphere(this, 35.0);
        MR::onCalcShadowOneTime(this, 0);
        MR::onCalcShadowDropGravityOneTime(this, 0);
    }

    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 35.0f, TVec3f(0.0f, 0.0f, 0.0f));

    makeActorAppeared();
}

void ElectricRailPoint::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (_8D) {
        MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
    } else {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

const char* ElectricRail::cSensorNameTable[8] = {"body0", "body1", "body2", "body3", "body4", "body5", "body6", "body7"};

ElectricRail::ElectricRail(const char* name) : LiveActor(name) {
    mPoints = 0;
    mPointCount = 0;
    _94 = 0;
    mSeparators = 0;
    mSeparatorCount = 0;
    _A0 = 0;
    mDLLength = 0;
    mRailHeight = 1;
    mEaseIn = 30.0f;
    mShadowDrawer = 0;
    _B4 = 0;
}

void ElectricRail::init(const JMapInfoIter& iter) {
    s32 railType = 0;
    MR::getJMapInfoArg3NoInit(iter, &railType);
    ElectricRailFunction::registerRail(this, (ElectricRailType)railType);
    MR::connectToSceneMapObjMovement(this);
    initMapToolInfo(iter);
    initRailRider(iter);

    f32 ret;
    MR::calcRailClippingInfo(&mPosition, &ret, this, 100.0f, 500.0f);
    MR::setClippingTypeSphere(this, ret, &mPosition);

    MR::getJMapInfoArg4NoInit(iter, &_B4);

    if (_B4) {
        bool val = MR::calcGravityVectorOrZero(this, mPosition, &mGravity, 0, 0);

        if (!val)
            MR::calcDropShadowVector(this, mPosition, &mGravity, 0, 0);
    }

    initSensor();
    MR::setClippingFar200m(this);
    initPoints();
    initSeparators();
    initDisplayList();
    initShadow(iter);
    initSound(4, 1);
    mSoundObject->setTrans(_94);

    bool demoCastFlag = MR::tryRegisterDemoCast(this, iter);

    if (demoCastFlag) {
        s32 curPoint = 0;

        while (curPoint < mPointCount) {
            MR::tryRegisterDemoCast(&mPoints[curPoint], iter);
            curPoint++;
        }

        MR::registerDemoActionNerve(this, &NrvElectricRail::ElectricRailNrvDisappear::sInstance, 0);
    }

    initNerve(&NrvElectricRail::ElectricRailNrvWait::sInstance);

    if (MR::isExistStageSwitchSleep(iter)) {
        MR::useStageSwitchSleep(this, iter);
        makeActorDead();
    } else
        makeActorAppeared();
}

void ElectricRail::initAfterPlacement() {
    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            MR::invalidateHitSensors(this);

            s32 curPoint = 0;

            while (curPoint < mPointCount) {
                mPoints[curPoint]._8D = 0;
                curPoint++;
            }

            setNerve(&NrvElectricRail::ElectricRailNrvDisappeared::sInstance);
        }
    }
}

void ElectricRail::draw() const {
    if (MR::isValidDraw(this)) {
        if (isNerve(&NrvElectricRail::ElectricRailNrvWait::sInstance))
            GXCallDisplayList(_A0, mDLLength);
        else
            drawRailGX(mEaseIn);
    }
}

void ElectricRail::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    s32 curPoint = 0;

    while (curPoint < mPointCount) {
        mPoints[curPoint].makeActorAppeared();
        curPoint++;
    }
}

void ElectricRail::makeActorDead() {
    LiveActor::makeActorDead();

    s32 curPoint = 0;

    while (curPoint < mPointCount) {
        mPoints[curPoint].makeActorDead();
        curPoint++;
    }
}

void ElectricRail::disappear() {
    if (isNerve(&NrvElectricRail::ElectricRailNrvWait::sInstance))
        setNerve(&NrvElectricRail::ElectricRailNrvDisappear::sInstance);
}

void ElectricRail::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (!MR::isPlayerSquat())
            MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
    }
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

    s32 curSensor = 0;

    while (curSensor < mRailHeight) {
        TVec3f temp(0.0f, 0.0f, 0.0f);

        f32 hitSensorRadius = ElectricRailFunction::getHitSensorRadius();
        MR::addHitSensorPosMapObj(this, ElectricRail::cSensorNameTable[curSensor], 8, hitSensorRadius, &_94[curSensor], temp);
        curSensor++;
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
                    curPointInRail->_8C = flag;
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
    f32 railTotalLen = MR::getRailTotalLength(this);
    f64 separatorCountF64 = railTotalLen / 200.0f;
    s32 separatorCountS32 = (s32)separatorCountF64;
    separatorCountS32++;
    mSeparatorCount = separatorCountS32;
    mSeparators = new ElectricRailSeparator[separatorCountS32];

    // todo -- finish
}

void ElectricRail::initDisplayList() {
    MR::ProhibitSchedulerAndInterrupts prohibitScheduler(0);

    mDLLength = ElectricRailFunction::calcDisplayListSize(0x14, ((mSeparatorCount << 2) * mRailHeight));

    void* shit = new u8[0x20];
    _A0 = shit;

    GDLObj obj;
    GDInitGDLObj(&obj, shit, mDLLength);
    __GDCurrentDL = &obj;

    drawPlane(30.0, 30.0, -30.0, -30.0);
    drawPlane(-30.0, 30.0, 30.0, -30.0);
    GDPadCurr32();
    // mDLLength = obj.mStart - obj.mPtr;
    DCStoreRange(_A0, mDLLength);
}

void ElectricRail::drawRailGX(f32 a1) const {
    drawPlaneGX(a1, a1, -a1, -a1);
    drawPlaneGX(-a1, a1, a1, -a1);
}

// ElectricRail::drawPlane()
// ElectricRail::drawPlaneGX()

void ElectricRail::initShadow(const JMapInfoIter& iter) {
    f32 arg_1;
    MR::getJMapInfoArg1WithInit(iter, &arg_1);
    f32 arg_2;
    MR::getJMapInfoArg2WithInit(iter, &arg_2);

    if (arg_1 >= 0.0 && arg_2 < 0.0) {
        bool flag = arg_2 <= 0.0;

        f32 dropLength;

        if (flag) {
            MR::initShadowController(this, 2);
            dropLength = arg_2;
        } else {
            MR::initShadowController(this, 3);
            dropLength = arg_1;
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
}

void ElectricRail::updateHitSensorPos() {
    MR::calcNearestRailPos(_94, this, *MR::getPlayerCenterPos());

    if (mRailHeight > 1) {
        TVec3f gravity;
        calcGravity(&gravity, *_94);
        gravity.scale(-100.0);

        s32 curIdx = 1;

        while (curIdx < mRailHeight) {
            _94[curIdx].add(_94[curIdx - 1], gravity);
            curIdx++;
        }
    }
}

void ElectricRail::calcGravity(TVec3f* pOut, const TVec3f& a2) const {
    if (_B4) {
        pOut->set< f32 >(mGravity);
    } else {
        bool ret = MR::calcGravityVectorOrZero(this, a2, pOut, 0, 0);

        if (!ret) {
            MR::calcDropShadowVector(this, a2, pOut, 0, 0);
        }
    }
}

void ElectricRail::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_ELEC_RAIL_VANISH");
        MR::invalidateHitSensors(this);

        s32 curPoint = 0;

        while (curPoint < mPointCount) {
            ElectricRailPoint* pCurPoint = &mPoints[curPoint];
            curPoint++;
            pCurPoint->_8D = 0;
        }
    }

    mEaseIn = MR::calcNerveEaseInValue(this, 20, 30.0, 0.0);

    if (MR::isStep(this, 20)) {
        setNerve(&NrvElectricRail::ElectricRailNrvDisappeared::sInstance);
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
