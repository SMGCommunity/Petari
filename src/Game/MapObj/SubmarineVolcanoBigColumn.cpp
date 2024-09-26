#include "Game/MapObj/SubmarineVolcanoBigColumn.hpp"
#include "Game/MapObj/AirBubbleHolder.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include <cstdio>

namespace {
    static f32 sShakeDistance = 800.0f;
};

SubmarineVolcanoBigColumn::SubmarineVolcanoBigColumn(const char *pName) : LiveActor(pName) {
    mBreakModel = nullptr;
    mIsSmallColumn = false;
}

void SubmarineVolcanoBigColumn::init(const JMapInfoIter &rIter) {
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

#ifdef NON_MATCHING
void SubmarineVolcanoBigColumn::exeWait() {
    if (mIsSmallColumn) {
        if (MR::isStep(this, 360)) {
            TVec3f upVec;
            MR::calcUpVec(&upVec, this);
            TVec3f v11;
            v11.scale(125.0f, upVec);
            TVec3f pos;
            pos.setInlinePS(v11);
            pos.addInline_4(mPosition);
            MR::appearAirBubble(pos, 360);
            setNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvWait::sInstance);
        }
    }
}
#endif

void SubmarineVolcanoBigColumn::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startRumbleWithShakeCameraWeak(this, "強", "弱", sShakeDistance, 3.4028235e38f);
        MR::hideModel(this);
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
        mBreakModel->appear();
        MR::startBck(mBreakModel, "Break", nullptr);

        if (mIsSmallColumn) {
            MR::startSound(this, "SE_OJ_SMN_VOLC_COL_S_BREAK", -1, -1);
        }
        else {
            MR::startSound(this, "SE_OJ_SMN_VOLC_COL_L_BREAK", -1, -1);
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

bool SubmarineVolcanoBigColumn::receiveMsgEnemyAttack(u32 msg, HitSensor *, HitSensor *) {
    if (isNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvWait::sInstance)) {
        setNerve(&NrvSubmarineVolcanoBigColumn::SubmarineVolcanoBigColumnNrvBreak::sInstance);
        return true;
    }

    return false;
}

void SubmarineVolcanoBigColumn::initBreakModel(const char *pModelName) {
    char buf[256];
    snprintf(buf, sizeof(buf), "%sBreak", pModelName);
    mBreakModel = MR::createModelObjMapObj("海底火山石柱壊れモデル", buf, getBaseMtx());
    MR::invalidateClipping(mBreakModel);
    mBreakModel->makeActorDead();
}

SubmarineVolcanoBigColumn::~SubmarineVolcanoBigColumn() {

}

namespace NrvSubmarineVolcanoBigColumn {
    INIT_NERVE(SubmarineVolcanoBigColumnNrvWait);
    INIT_NERVE(SubmarineVolcanoBigColumnNrvBreak);
};