#include "Game/MapObj/ElectricRail/ElectricRail.h"
#include "Game/MapObj/ElectricRail/ElectricRailHolder.h"
#include "JMath/JMath.h"
#include "MR/actor/LiveActorUtil.h"
#include "MR/actor/ActorSensorUtil.h"
#include "MR/actor/ActorShadowUtil.h"
#include "MR/actor/ActorSwitchUtil.h"
#include "MR/Functor/Functor.h"
#include "MR/JMap/JMapUtil.h"
#include "MR/DemoUtil.h"
#include "MR/GravityUtil.h"
#include "MR/ObjUtil.h"
#include "MR/PlayerUtil.h"
#include "MR/RailUtil.h"
#include "MR/SchedulerUtil.h"
#include "MR/SoundUtil.h"
#include <revolution/gd.h>
#include <revolution/gx.h>

ElectricRailPoint::ElectricRailPoint(const char *pName) : LiveActor(pName)
{
    _8C = 1;
    _8D = 1;
}

void ElectricRailPoint::init(const JMapInfoIter &iter)
{
    initModelManagerWithAnm("ElectricRailPoint", 0, 0);
    MR::connectToSceneMapObjDecoration(this);

    if (_8C)
    {
        MR::initShadowVolumeSphere(this, 35.0);
        MR::onCalcShadowOneTime(this, 0);
        MR::onCalcShadowDropGravityOneTime(this, 0);
    }

    initHitSensor(1);
    JGeometry::TVec3<f32> pos(0.0f, 0.0f, 0.0f);
    MR::addHitSensorMapObj(this, "body", 8, 35.0f, pos);

    makeActorAppeared();
}

void ElectricRailPoint::attackSensor(HitSensor *p1, HitSensor *p2)
{
    if (_8D)
    {
        MR::sendMsgEnemyAttackElectric(p1, p2);
    }
    else
    {
        MR::sendMsgPush(p1, p2);
    }
}

const char * ElectricRail::cSensorNameTable[8] =
{
    "body0", "body1", "body2", "body3", "body4", "body5", "body6", "body7"
};

