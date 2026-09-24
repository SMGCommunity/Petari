#include "Game/MapObj/BreakableCage.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/DummyDisplayModel.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/Util.hpp"
#include "Game/Util/MathUtil.hpp"

void BreakableCage_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace {
    const Vec cHitSensorOffsetCage = {0.0f, 0.0f, 0.0f};
    const Vec cHitSensorOffsetFixation = {0.0f, 0.0f, 0.0f};
};  // namespace

namespace NrvBreakableCage {
    NEW_NERVE(BreakableCageNrvWait, BreakableCage, Wait);
    NEW_NERVE(BreakableCageNrvWaitStartDemoBreak, BreakableCage, WaitStartDemoBreak);
    NEW_NERVE(BreakableCageNrvBreak, BreakableCage, Break);
};  // namespace NrvBreakableCage

BreakableCage::BreakableCage(const char* pName)
    : LiveActor(pName), mBreakModel(), mCageType(CAGE_INVALID), mRotationSpeed(), mItemModel(), mIgnoreGravity(), mDelayDeadActivate(),
      mCameraInfo() {
    mMtx.identity();
}

void BreakableCage::init(const JMapInfoIter& rIter) {
    TVec3f sensorOffset(::cHitSensorOffsetCage);
    const char* pName;
    f32 sensorRadius = 300.0f;

    MR::getObjectName(&pName, rIter);
    const char* pModelName = pName;

    if (MR::isEqualString(pModelName, "BreakableCageRotate")) {
        pModelName = "BreakableCage";
        mCageType = CAGE_NORMAL;
    } else if (MR::isEqualString(pName, "BreakableCageL")) {
        sensorRadius = 600.0f;
        mCageType = CAGE_LARGE;
    } else if (MR::isEqualString(pName, "BreakableFixation")) {
        mCageType = CAGE_FIX;
        sensorOffset.set(::cHitSensorOffsetFixation);
        sensorRadius = 425.0f;
    } else if (MR::isEqualString(pName, "BreakableTrash")) {
        mCageType = CAGE_TRASH;
        MR::joinToGroupArray(this, rIter, "ゴミ管理", 32);
    }

    f32 scaledRadius = sensorRadius * mScale.x;
    sensorOffset.scale(mScale.x);

    initMapToolInfo(rIter);
    initModel(pModelName, rIter);
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_BREAKABLE_CAGE, 8, scaledRadius, sensorOffset);
    MR::initCollisionParts(this, pModelName, getSensor("body"), nullptr);

    if (!isTypeCage()) {
        initEffectKeeper(0, pName, false);
    }

    MR::setClippingTypeSphere(this, 100.0f + scaledRadius);
    MR::setGroupClipping(this, rIter, 0x20);
    initSound(4, false);

    if (!mIgnoreGravity) {
        initBaseMtxForCage();
    } else {
        MR::makeMtxTR(mMtx.toMtxPtr(), this);
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (isTypeCage()) {
            MR::tryRegisterDemoCast(mBreakModel, rIter);
        }

        if (mItemModel != nullptr) {
            MR::tryRegisterDemoCast(mItemModel, rIter);
        }
    }

    MR::addToAttributeGroupSearchTurtle(this);

    if (isAppearPowerStar()) {
        MR::declarePowerStar(this);
    }

    if (MR::createActorCameraInfoIfExist(rIter, &mCameraInfo)) {
        MR::initActorCamera(this, rIter, &mCameraInfo);
    }

    initNerve(GET_NERVE(BreakableCage, BreakableCageNrvWait));

    if (MR::isExistStageSwitchSleep(rIter)) {
        MR::useStageSwitchSleep(this, rIter);
        makeActorDead();
    } else if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void BreakableCage::appear() {
    mRotation.zero();
    MR::showModel(this);
    getSensor("body")->validate();
    MR::validateClipping(this);

    if (isTypeCage()) {
        mBreakModel->kill();
    }

    if (mCageType == CAGE_TRASH && MR::isValidSwitchDead(this)) {
        MR::offSwitchDead(this);
    }

    setNerve(GET_NERVE(BreakableCage, BreakableCageNrvWait));
    LiveActor::appear();
}

