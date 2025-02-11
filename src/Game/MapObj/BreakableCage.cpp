#include "Game/MapObj/BreakableCage.hpp"
#include "Game/MapObj/PowerStar.hpp"

inline f32 normalizeAngle(f32 a1, f32 a2) {
    return a1 + (f32)fmod(360.0f + (a2 - a1), 360.0f);
}

BreakableCage::BreakableCage(const char *pName) : LiveActor(pName) {
    mBreakModel = nullptr;
    mCageType = CAGE_INVALID;
    mRotationSpeed = 0.0f;
    mItemModel = nullptr;
    mIgnoreGravity = false;
    mDelayDeadActivate = false;
    mCameraInfo = nullptr;
    mMtx.identity();
}

/*
void BreakableCage::init(const JMapInfoIter &rIter) {
    TVec3f stack_C;
    stack_C.setInlinePS(cHitSensorOffsetCage);
    const char* name;
    f32 v7 = 300.0f;
    stack_C.z = 0.0f;
    MR::getObjectName(&name, rIter);
    const char* modelName = name;

    if (MR::isEqualString(modelName, "BreakableCageRotate")) {
        name = "BreakableCage";
        mCageType = CAGE_NORMAL;
    }
    else if (MR::isEqualString(name, "BreakableCageL")) {
        v7 = 600.0f;
        mCageType = CAGE_LARGE;
    }
    else if (MR::isEqualString(name, "BreakableFixation")) {
        mCageType = CAGE_FIX;
        stack_C.setInline(cHitSensorOffsetFixation);
        v7 = 425.0f;
    }
    else if (MR::isEqualString(name, "BreakableTrash")) {
        mCageType = CAGE_TRASH;
        MR::joinToGroupArray(this, rIter, "ゴミ管理", 32);
    }

    f32 v9 = v7 * mScale.x;
    stack_C.x = stack_C.x * mScale.x;
    stack_C.y = stack_C.y * mScale.x;
    stack_C.z = stack_C.z * mScale.x;
    initMapToolInfo(rIter);
    initModel(modelName, rIter);
    MR::connectToSceneMapObjStrongLight(this);
    initHitSensor(1);
    MR::addHitSensor(this, "body", 82, 8, v9, stack_C);
    MR::initCollisionParts(this, modelName, getSensor("body"), nullptr);

    if (!isTypeCage()) {
        initEffectKeeper(0, name, false);
    }

    MR::setClippingTypeSphere(this, 100.0f + v9);
    MR::setGroupClipping(this, rIter, 0x20);
    initSound(4, false);
    if (mIgnoreGravity) {
        MR::makeMtxTR(mMtx.toMtxPtr(), this);
    }
    else {
        initBaseMtxForCage(); 
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (isTypeCage()) {
            MR::tryRegisterDemoCast(mBreakModel, rIter);
        }

        if (mItemModel) {
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

    initNerve(&NrvBreakableCage::BreakableCageNrvWait::sInstance);
    if (MR::isExistStageSwitchSleep(rIter)) {
        MR::useStageSwitchSleep(this, rIter);
        makeActorDead();
    }
    else if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    }
    else {
        makeActorAppeared();
    }
}
*/