ElectricRail::ElectricRail(const char *name) : LiveActor(name)
{
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

void ElectricRail::init(const JMapInfoIter &iter)
{
    s32 railType = 0;
    MR::getJMapInfoArg3NoInit(iter, &railType);
    ElectricRailFunction::registerRail(this, (ElectricRailType)railType);
    MR::connectToSceneMapObjMovement(this);
    initMapToolInfo(iter);
    initRailRider(iter);

    f32 ret;
    MR::calcRailClippingInfo(&mTranslation, &ret, this, 100.0f, 500.0f);
    MR::setClippingTypeSphere(this, ret, mTranslation);
    
    MR::getJMapInfoArg4NoInit(iter, &_B4);

    if (_B4)
    {
        bool val = MR::calcGravityVectorOrZero(this, mTranslation, &mGravity, 0, 0);
    
        if (!val)
            MR::calcDropShadowVector(this, mTranslation, &mGravity, 0, 0);
    }

    initSensor();
    MR::setClippingFar200m(this);
    initPoints();
    initSeparators();
    initDisplayList();
    initShadow(iter);
    initSound(4, 1);
    mSoundObj->setTrans(_94);
    
    bool demoCastFlag = MR::tryRegisterDemoCast(this, iter);

    if (demoCastFlag)
    {
        s32 curPoint = 0;

        while (curPoint < mPointCount)
        {
            MR::tryRegisterDemoCast(&mPoints[curPoint], iter);
            curPoint++;
        }

        MR::registerDemoActionNerve(this, NrvElectricRail::ElectricRailNrvDisappear::sInstance, 0);
    }

    initNerve(NrvElectricRail::ElectricRailNrvWait::sInstance);

    if (!MR::isExistStageSwitchSleep(iter))
    {
        MR::useStageSwitchSleep(this, iter);
        makeActorDead();
    }
    else
        makeActorAppeared();
}

void ElectricRail::initAfterPlacement()
{
    if (MR::isValidSwitchA(this))
    {
        if (MR::isOnSwitchA(this))
        {
            MR::invalidateHitSensors(this);

            s32 curPoint = 0;

            while(curPoint < mPointCount)
            {
                mPoints[curPoint]._8D = 0;
                curPoint++;
            }

            setNerve(NrvElectricRail::ElectricRailNrvDisappeared::sInstance);
        }
    }
}

void ElectricRail::draw() const
{
    if (MR::isValidDraw(this))
    {
        if (isNerve(NrvElectricRail::ElectricRailNrvWait::sInstance))
            GXCallDisplayList(_A0, mDLLength);
        else
            drawRailGX(mEaseIn);
    }
}

void ElectricRail::makeActorAppeared()
{
    LiveActor::makeActorAppeared();

    s32 curPoint = 0;

    while (curPoint < mPointCount)
    {
        mPoints[curPoint].makeActorAppeared();
        curPoint++;
    }
}

void ElectricRail::makeActorDead()
{
    LiveActor::makeActorDead();

    s32 curPoint = 0;

    while (curPoint < mPointCount)
    {
        mPoints[curPoint].makeActorDead();
        curPoint++;
    }
}

void ElectricRail::disappear()
{
    if (isNerve(NrvElectricRail::ElectricRailNrvWait::sInstance))
        setNerve(NrvElectricRail::ElectricRailNrvDisappear::sInstance);
}

void ElectricRail::attackSensor(HitSensor *a1, HitSensor *a2)
{
    if (MR::isSensorPlayer(a1))
    {
        if (!MR::isPlayerSquat())
            MR::sendMsgEnemyAttackElectric(a1, a2);
    }
}

void ElectricRail::initMapToolInfo(const JMapInfoIter &iter)
{
    MR::initDefaultPos(this, iter);
    MR::getJMapInfoArg0NoInit(iter, &mRailHeight);

    if (MR::useStageSwitchReadA(this, iter))
    {
        MR::FunctorV0M<ElectricRail *, void (ElectricRail:: *)(void)> functor;
        functor._4 = this;
        functor._8 = 0;
        functor._C = -1;
        //functor.mFuncPtr = (void)ElectricRail::disappear;
        MR::listenStageSwitchOnA(this, functor);
    }
}

void ElectricRail::initSensor()
{
    initHitSensor(mRailHeight);

    _94 = new JGeometry::TVec3<f32>[mRailHeight];

    s32 curSensor = 0;

    while (curSensor < mRailHeight)
    {
        JGeometry::TVec3<f32> temp(0.0f, 0.0f, 0.0f);
        
        f32 hitSensorRadius = ElectricRailFunction::getHitSensorRadius();
        MR::addHitSensorPosMapObj(this, ElectricRail::cSensorNameTable[curSensor], 8, hitSensorRadius, &_94[curSensor], temp);
        curSensor++;
    }

    updateHitSensorPos();
}

void ElectricRail::initPoints()
{
    s32 railPointNum = MR::getRailPointNum(this);
    mPointCount = 0;

    s32 curRail = 0;

    while (curRail < railPointNum)
    {
        s32 curRailPointArg = -1;

        bool ret = MR::getRailPointArg0NoInit(this, curRail, &curRailPointArg);

        if (!ret)
            mPointCount++;

        curRail++;
    }

    mPointCount *= mRailHeight;
    mPoints = new ElectricRailPoint[mPointCount];

    s32 curPointIdx = 0;
    s32 curRailPoint = 0;
    s32 flag = 0;

    while (curRailPoint < MR::getRailPointNum(this))
    {
        s32 tempArg = -1;
        bool ret = MR::getRailPointArg0NoInit(this, curRailPoint, &tempArg);

        if (!ret)
        {
            ElectricRailPoint* point = &mPoints[curPointIdx];
            JGeometry::TVec3<f32> temp;
            MR::calcRailPointPos(&temp, this, curRailPoint);
            point->mTranslation.set<f32>(temp);
            curPointIdx++;

            if (mRailHeight > 1)
            {
                JGeometry::TVec3<f32> outGrav;
                calcGravity(&outGrav, temp);
                outGrav.scale(-100.0f);

                s32 curHeight = 1;

                while(curHeight < mRailHeight)
                {
                    ElectricRailPoint* curPointInRail = &mPoints[curPointIdx];
                    //JMathInlineVEC::PSVECAdd(&temp, &outGrav, &temp);
                    curPointInRail->mTranslation.set<f32>(temp);
                    curPointInRail->_8C = flag;
                    curPointIdx++;
                    curHeight++;
                }
            }
        }

        curRailPoint++;
    }

    s32 curPoint = 0;

    while (curPoint < mPointCount)
    {
        mPoints[curPoint].initWithoutIter();
        curPoint++;
    }
}

void ElectricRail::initSeparators()
{
    f32 railTotalLen = MR::getRailTotalLength(this);
    f64 separatorCountF64 = railTotalLen / 200.0f;
    s32 separatorCountS32 = (s32)separatorCountF64;
    separatorCountS32++;
    mSeparatorCount = separatorCountS32;
    mSeparators = new ElectricRailSeparator[separatorCountS32];

    // todo -- finish
}

void ElectricRail::initDisplayList()
{
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
    mDLLength = obj.start - obj.ptr;
    DCStoreRange(_A0, mDLLength);
}

void ElectricRail::drawRailGX(f32 a1) const
{
    drawPlaneGX(a1, a1, -a1, -a1);
    drawPlaneGX(-a1, a1, a1, -a1);
}

// ElectricRail::drawPlane()
// ElectricRail::drawPlaneGX()

void ElectricRail::initShadow(const JMapInfoIter &iter)
{
    f32 arg_1;
    MR::getJMapInfoArg1WithInit(iter, &arg_1);
    f32 arg_2;
    MR::getJMapInfoArg2WithInit(iter, &arg_2);

    if (arg_1 >= 0.0 && arg_2 < 0.0)
    {
        bool flag = arg_2 <= 0.0;

        f32 dropLength;

        if (flag)
        {
            MR::initShadowController(this, 2);
            dropLength = arg_2;
        }
        else
        {
            MR::initShadowController(this, 3);
            dropLength = arg_1;
        }

        MR::addShadowVolumeCylinder(this, "start", 20.0);
        MR::addShadowVolumeCylinder(this, "end", 20.0);

        JGeometry::TVec3<f32> startPos;
        MR::calcRailStartPos(&startPos, this);

        JGeometry::TVec3<f32> endPos;
        MR::calcRailEndPos(&endPos, this);

        MR::setShadowDropPosition(this, "start", startPos);
        MR::setShadowDropPosition(this, "end", endPos);

        MR::setShadowDropLength(this, "start", dropLength);
        MR::setShadowDropLength(this, "end", dropLength);

        MR::onCalcShadowDropPrivateGravity(this, "start");
        MR::onCalcShadowDropPrivateGravity(this, "end");

        if (flag)
        {
            mShadowDrawer = new ElectricRailShadowDrawer(this, mSeparators, mSeparatorCount);
        }
        else
        {
            MR::addShadowVolumeLine(this, "line", this, "start", 20.0, this, "end", 20.0);
        }  
    }
}

void ElectricRail::updateHitSensorPos()
{
    JGeometry::TVec3<f32>* playerCenterPos = MR::getPlayerCenterPos();
    MR::calcNearestRailPos(_94, this, *playerCenterPos);

    if (mRailHeight > 1)
    {
        JGeometry::TVec3<f32> gravity;
        calcGravity(&gravity, *_94);
        gravity.scale(-100.0);

        s32 curIdx = 1;

        while (curIdx < mRailHeight)
        {
            //JMathInlineVEC::PSVECAdd(&_94[curIdx - 1], &gravity, &_94[curIdx]);
            curIdx++;
        }
    }
}

void ElectricRail::calcGravity(JGeometry::TVec3<f32> *pOut, const JGeometry::TVec3<f32> &a2) const
{
    if (_B4)
    {
        pOut->set<f32>(mGravity);
    }
    else
    {
        bool ret = MR::calcGravityVectorOrZero(this, a2, pOut, 0, 0);
    
        if (!ret)
        {
            MR::calcDropShadowVector(this, a2, pOut, 0, 0);
        }
    }
}

void ElectricRail::exeDisappear()
{
    if (MR::isFirstStep(this))
    {
        MR::startSound(this, "SE_OJ_ELEC_RAIL_VANISH", -1, -1);
        MR::invalidateHitSensors(this);

        s32 curPoint = 0;

        while (curPoint < mPointCount)
        {
            ElectricRailPoint *pCurPoint = &mPoints[curPoint];
            curPoint++;
            pCurPoint->_8D = 0;
        }
    }

    mEaseIn = MR::calcNerveEaseInValue(this, 0x14, 30.0, 0.0);

    if (MR::isStep(this, 0x14))
    {
        setNerve(NrvElectricRail::ElectricRailNrvDisappeared::sInstance);
    }
}

namespace NrvElectricRail
{
    void ElectricRailNrvDisappeared::execute(Spine *pSpine) const
    {
        LiveActor *pActor = (LiveActor*)pSpine->mExecutor;
        if (MR::isFirstStep(pActor))
        {
            MR::hideModel(pActor);
        }
    }

    void ElectricRailNrvDisappear::execute(Spine *pSpine) const
    {
        ElectricRail *pRail = (ElectricRail*)pSpine->mExecutor;
        pRail->exeDisappear();
    }

    void ElectricRailNrvWait::execute(Spine *pSpine) const
    {
        ElectricRail *pRail = (ElectricRail*)pSpine->mExecutor;
        pRail->updateHitSensorPos();
        MR::startLevelSound(pRail, "SE_OJ_LV_ELEC_RAIL_HAM", -1, -1, -1);
    }
};