void BreakableCage::kill() {
    LiveActor::kill();

    if (isTypeCage()) {
        mBreakModel->kill();
    }

    if (mItemModel != nullptr) {
        mItemModel->kill();
    }
}

void BreakableCage::calcAndSetBaseMtx() {
    if (mCageType == CAGE_NORMAL) {
        TPos3f rotationMtx;
        rotationMtx.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(mRotation.y));
        TPos3f baseMtx;
        baseMtx.concat(mMtx, rotationMtx);
        MR::setBaseTRMtx(this, baseMtx);
    } else {
        MR::setBaseTRMtx(this, mMtx);
    }
}

bool BreakableCage::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool result;

    if (MR::isMsgJetTurtleAttack(msg)) {
        result = tryBreak();
    } else {
        result = false;
    }

    return result;
}

bool BreakableCage::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool result;

    if (MR::isMsgEnemyAttackFire(msg) || MR::isMsgEnemyAttackFireStrong(msg)) {
        result = false;
    } else {
        result = tryBreak();
    }

    return result;
}

void BreakableCage::initMapToolInfo(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    if (mCageType == CAGE_NORMAL) {
        MR::getJMapInfoArg0WithInit(rIter, &mRotationSpeed);
        mRotationSpeed *= 0.01f;
    }

    if (isTypeCage()) {
        MR::getJMapInfoArg1NoInit(rIter, &mIgnoreGravity);
    } else {
        mIgnoreGravity = true;
    }

    MR::getJMapInfoArg2NoInit(rIter, &mDelayDeadActivate);
}

void BreakableCage::initModel(const char* pName, const JMapInfoIter& rIter) {
    initModelManagerWithAnm(pName, nullptr, false);

    if (isTypeCage()) {
        ModelObj* pBreakModel = MR::createModelObjMapObjStrongLight("壊れる籠壊れモデル", "BreakableCageBreak", mMtx.toMtxPtr());
        mBreakModel = pBreakModel;
        pBreakModel->initWithoutIter();
        mBreakModel->mScale.set(mScale);
        MR::invalidateClipping(mBreakModel);
        MR::registerDemoSimpleCastAll(mBreakModel);
        mBreakModel->makeActorDead();
        mItemModel = MR::createDummyDisplayModel(this, rIter, -1, TVec3f(0.0f, 150.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));

        if (mItemModel != nullptr) {
            s32 modelId = MR::getDummyDisplayModelId(rIter, -1);

            switch (modelId) {
            case 4:
                MR::startBck(mItemModel, "Rotation");
                break;
            case 7:
                break;
            }
        }
    }
}

void BreakableCage::initBaseMtxForCage() {
    MR::calcGravity(this);

    TPos3f baseMtx;
    MR::makeMtxRotate(baseMtx, mRotation);
    TVec3f front;
    baseMtx.getZDir(front);

    MR::makeMtxUpFrontPos(&mMtx, -mGravity, front, mPosition);
}

bool BreakableCage::isTypeCage() const {
    bool isCage = true;
    bool isStandardCage = true;
    bool isFixedCage = true;
    CageType type = mCageType;

    if (type != CAGE_INVALID && type != CAGE_LARGE) {
        isFixedCage = false;
    }

    if (!isFixedCage && type != CAGE_NORMAL) {
        isStandardCage = false;
    }

    if (!isStandardCage && type != CAGE_TRASH) {
        isCage = false;
    }

    return isCage;
}

bool BreakableCage::isAppearPowerStar() const {
    bool res = false;

    if (mItemModel != nullptr && MR::getDummyDisplayModelId(mItemModel) == 7) {
        res = true;
    }

    return res;
}