void BreakableCage::appear() {
    mRotation.x = 0.0f;
    mRotation.y = 0.0f;
    mRotation.z = 0.0f;
    MR::showModel(this);
    getSensor("body")->validate();
    MR::validateClipping(this);

    if (isTypeCage()) {
        mBreakModel->kill();
    }

    if (mCageType == CAGE_TRASH && MR::isValidSwitchDead(this)) {
        MR::offSwitchDead(this);
    }

    setNerve(&NrvBreakableCage::BreakableCageNrvWait::sInstance);
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

#ifndef NON_MATCHING
void BreakableCage::calcAndSetBaseMtx() {
    if (mCageType == CAGE_NORMAL) {
        f32 v2 = (0.017453292f * mRotation.y);
        TVec3f stack_C;
        TVec3f stack_14(0.0f, 1.0f, 0.0f);
        TPos3f stack_20;
        stack_20.mMtx[0][3] = 0.0f;
        stack_20.mMtx[1][3] = 0.0f;
        stack_20.mMtx[2][3] = 0.0f;
        stack_C.set<f32>(stack_14);
        PSVECMag(&stack_C);
        PSVECNormalize(&stack_C, &stack_C);
        TPos3f stack_50;
        f32 v3 = sin(v2);
        f32 v4 = cos(v2);
        stack_50.mMtx[0][0] = v4 + ((1.0f - v4) * (stack_C.x * stack_C.x));
        stack_50.mMtx[1][1] = v4 + ((1.0f - v4) * (stack_C.y * stack_C.y));
        stack_50.mMtx[2][2] = v4 + ((1.0f - v4) * (stack_C.z * stack_C.z));
        stack_50.mMtx[0][1] = (stack_C.y * ((1.0f - v4) * stack_C.x)) - (v3 * stack_C.z);
        stack_50.mMtx[0][2] = (stack_C.z * ((1.0f - v4) * stack_C.x)) + (v3 * stack_C.y);
        stack_50.mMtx[1][0] = (stack_C.y * ((1.0f - v4) * stack_C.x)) + (v3 * stack_C.z);
        stack_50.mMtx[2][0] = (stack_C.z * ((1.0f - v4) * stack_C.x)) - (v3 * stack_C.y);
        stack_50.mMtx[1][2] = (stack_C.z * ((1.0f - v4) * stack_C.y)) - (v3 * stack_C.x);
        stack_50.mMtx[2][1] = (stack_C.z * ((1.0f - v4) * stack_C.y)) + (v3 * stack_C.x);

        stack_20.concat(mMtx, stack_50);
        MR::setBaseTRMtx(this, stack_20);
    }
    else {
        MR::setBaseTRMtx(this, mMtx);
    }
}
#endif

bool BreakableCage::receiveMsgPlayerAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    bool result;
    if (MR::isMsgJetTurtleAttack(msg)) {
        result = tryBreak();
    }
    else {
        result = false;
    }

    return result;
}

bool BreakableCage::receiveMsgEnemyAttack(u32 msg, HitSensor *a2, HitSensor *a3) {
    bool result;

    if (MR::isMsgEnemyAttackFire(msg) || MR::isMsgEnemyAttackFireStrong(msg)) {
        result = false;
    }
    else {
        result = tryBreak();
    }

    return result;
}

void BreakableCage::initMapToolInfo(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchReadAppear(this, rIter);
    MR::useStageSwitchWriteA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);

    if (mCageType == CAGE_NORMAL) {
        MR::getJMapInfoArg0WithInit(rIter, &mRotationSpeed);
        mRotationSpeed *= 0.0099999998f;
    }

    if (isTypeCage()) {
        MR::getJMapInfoArg1NoInit(rIter, &mIgnoreGravity);
    }
    else {
        mIgnoreGravity = true;
    }

    MR::getJMapInfoArg2NoInit(rIter, &mDelayDeadActivate);
}

