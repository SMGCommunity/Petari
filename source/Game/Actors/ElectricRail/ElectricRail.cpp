#include "Game/Actors/ElectricRail/ElectricRail.h"
#include "Game/MapObj/ElectricRailHolder.h"
#include "JMath/JMath.h"
#include "MR/actor/LiveActorUtil.h"
#include "MR/actor/ActorSensorUtil.h"
#include "MR/actor/ActorSwitchUtil.h"
#include "MR/Functor/Functor.h"
#include "MR/JMap/JMapUtil.h"
#include "MR/DemoUtil.h"
#include "MR/GravityUtil.h"
#include "MR/ObjUtil.h"
#include "MR/PlayerUtil.h"
#include "MR/RailUtil.h"
#include "MR/SchedulerUtil.h"
#include "OS/OSCache.h"
#include "OS/GD/GDBase.h"
#include "OS/GX/GXDisplayList.h"
#include "defines.h"

const char * ElectricRail::cSensorNameTable[8] =
{
    "body0", "body1", "body2", "body3", "body4", "body5", "body6", "body7"
};

ElectricRail::ElectricRail(const char *name) : LiveActor(name)
{
    this->mPoints = 0;
    this->mPointCount = 0;
    this->_94 = 0;
    this->mSeparators = 0;
    this->mSeparatorCount = 0;
    this->_A0 = 0;
    this->mDLLength = 0;
    this->mRailHeight = 1;
    this->mEaseIn = 30.0f;
    this->mShadowDrawer = 0;
    this->_B4 = 0;
}

void ElectricRail::init(const JMapInfoIter &iter)
{
    s32 railType = 0;
    MR::getJMapInfoArg3NoInit(iter, &railType);
    ElectricRailFunction::registerRail(this, (ElectricRailType)railType);
    MR::connectToSceneMapObjMovement(this);
    this->initMapToolInfo(iter);
    this->initRailRider(iter);

    f32 ret;
    MR::calcRailClippingInfo(&this->mTranslation, &ret, this, 100.0f, 500.0f);
    MR::setClippingTypeSphere(this, ret, this->mTranslation);
    
    MR::getJMapInfoArg4NoInit(iter, &this->_B4);

    if (this->_B4)
    {
        bool val = MR::calcGravityVectorOrZero(this, this->mTranslation, &this->mGravity, 0, 0);
    
        if (!val)
            MR::calcDropShadowVector(this, this->mTranslation, &this->mGravity, 0, 0);
    }

    this->initSensor();
    MR::setClippingFar200m(this);
    this->initPoints();
    this->initSeparators();
    this->initDisplayList();
    this->initShadow(iter);
    this->initSound(4, 1);
    this->mSoundObj->setTrans(this->_94);
    
    bool demoCastFlag = MR::tryRegisterDemoCast(this, iter);

    if (demoCastFlag)
    {
        s32 curPoint = 0;

        while (curPoint < this->mPointCount)
        {
            MR::tryRegisterDemoCast(&this->mPoints[curPoint], iter);
            curPoint++;
        }

        MR::registerDemoActionNerve(this, NrvElectricRail::ElectricRailNrvDisappear::sInstance, 0);
    }

    this->initNerve(NrvElectricRail::ElectricRailNrvWait::sInstance);

    if (!MR::isExistStageSwitchSleep(iter))
    {
        MR::useStageSwitchSleep(this, iter);
        this->makeActorDead();
    }
    else
        this->makeActorAppeared();
}

void ElectricRail::initAfterPlacement()
{
    if (MR::isValidSwitchA(this))
    {
        if (MR::isOnSwitchA(this))
        {
            MR::invalidateHitSensors(this);

            s32 curPoint = 0;

            while(curPoint < this->mPointCount)
            {
                this->mPoints[curPoint]._8D = 0;
                curPoint++;
            }

            this->setNerve(NrvElectricRail::ElectricRailNrvDisappeared::sInstance);
        }
    }
}

void ElectricRail::draw() const
{
    if (MR::isValidDraw(this))
    {
        if (this->isNerve(NrvElectricRail::ElectricRailNrvWait::sInstance))
            GXCallDisplayList(this->_A0, this->mDLLength);
        else
            this->drawRailGX(this->mEaseIn);
    }
}

void ElectricRail::makeActorAppeared()
{
    LiveActor::makeActorAppeared();

    s32 curPoint = 0;

    while (curPoint < this->mPointCount)
    {
        this->mPoints[curPoint].makeActorAppeared();
        curPoint++;
    }
}