bool BreakableCage::tryBreak() {
    if (isNerve(GET_NERVE(BreakableCage, BreakableCageNrvWait))) {
        if (mCameraInfo != nullptr) {
            MR::requestStartDemoWithoutCinemaFrame(this, "破壊", GET_NERVE(BreakableCage, BreakableCageNrvBreak),
                                                   GET_NERVE(BreakableCage, BreakableCageNrvWaitStartDemoBreak));
        } else {
            setNerve(GET_NERVE(BreakableCage, BreakableCageNrvBreak));
        }

        return true;
    }

    return false;
}

void BreakableCage::exeWait() {
    if (mCageType == CAGE_NORMAL) {
        mRotation.y = MR::repeat(mRotation.y + mRotationSpeed, 0.0f, 360.0f);
    }

    if (mItemModel != nullptr) {
        if (isAppearPowerStar()) {
            DummyDisplayModel* pModel = mItemModel;
            mItemModel->mRotation.y = MR::repeat(pModel->mRotation.y + PowerStar::getPowerStarWaitRotateSpeed(), 0.0f, 360.0f);
        }
    }
}

void BreakableCage::exeWaitStartDemoBreak() {
}

void BreakableCage::exeBreak() {
    u32 activateDeadEarly = 1;
    u32 hasImmediateDeadCondition = 0;

    if (mDelayDeadActivate || mItemModel != nullptr || mCageType == static_cast< s32 >(CAGE_FIX) || mCageType == CAGE_TRASH) {
        hasImmediateDeadCondition = 1;
    }

    if (!hasImmediateDeadCondition && mCageType != CAGE_NORMAL) {
        activateDeadEarly = 0;
    }

    if (MR::isFirstStep(this)) {
        switch (mCageType) {
        case CAGE_FIX:
            MR::startSound(this, "SE_OJ_BREAK_FIXATION_BREAK");
            break;
        default:
            MR::startSound(this, "SE_OJ_IRON_CAGE_BREAK");
            break;
        }

        MR::hideModel(this);
        getSensor("body")->invalidate();
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);

        const ActorCameraInfo* pCamera = mCameraInfo;

        if (pCamera != nullptr) {
            MR::startActorCameraTargetSelf(this, getCamInfo(), -1);

            if (mBreakModel != nullptr) {
                MR::requestMovementOn(mBreakModel);
            }
        }

        if (isTypeCage()) {
            mBreakModel->appear();
            MR::startBck(mBreakModel, "Break");
        } else {
            MR::emitEffect(this, "Break");
        }

        if (isAppearPowerStar()) {
            if (mItemModel != nullptr) {
                MR::requestAppearPowerStar(this, mItemModel->mPosition);
            } else {
                MR::requestAppearPowerStar(this, this, 150.0f);
            }

            MR::requestMovementOn(this);
        }

        if (mItemModel != nullptr) {
            mItemModel->kill();
        }

        if (activateDeadEarly && MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
    }

    if (isAppearPowerStar() || mCameraInfo != nullptr) {
        MR::stopSceneAtStep(this, 2, 16);
    } else if (MR::isNearPlayer(this, 1500.0f)) {
        f32 radius = getSensor("body")->mRadius;

        if (!MR::isJudgedToClipFrustum(mPosition, radius)) {
            MR::stopSceneAtStep(this, 2, 6);
        }
    }

    bool canDoSwitch;

    if (mCameraInfo != nullptr) {
        canDoSwitch = MR::isStep(this, 120);
    } else if (isTypeCage()) {
        canDoSwitch = MR::isBckStopped(mBreakModel);
    } else {
        canDoSwitch = MR::isEffectValid(this, "Break") == false;
    }

    if (canDoSwitch) {
        if (!activateDeadEarly && MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }

        if (MR::isValidSwitchB(this)) {
            MR::offSwitchB(this);
        }

        if (mCameraInfo != nullptr) {
            MR::endActorCamera(this, getCamInfo(), false, -1);
            MR::endDemo(this, "破壊");
        }

        kill();
    }
}

BreakableCage::~BreakableCage() {
}
