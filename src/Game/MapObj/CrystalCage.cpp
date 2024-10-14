#include "Game/MapObj/CrystalCage.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath.hpp"

CrystalCage::CrystalCage(const char *pName) : LiveActor(pName), 
    mCrystalCageType(0), mBreakObj(nullptr), _C4(1), _C8(0), mRumbleCalc(nullptr), _D0(0.0f, 1.0f), _DC(gZeroVec), _E8(gZeroVec),
    mDisplayModel(nullptr), _F8(gZeroVec), _104(0), _108(-1), mIsBreakObjVisible(false), mPlayRiddleSFX(false), mHasBinding(false), _110(gZeroVec) {
        _94.identity();
}

void CrystalCage::init(const JMapInfoIter &rIter) {
    const char* obj_name;
    TVec3f v23;
    MR::getObjectName(&obj_name, rIter);
    initMapToolInfo(rIter);
    MR::calcGravity(this);
    initModel(obj_name);
    MR::connectToSceneCrystal(this);
    _E8.set(mPosition);
    initHitSensor(1);
    MR::addHitSensorPosMapObj(this, "body", 8, (130.0f * mScale.x), &_E8, TVec3f(0.0f, 0.0f, 0.0f));

    TVec3f v21;

    if (mCrystalCageType) {
        MR::invalidateHitSensors(this);
    }

    if (mHasBinding) {
        initBinder(50.0f, 0.0f, 0);
        MR::setBinderOffsetVec(this, &_110, nullptr);
        MR::setBinderExceptActor(this, this);
    }

    if (mCrystalCageType != 2) {
        initEffectKeeper(0, nullptr, false);
    }

    MR::initCollisionParts(this, obj_name, getSensor("body"), nullptr);

    f32 range = !mCrystalCageType ? 300.0f : 600.0f;
    MR::setClippingTypeSphere(this, range * mScale.x);
    initSound(4, false);

    if (mCrystalCageType == 2) {
        f32 mult = mScale.x * 250.0f;
        _C4 = 2;
        
        v21.x = 0.0f;
        v21.y = mult;
        v21.z = 0.0f;
        mDisplayModel = MR::createDummyDisplayModelCrystalItem(this, 7, v21, TVec3f(0.0f, 0.0f, 0.0f));

        if (_108 == -1) {
            MR::declarePowerStar(this);
        }
        else {
            MR::declarePowerStar(this, _108);
        }
    }
    else {
        if (!mCrystalCageType) {
            v23.set(-30.0f, 100.0f, -30.0f);
        }
        else {
            v23.set(0.0f, 200.0f, 0.0f);
        }

        mDisplayModel = MR::createDummyDisplayModelCrystalItem(this, rIter, v23, TVec3f(0.0f, 0.0f, 0.0f));
        
        if (mDisplayModel) {
            switch (MR::getDummyDisplayModelId(rIter, -1)) {
                case 0:
                    MR::declareCoin(this, 1);
                    break;
                case 4:
                    MR::startBva(mDisplayModel, "Freeze");
            }
        }
    }

    MR::startBva(this, obj_name);
    MR::setBvaFrameAndStop(this, 0.0f);

    if (_C4 > 1) {
        mRumbleCalc = new RumbleCalculatorCosMultLinear(4.0f, 1.5707964f, 50.0f, 0x14);
    }

    MR::joinToGroupArray(this, rIter, nullptr, 0x20);
    initNerve(&NrvCrystalCage::CrystalCageNrvWait::sInstance);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (mDisplayModel) {
            MR::tryRegisterDemoCast(mDisplayModel, rIter);
        }
    }

    if (mCrystalCageType == 2) {
        makeActorAppeared();
        return;
    }

    if (MR::isExistStageSwitchSleep(rIter)) {
        MR::useStageSwitchSleep(this, rIter);
        makeActorDead();
        return;
    }

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}

void CrystalCage::initAfterPlacement() {
    if (!mIsBreakObjVisible || mHasBinding) {
        f32 val = mCrystalCageType == 2 ? 1000.0f : 300.0f;
        TVec3f up_vec;
        TVec3f stack_2C;
        TVec3f stack_20;
        MR::calcUpVec(&up_vec, this);
        // I realy do not like this, but it matches :c
        JMathInlineVEC::PSVECAdd(mPosition.toCVec(), (Vec *) &(up_vec * (val * mScale.x)), stack_20.toVec());
        stack_2C.scale((-(2.0f * val) * mScale.x), up_vec);

        if (!MR::getFirstPolyOnLineToMapExceptActor(&_F8, 0, stack_20, stack_2C, this)) {
            _F8.set(mPosition);
        }

        if (mHasBinding) {
            _110.sub(_F8, mPosition);
            JMathInlineVEC::PSVECAdd(_110.toCVec(), (up_vec * 50.0f).toCVec(), _110.toVec());
            mVelocity.scale(-2.0f, up_vec);
        }
    }
}