void ElectricRail::makeActorDead()
{
    LiveActor::makeActorDead();

    s32 curPoint = 0;

    while (curPoint < this->mPointCount)
    {
        this->mPoints[curPoint].makeActorDead();
        curPoint++;
    }
}

void ElectricRail::disappear()
{
    if (this->isNerve(NrvElectricRail::ElectricRailNrvWait::sInstance))
        this->setNerve(NrvElectricRail::ElectricRailNrvDisappear::sInstance);
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
    MR::getJMapInfoArg0NoInit(iter, &this->mRailHeight);

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
    this->initHitSensor(this->mRailHeight);

    this->_94 = new JGeometry::TVec3<f32>[this->mRailHeight];

    s32 curSensor = 0;

    while (curSensor < this->mRailHeight)
    {
        JGeometry::TVec3<f32> temp(0.0f, 0.0f, 0.0f);
        
        f32 hitSensorRadius = ElectricRailFunction::getHitSensorRadius();
        MR::addHitSensorPosMapObj(this, ElectricRail::cSensorNameTable[curSensor], 8, hitSensorRadius, &this->_94[curSensor], temp);
        curSensor++;
    }

    this->updateHitSensorPos();
}

void ElectricRail::initPoints()
{
    s32 railPointNum = MR::getRailPointNum(this);
    this->mPointCount = 0;

    s32 curRail = 0;

    while (curRail < railPointNum)
    {
        s32 curRailPointArg = -1;

        bool ret = MR::getRailPointArg0NoInit(this, curRail, &curRailPointArg);

        if (!ret)
            this->mPointCount++;

        curRail++;
    }

    this->mPointCount *= this->mRailHeight;
    this->mPoints = new ElectricRailPoint[this->mPointCount];

    s32 curPointIdx = 0;
    s32 curRailPoint = 0;
    s32 flag = 0;

    while (curRailPoint < MR::getRailPointNum(this))
    {
        s32 tempArg = -1;
        bool ret = MR::getRailPointArg0NoInit(this, curRailPoint, &tempArg);

        if (!ret)
        {
            ElectricRailPoint* point = &this->mPoints[curPointIdx];
            JGeometry::TVec3<f32> temp;
            MR::calcRailPointPos(&temp, this, curRailPoint);
            point->mTranslation.set<f32>(temp);
            curPointIdx++;

            if (this->mRailHeight > 1)
            {
                JGeometry::TVec3<f32> outGrav;
                this->calcGravity(&outGrav, temp);
                outGrav.scale(-100.0f);

                s32 curHeight = 1;

                while(curHeight < this->mRailHeight)
                {
                    ElectricRailPoint* curPointInRail = &this->mPoints[curPointIdx];
                    JMathInlineVEC::PSVECAdd(&temp, &outGrav, &temp);
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

    while (curPoint < this->mPointCount)
    {
        this->mPoints[curPoint].initWithoutIter();
        curPoint++;
    }
}

void ElectricRail::initSeparators()
{
    f32 railTotalLen = MR::getRailTotalLength(this);
    f64 separatorCountF64 = railTotalLen / 200.0f;
    s32 separatorCountS32 = (s32)separatorCountF64;
    separatorCountS32++;
    this->mSeparatorCount = separatorCountS32;
    this->mSeparators = new ElectricRailSeparator[separatorCountS32];

    // todo -- finish
}

void ElectricRail::initDisplayList()
{
    MR::ProhibitSchedulerAndInterrupts prohibitScheduler(0);

    this->mDLLength = ElectricRailFunction::calcDisplayListSize(0x14, ((this->mSeparatorCount << 2) * this->mRailHeight));

    void* shit = new u8[0x20];
    this->_A0 = shit;

    GDLObj obj;
    GDInitGDLObj(&obj, shit, this->mDLLength);
    __GDCurrentDL = &obj;

    this->drawPlane(30.0, 30.0, -30.0, -30.0);
    this->drawPlane(-30.0, 30.0, 30.0, -30.0);
    GDPadCurr32();
    this->mDLLength = obj.mStart - obj.mPtr;
    DCStoreRange(this->_A0, this->mDLLength);
}

void ElectricRail::drawRailGX(f32 a1) const
{
    this->drawPlaneGX(a1, a1, -a1, -a1);
    this->drawPlaneGX(-a1, a1, a1, -a1);
}

// ElectricRail::drawPlane()
// ElectricRail::drawPlaneGX()