void BreakableCage::initModel(const char *pName, const JMapInfoIter &rIter) {
    initModelManagerWithAnm(pName, nullptr, false);
    if (isTypeCage()) {
        ModelObj* obj = MR::createModelObjMapObjStrongLight("壊れる籠壊れモデル", "BreakableCageBreak", mMtx.toMtxPtr());
        mBreakModel = obj;
        obj->initWithoutIter();
        mBreakModel->mScale.set<f32>(mScale);
        MR::invalidateClipping(mBreakModel);
        MR::registerDemoSimpleCastAll(mBreakModel);
        mBreakModel->makeActorDead();
        mItemModel = MR::createDummyDisplayModel(this, rIter, -1, TVec3f(0.0f, 150.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
        if (mItemModel != nullptr) {
            s32 model_id = MR::getDummyDisplayModelId(rIter, -1);

            switch (model_id) {
                case 4:
                    MR::startBck(mItemModel, "Rotation", nullptr);
                    break;
                case 7:
                    break;
            }
        }
    }
}

/*
void BreakableCage::initBaseMtxForCage() {
    MR::calcGravity(this);

    TMtx34f stack_20;
    MR::makeMtxRotate(stack_20.toMtxPtr(), mRotation);
    TVec3f stack_14;
    f32 z = stack_20.mMtx[2][2];
    f32 y = stack_20.mMtx[1][2];
    f32 x = stack_20.mMtx[0][2];
    stack_14.set(x, y, z);
    TVec3f stack_8;
    stack_8.negateInline_2(mGravity);
    MR::makeMtxUpFrontPos(&mMtx, stack_8, stack_14, mPosition);
}
*/

bool BreakableCage::isTypeCage() const {
    bool res = true;
    bool v3 = true;
    bool v4 = true;
    CageType type = mCageType;

    if (type && type != CAGE_LARGE) {
        v4 = false;
    }

    if (!v4 && type != CAGE_NORMAL) {
        v3 = false;
    }

    if (!v3 && type != CAGE_TRASH) {
        res = false;
    }

    return res;
}

bool BreakableCage::isAppearPowerStar() const {
    bool res = false;
    if (mItemModel != nullptr && MR::getDummyDisplayModelId(mItemModel) == 7) {
        res = true;
    }
 
    return res;
}

bool BreakableCage::tryBreak() {
    if (isNerve(&NrvBreakableCage::BreakableCageNrvWait::sInstance)) {
        if (mCameraInfo != nullptr) {
            MR::requestStartDemoWithoutCinemaFrame(this, "破壊", &NrvBreakableCage::BreakableCageNrvBreak::sInstance, &NrvBreakableCage::BreakableCageNrvWaitStartDemoBreak::sInstance);
        }
        else {
            setNerve(&NrvBreakableCage::BreakableCageNrvBreak::sInstance);
        }

        return true;
    }

    return false;
}

void BreakableCage::exeWait() {
    if (mCageType == CAGE_NORMAL) {
        mRotation.y = normalizeAngle(0.0f, mRotationSpeed + mRotation.y);
    }

    if (mItemModel != nullptr) {
        if (isAppearPowerStar()) {
            DummyDisplayModel* model = mItemModel;
            mItemModel->mRotation.y = normalizeAngle(0.0f, model->mRotation.y + PowerStar::getPowerStarWaitRotateSpeed());
        }
    }
}

void BreakableCage::exeBreak() {
    u32 v2 = 1;
    u32 v3 = 0;
    ActorCameraInfo* inf;

    if (mDelayDeadActivate || mItemModel != nullptr || mCageType == (s32)3 || mCageType == CAGE_TRASH) {
        v3 = 1;
    }

    if (!v3 && mCageType != CAGE_NORMAL) {
        v2 = 0;
    }

    if (MR::isFirstStep(this)) {
        switch(mCageType) {
            case CAGE_FIX:
                MR::startSound(this, "SE_OJ_BREAK_FIXATION_BREAK", -1, -1);
                break;
            default:
                MR::startSound(this, "SE_OJ_IRON_CAGE_BREAK", -1, -1);
                break;
        }

        MR::hideModel(this);
        getSensor("body")->invalidate();
        MR::invalidateCollisionParts(this);
        MR::invalidateClipping(this);

        const ActorCameraInfo* camera = mCameraInfo;

        if (camera) {
            MR::startActorCameraTargetSelf(this, getCamInfo(), -1);
            if (mBreakModel != nullptr) {
                MR::requestMovementOn(mBreakModel);
            }
        }

        if (isTypeCage()) {
            mBreakModel->appear();
            MR::startBck(mBreakModel, "Break", nullptr);
        }
        else { 
            MR::emitEffect(this, "Break");
        }

        if (isAppearPowerStar()) {
            if (mItemModel != nullptr) {
                MR::requestAppearPowerStar(this, mItemModel->mPosition);
            }
            else {
                MR::requestAppearPowerStar(this, this, 150.0f);
            }

            MR::requestMovementOn(this);
        }

        if (mItemModel != nullptr) {
            mItemModel->kill();
        }

        if (v2 && MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }
    }

    if (isAppearPowerStar() || mCameraInfo != nullptr) {
        MR::stopSceneAtStep(this, 2, 16);
    }
    else if (MR::isNearPlayer(this, 1500.0f)) {
        f32 radius = getSensor("body")->mRadius;
        if (!MR::isJudgedToClipFrustum(mPosition, radius)) {
            MR::stopSceneAtStep(this, 2, 6);
        }
    }

    bool canDoSwitch;

    if (mCameraInfo != nullptr) {
        canDoSwitch = MR::isStep(this, 120);
    }
    else if (isTypeCage()) {
        canDoSwitch = MR::isBckStopped(mBreakModel);
    }
    else {
        canDoSwitch = MR::isEffectValid(this, "Break") == false;
    }
    
    if (canDoSwitch) {
        if (!v2 && MR::isValidSwitchDead(this)) {
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

namespace NrvBreakableCage {
    INIT_NERVE(BreakableCageNrvWait);
    INIT_NERVE(BreakableCageNrvWaitStartDemoBreak);
    INIT_NERVE(BreakableCageNrvBreak);

    void BreakableCageNrvBreak::execute(Spine *pSpine) const {
        BreakableCage* cage = reinterpret_cast<BreakableCage*>(pSpine->mExecutor);
        cage->exeBreak();
    }

    void BreakableCageNrvWaitStartDemoBreak::execute(Spine *pSpine) const {

    }

    void BreakableCageNrvWait::execute(Spine *pSpine) const {
        BreakableCage* cage = reinterpret_cast<BreakableCage*>(pSpine->mExecutor);
        cage->exeWait();
    }
};
