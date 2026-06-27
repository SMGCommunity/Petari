#include "Game/MapObj/SubmarineVolcanoBigColumn.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/AirBubbleHolder.hpp"
#include "Game/Util.hpp"
#include <cstdio>

namespace {
    static const f32 sShakeDistance = 800.0f;
    static const s32 sGenerateInterval = 360;
    static const s32 sGenerateLife = 360;
    static const f32 sGenerateOffsetY = 125.0f;
};  // namespace

namespace NrvSubmarineVolcanoBigColumn {
    NEW_NERVE(SubmarineVolcanoBigColumnNrvWait, SubmarineVolcanoBigColumn, Wait);
    NEW_NERVE(SubmarineVolcanoBigColumnNrvBreak, SubmarineVolcanoBigColumn, Break);
};  // namespace NrvSubmarineVolcanoBigColumn

SubmarineVolcanoBigColumn::SubmarineVolcanoBigColumn(const char* pName) : LiveActor(pName) {
    mBreakModel = nullptr;
    mIsSmallColumn = false;
}

void SubmarineVolcanoBigColumn::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    const char* objName;
    MR::getObjectName(&objName, rIter);
    mIsSmallColumn = MR::isEqualString("SubmarineVolcanoSmallColumn", objName);
    initModelManagerWithAnm(objName, nullptr, false);
    initBreakModel(objName);
    MR::connectToSceneMapObj(this);
    initHitSensor(1);
    MR::addBodyMessageSensorMapObj(this);
    MR::initCollisionParts(this, objName, getSensor("body"), nullptr);
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    MR::setClippingTypeSphereContainsModelBoundingBox(this, 100.0f);
    MR::joinToGroupArray(this, rIter, nullptr, 32);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::tryRegisterDemoCast(mBreakModel, rIter);
    }

    if (mIsSmallColumn) {
        MR::createAirBubbleHolder();
    }

    initNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvWait::sInstance);
    makeActorAppeared();
}

void SubmarineVolcanoBigColumn::kill() {
    mBreakModel->kill();
    LiveActor::kill();
}

void SubmarineVolcanoBigColumn::exeWait() {
    if (mIsSmallColumn) {
        if (MR::isStep(this, ::sGenerateInterval)) {
            TVec3f upVec;
            MR::calcUpVec(&upVec, this);
            TVec3f v11;
            v11.scale(::sGenerateOffsetY, upVec);
            MR::appearAirBubble(mPosition.addOtherInline2(v11), ::sGenerateLife);
            setNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvWait::sInstance);
        }
    }
}

void SubmarineVolcanoBigColumn::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startRumbleWithShakeCameraWeak(this, "強", "弱", ::sShakeDistance, FLOAT_MAX);
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
        mBreakModel->appear();
        MR::startBck(mBreakModel, "Break", nullptr);

        if (mIsSmallColumn) {
            MR::startSound(this, "SE_OJ_SMN_VOLC_COL_S_BREAK");
        } else {
            MR::startSound(this, "SE_OJ_SMN_VOLC_COL_L_BREAK");
        }
    }

    if (MR::isBckStopped(mBreakModel)) {
        kill();
    }
}

void SubmarineVolcanoBigColumn::pauseOff() {
    MR::requestMovementOn(this);
    MR::requestMovementOn(mBreakModel);
}

bool SubmarineVolcanoBigColumn::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvWait::sInstance)) {
        setNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvBreak::sInstance);
        return true;
    }

    return false;
}

void SubmarineVolcanoBigColumn::initBreakModel(const char* pModelName) {
    char modelName[256];
    snprintf(modelName, sizeof(modelName), "%sBreak", pModelName);
    mBreakModel = MR::createModelObjMapObj("海底火山石柱壊れモデル", modelName, getBaseMtx());
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
}

SubmarineVolcanoBigColumn::~SubmarineVolcanoBigColumn() {
}