void CrystalCage::kill() {
    LiveActor::kill();
    mBreakObj->kill();

    if (mDisplayModel) {
        mDisplayModel->kill();
    }
}

void CrystalCage::forceBreak() {
    if (isNerve(&NrvCrystalCage::CrystalCageNrvWait::sInstance)) {
        if (!_104 || MR::isClipped(this)) {
            MR::tryRumblePadStrong(this, 0);
            MR::invalidateClipping(this);
            MR::invalidateCollisionParts(this);
            MR::invalidateHitSensors(this);
            setNerve(&NrvCrystalCage::CrystalCageNrvBreak::sInstance);
        }
        else {
            kill();
        }
    }
}

void CrystalCage::attackSensor(HitSensor *a1, HitSensor *a2) {
    if (!mCrystalCageType) {
        MR::sendMsgPush(a2, a1);
    }
}

bool CrystalCage::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (!isNerve(&NrvCrystalCage::CrystalCageNrvWait::sInstance)) {
        return 0;
    }

    if (!MR::isMsgPlayerHitAll(msg)) {
        return false;
    }

    if (_C4 > 0 && !_C8) {
        if (_C4 == 1) {
            MR::invalidateClipping(this);
            MR::invalidateCollisionParts(this);
            MR::invalidateHitSensors(this);
            setNerve(&NrvCrystalCage::CrystalCageNrvBreak::sInstance);
            MR::startCSSound("CS_SPIN_HIT", nullptr, 0);
            return 1;
        }

        if (MR::sendArbitraryMsg(63, a2, a3)) {
            _C8 = 20;
            _C4 = _C4 - 1;

            MR::setBvaFrameAndStop(this, 1.0f);
            if (mCrystalCageType == 2) {
                MR::startSound(this, "SE_OJ_CRYSTAL_CAGE_L_DAMAGE", -1, -1);
            }
            else {
                MR::startSound(this, "SE_OJ_CRYSTAL_CAGE_S_DAMAGE", -1, -1);
            }

            mRumbleCalc->start(0);
            _D0.sub(a2->mPosition, a3->mPosition);
            MR::normalize(&_D0);
            return 1;
        }
    }

    return 0;
}

bool CrystalCage::receiveMsgEnemyAttack(u32 msg, HitSensor *, HitSensor *) {
    if (mCrystalCageType != 2 && isNerve(&NrvCrystalCage::CrystalCageNrvWait::sInstance)) {
        MR::invalidateClipping(this);
        MR::invalidateCollisionParts(this);
        MR::invalidateHitSensors(this);
        setNerve(&NrvCrystalCage::CrystalCageNrvBreak::sInstance);
        return 1;
    }

    return 0;
}

void CrystalCage::initMapToolInfo(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    _DC.set(mPosition);

    if (MR::isEqualObjectName(rIter, "CrystalCageS")) {
        mCrystalCageType = 0;
    }
    else if (MR::isEqualObjectName(rIter, "CrystalCageM")) {
        mCrystalCageType = 1;
    }
    else {
        mCrystalCageType = 2;
    }

    if (mCrystalCageType == 2) {
        MR::getJMapInfoArg0NoInit(rIter, &_108);
    }
    else {
        MR::useStageSwitchWriteDead(this, rIter);

        if (MR::useStageSwitchReadA(this, rIter)) {
            MR::FunctorV0M<CrystalCage *, void (CrystalCage::*)()> forceFunc = MR::Functor<CrystalCage>(this, &CrystalCage::forceBreak);
            MR::listenStageSwitchOnA(this, forceFunc);
        }

        MR::getJMapInfoArg0NoInit(rIter, &mIsBreakObjVisible);
        MR::getJMapInfoArg1NoInit(rIter, &mPlayRiddleSFX);
        MR::getJMapInfoArg2NoInit(rIter, &mHasBinding);
        MR::getJMapInfoArg3NoInit(rIter, &_104);
    }
}

void CrystalCage::initModel(const char *pName) {
    initModelManagerWithAnm(pName, nullptr, false);
    TVec3f stack_8;
    stack_8.negateInline_2(mGravity);
    MR::makeMtxUpNoSupportPos(&_94, stack_8, _DC);

    if (mCrystalCageType == 2) {
        mBreakObj = MR::createModelObjMapObjStrongLight("クリスタルケージ[大]壊れモデル", "CrystalCageLBreak", (MtxPtr)&_94);
    }
    else {
        mBreakObj = MR::createModelObjMapObjStrongLight("クリスタルケージ[小]壊れモデル", "CrystalCageSBreak", (MtxPtr)&_94);
    }

    MR::invalidateClipping(this);
    mBreakObj->mScale.set(mScale);
    MR::registerDemoSimpleCastAll(mBreakObj);
    mBreakObj->makeActorDead();
}

void CrystalCage::tryOnSwitchDead() {
    if (mCrystalCageType == 2 || !MR::isValidSwitchDead(this)) {
        return;
    }

    s32 step = 0;

    if (mDisplayModel && MR::getDummyDisplayModelId(mDisplayModel) == 3) {
        step = 10;
    }

    if (MR::isStep(this, step)) {
        MR::onSwitchDead(this);
    }
}

void CrystalCage::exeWait() { 
    if (_C8 > 0) {
        _C8--;
    }

    if (mRumbleCalc) {
        if (mRumbleCalc->_4 < mRumbleCalc->_8) {
            mRumbleCalc->calc();
            TVec3f v9;
            v9.scale(mRumbleCalc->_C.y, _D0);
            JMathInlineVEC::PSVECAdd(_DC.toCVec(), v9.toCVec(), mPosition.toVec());
        }
        else {
            mPosition.set(_DC);
        }
    }

    if (mCrystalCageType == 0) {
        TVec3f up_vec;
        MR::calcUpVec(&up_vec, this);
        TVec3f v6;
        v6.scale(-40.0f, up_vec);
        JMathInlineVEC::PSVECAdd(v6.toCVec(), mPosition.toCVec(), v6.toVec());
        TVec3f v5;
        v5.scale(140.0f, up_vec);
        JMathInlineVEC::PSVECAdd(v5.toCVec(), mPosition.toCVec(), v5.toVec());
        MR::calcPerpendicFootToLineInside(&_E8, *MR::getPlayerPos(), v6, v5);
    }
}

void CrystalCage::exeBreak() {
    if (MR::isFirstStep(this)) {
        if (mIsBreakObjVisible) {
            MR::hideModel(this);
        }
        else {
            MR::setBvaFrameAndStop(this, mCrystalCageType == 2 ? 2.0f : 1.0f);
            mPosition.set(_F8);
        }

        if (mHasBinding) {
            MR::offBind(this);
            mVelocity.z = 0.0f;
            mVelocity.y = 0.0f;
            mVelocity.x = 0.0f;
        }

        mBreakObj->appear();
        MR::startBck(mBreakObj, "Break", nullptr);

        if (mCrystalCageType == 2) {
            MR::startSound(this, "SE_OJ_CRYSTAL_CAGE_L_BREAK", -1, -1);
            if (_108 == -1) {
                MR::requestAppearPowerStar(this, mDisplayModel->mPosition);
            }
            else {
                MR::requestAppearPowerStar(this, _108, mDisplayModel->mPosition);
            }
        }
        else {
            MR::emitEffect(this, "Break");
            MR::startSound(this, "SE_OJ_CRYSTAL_CAGE_S_BREAK", -1, -1);

            if (mPlayRiddleSFX) {
                MR::startSystemSE("SE_SY_READ_RIDDLE_S", -1, -1);
            }
        }

        if (mDisplayModel && !MR::getDummyDisplayModelId(mDisplayModel)) {
            MR::appearCoinPop(this, mDisplayModel->mPosition, 1);
        }

        if (mDisplayModel && MR::getDummyDisplayModelId(mDisplayModel) == 5) {
            MR::startSound(this, "SE_OJ_CRY_CAGE_MV_TICO_APR", -1, -1);
        }

        if (mDisplayModel && MR::getDummyDisplayModelId(mDisplayModel) == 1) {
            MR::startSound(this, "SE_SV_KINOPIO_PUHA", -1, -1);
        }

        if (mDisplayModel) {
            mDisplayModel->kill();
        }
    }

    if (mCrystalCageType != 2 && MR::isStep(this, 2)) {
        MR::stopScene(3);
    }

    tryOnSwitchDead();

    if (MR::isBckStopped(mBreakObj)) {
        if (mIsBreakObjVisible) {
            kill();
        }
        else {
            setNerve(&NrvCrystalCage::CrystalCageNrvBreakAfter::sInstance);
        }
    }
}

void CrystalCage::exeBreakAfter() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        mBreakObj->kill();
    }
}

namespace NrvCrystalCage {
    INIT_NERVE(CrystalCageNrvWait);
    INIT_NERVE(CrystalCageNrvBreak);
    INIT_NERVE(CrystalCageNrvBreakAfter);

    void CrystalCageNrvBreakAfter::execute(Spine *pSpine) const {
        CrystalCage* cage = reinterpret_cast<CrystalCage*>(pSpine->mExecutor);
        cage->exeBreakAfter();
    }

    void CrystalCageNrvBreak::execute(Spine *pSpine) const {
        CrystalCage* cage = reinterpret_cast<CrystalCage*>(pSpine->mExecutor);
        cage->exeBreak();
    }

    void CrystalCageNrvWait::execute(Spine *pSpine) const {
        CrystalCage* cage = reinterpret_cast<CrystalCage*>(pSpine->mExecutor);
        cage->exeWait();
    }
};

CrystalCage::~CrystalCage() {

}
