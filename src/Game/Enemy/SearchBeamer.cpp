#include "Game/Enemy/SearchBeamer.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointController.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

namespace NrvSearchBeamer {
    NEW_NERVE_ONEND(SearchBeamerNrvNonActive, SearchBeamer, NonActive, NonActive);
    NEW_NERVE(SearchBeamerNrvCloseWaitFar, SearchBeamer, CloseWaitFar);
    NEW_NERVE(SearchBeamerNrvCloseSearch, SearchBeamer, CloseSearch);
    NEW_NERVE(SearchBeamerNrvCloseWaitNear, SearchBeamer, CloseWaitNear);
    NEW_NERVE(SearchBeamerNrvOpenMouth, SearchBeamer, OpenMouth);
    NEW_NERVE(SearchBeamerNrvBeamPrepare, SearchBeamer, BeamPrepare);
    NEW_NERVE(SearchBeamerNrvBeamStart, SearchBeamer, BeamStart);
    NEW_NERVE(SearchBeamerNrvBeamAim, SearchBeamer, BeamAim);
    NEW_NERVE(SearchBeamerNrvCloseMouth, SearchBeamer, CloseMouth);
    NEW_NERVE(SearchBeamerNrvStopStart, SearchBeamer, StopStart);
    NEW_NERVE(SearchBeamerNrvStop, SearchBeamer, Stop);
    NEW_NERVE(SearchBeamerNrvStopForPlayerOff, SearchBeamer, StopForPlayerOff);
    NEW_NERVE(SearchBeamerNrvRecover, SearchBeamer, Recover);
    NEW_NERVE_ONEND(SearchBeamerNrvDPDSwoon, SearchBeamer, DPDSwoon, DPDSwoon);
};  // namespace NrvSearchBeamer

SearchBeamer::SearchBeamer(const char* pName) : LiveActor(pName) {
    mScaleController = nullptr;
    mBindStarPointer = nullptr;
    _94.set(0.0f, 0.0f, 1.0f);
    _A0.set(0.0f, 0.0f, 0.0f);
    _AC = 0;
    _B0 = 0;
    mPropellerJointCtrl = nullptr;
    mBeanStartJointCtrl = nullptr;
    mBeamEndJointCtrl = nullptr;
    mBeamStart.set(0.0f, 0.0f, 0.0f);
    mBeamEnd.set(0.0f, 0.0f, 0.0f);
    _138 = 8.0f;
    _13C = 1000.0f;
    _140 = 0;
    _B8.identity();
}

void SearchBeamer::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SearchBeamer", nullptr, false);
    MR::connectToSceneCollisionMapObjStrongLight(this);
    MR::initLightCtrl(this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 8, 60.0f, TVec3f(0.0f, -80.0f, 0.0f));
    MR::addHitSensorCallbackEnemyAttack(this, "beam", 8, 15.0f);
    MR::invalidateHitSensor(this, "beam");
    _B8.setInline(getBaseMtx());
    MR::initCollisionParts(this, "Face", getSensor("body"), _B8);
    initEffectKeeper(1, nullptr, false);
    initSound(4, false);
    MR::initStarPointerTarget(this, 80.0f, TVec3f(0, 0, 0));
    mScaleController = new AnimScaleController(nullptr);
    mScaleController->setParamTight();
    mBindStarPointer = new WalkerStateBindStarPointer(this, mScaleController);

    MR::initShadowController(this, 2);
    MR::addShadowVolumeCylinder(this, "body", 80.0f);
    MR::addShadowVolumeBox(this, "beam", TVec3f(10.0f, 10.0f, 100.0f), MR::getJointMtx(this, "BeamStart"));
    MR::setShadowVolumeStartDropOffset(this, "body", 100.0f);
    MR::invalidateShadow(this, "beam");
    MR::setShadowDropLength(this, "body", 200.0f);
    MR::setShadowDropLength(this, "beam", 200.0f);
    MR::useStageSwitchSleep(this, rIter);
    MR::useStageSwitchReadA(this, rIter);

    mPropellerJointCtrl = MR::createJointController(this, "Propeller", &SearchBeamer::calcJointPropeller);
    _B0 = 40.0f;
    mBeanStartJointCtrl = MR::createJointController(this, "BeamStart", &SearchBeamer::calcJointBeamStart);
    mBeamEndJointCtrl = MR::createJointController(this, "BeamEnd", &SearchBeamer::calcJointBeamEnd);
    initBeamPos();
    MR::getJMapInfoArg0NoInit(rIter, &_138);
    MR::getJMapInfoArg1NoInit(rIter, &_13C);
    _140 = 300.0f + _13C;
    MR::calcFrontVec(&_94, this);
    MR::calcUpVec(&_A0, this);
    mGravity.negate(_A0);
    initNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitFar));
    makeActorAppeared();
}

void SearchBeamer::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::invalidateCollisionParts(this);
        MR::offCalcAnim(this);
    }

    if (MR::isNearPlayerAnyTime(this, 4000.0f)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitFar));
    }
}

void SearchBeamer::exeCloseWaitFar() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
        MR::startBrk(this, "Wait");
    }

    reformDirection(false);

    if (MR::isStep(this, 30)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseSearch));
    } else if (MR::isNearPlayer(this, 2500.0f)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitNear));
    } else {
        if (tryNonActive()) {
            return;
        }
    }
}

void SearchBeamer::exeCloseSearch() {
    reformDirection(false);
    MR::rotateVecDegree(&_94, _A0, 3.0f);

    if (MR::isStep(this, 50)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitFar));
    }
}

void SearchBeamer::exeCloseWaitNear() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Wait", nullptr);
        MR::startBrk(this, "Wait");
    }

    reformDirection(false);
    MR::turnDirectionToPlayerDegree(this, &_94, 3.0f);
    bool isNear = MR::isNearPlayer(this, 3000.0f) == false;

    if (isNear) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitFar));

    } else {
        if (isPlayerInTerritory()) {
            setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvOpenMouth));
        }
    }
}

void SearchBeamer::exeOpenMouth() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Open", nullptr);
        MR::startSound(this, "SE_EM_SEARCHBEAMER_PRE_BEAM");
    }

    bowToPlayer();

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvBeamPrepare));
    }
}

void SearchBeamer::exeBeamPrepare() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Attack", nullptr);
    }

    if (MR::isStep(this, 10)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvBeamStart));
    }
}

void SearchBeamer::exeBeamStart() {
    if (MR::isFirstStep(this)) {
        MR::startBtk(this, "SearchBeamer");
        MR::validateHitSensor(this, "beam");
        MR::validateShadow(this, "beam");
        MR::invalidateClipping(this);
        initBeamPos();
        MR::showMaterial(this, "lambert5_v");
    }

    MR::startLevelSound(this, "SE_EM_LV_SEARCHBEAMER_BEAM");
    bowToPlayer();
    TVec3f v7;
    checkBeamDistiny(&v7, TVec3f(_94));
    TVec3f corePos;
    MR::copyJointPos(this, "Core", &corePos);
    MR::vecBlend(corePos, v7, &mBeamEnd, MR::calcNerveRate(this, 15));
    updateBeamEffect(MR::isGreaterStep(this, 15));
    updateBeamShadow();

    if (MR::isStep(this, 1)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvBeamAim));
    }
}

// TODO -- stack issues
void SearchBeamer::exeBeamAim() {
    MR::startLevelSound(this, "SE_EM_LV_SEARCHBEAMER_BEAM");
    MR::copyJointPos(this, "Core", &mBeamStart);
    TVec3f v14;
    TVec3f up;
    MR::getPlayerUpVec(&up);

    JMAVECScaleAdd(&up, MR::getPlayerCenterPos(), &v14, -20.0f);
    TVec3f v13;
    v13.sub(v14, mPosition);
    MR::normalize(&v13);
    TVec3f v12;
    checkBeamDistiny(&v12, TVec3f(v13));
    TVec3f v11;
    v11.sub(v12, mBeamEnd);
    MR::normalizeOrZero(&v11);
    TVec3f v10;
    JMAVECScaleAdd(&v11, &mBeamEnd, &v10, _138);
    _94.sub(v10, mPosition);
    MR::normalize(&_94);
    TVec3f v9;
    MR::turnVecToPlane(&v9, _94, _A0);
    MR::clampVecAngleDeg(&_94, v9, 35.0f);
    updateBeamEffect(checkBeamDistiny(&mBeamEnd, TVec3f(_94)));
    updateBeamShadow();

    if (!isPlayerInTerritory()) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseMouth));
    }
}

void SearchBeamer::exeCloseMouth() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Close", nullptr);
        MR::deleteEffectAll(this);
        MR::invalidateHitSensor(this, "beam");
        MR::invalidateShadow(this, "beam");
    }

    reformDirection(false);

    if (MR::isLessStep(this, 5)) {
        TVec3f corePos;
        MR::copyJointPos(this, "Core", &corePos);
        MR::vecBlend(corePos, mBeamEnd, &mBeamStart, MR::calcNerveRate(this, 5));
    }

    if (MR::isStep(this, 5)) {
        initBeamPos();
    }

    if (MR::isBckStopped(this)) {
        MR::validateClipping(this);
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitNear));
    }
}

void SearchBeamer::exeStopStart() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Close", nullptr);
        MR::deleteEffectAll(this);
        MR::invalidateHitSensor(this, "beam");
        MR::invalidateShadow(this, "beam");
        initBeamPos();
        MR::validateClipping(this);
    }

    reformDirection(true);

    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStop));
    }
}

void SearchBeamer::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::tryStartBck(this, "Stop", nullptr);
        MR::startBrk(this, "Stop");
    }

    reformDirection(true);
    setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff));
}

void SearchBeamer::exeRecover() {
    if (MR::isStep(this, 60)) {
        MR::startBrk(this, "Wait");
    }

    if (MR::isStep(this, 120)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitNear));
    }
}

void SearchBeamer::exeDPDSwoon() {
    if (MR::isFirstStep(this)) {
        MR::deleteEffectAll(this);
        MR::invalidateHitSensor(this, "beam");
        MR::invalidateShadow(this, "beam");
        initBeamPos();
        MR::validateClipping(this);
    }

    MR::updateActorStateAndNextNerve(this, mBindStarPointer, GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitNear));
}

void SearchBeamer::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::makeMtxFrontUpPos(&mtx, _94, _A0, mPosition);
    MR::setBaseTRMtx(this, mtx);
    TVec3f v7;
    JMathInlineVEC::PSVECMultiply(&mScale, &mScaleController->_C, &v7);
    MR::setBaseScale(this, v7);
    mPropellerJointCtrl->registerCallBack();
    mBeanStartJointCtrl->registerCallBack();
    mBeamEndJointCtrl->registerCallBack();
}

void SearchBeamer::control() {
    mScaleController->updateNerve();
    updatePropeller();

    if (tryDPDSwoon()) {
        return;
    }
}

void SearchBeamer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isSensorEnemyAttack(pSender)) {
            MR::sendMsgEnemyAttackElectric(pReceiver, pSender);
        } else if (MR::isSensorEnemy(pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
    }
}

bool SearchBeamer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return false;
    }

    bool isStopped = false;

    if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopStart)) || isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStop)) ||
        isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff))) {
        isStopped = true;
    }

    if (isStopped) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        MR::deleteEffectAll(this);
        MR::invalidateHitSensor(this, "beam");
        MR::invalidateShadow(this, "beam");
        initBeamPos();
        MR::validateClipping(this);
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvCloseWaitNear));
        return true;
    }

    return false;
}

bool SearchBeamer::receiveOtherMsg(u32 msg, HitSensor*, HitSensor*) {
    if (MR::isMsgFloorTouch(msg)) {
        if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff))) {
            setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff));
            return true;
        } else {
            return tryStopStart();
        }
    }

    return false;
}

void SearchBeamer::updateHitSensor(HitSensor* pSensor) {
    if (!pSensor->mValidByHost) {
        pSensor->mPosition.set< f32 >(mBeamStart);
        return;
    }

    TVec3f v7;
    TVec3f up;
    MR::getPlayerUpVec(&up);
    v7.scaleAdd(up, *MR::getPlayerCenterPos(), -20.0f);
    MR::calcPerpendicFootToLineInside(&pSensor->mPosition, v7, mPosition, mBeamEnd);
}

bool SearchBeamer::calcJointPropeller(TPos3f* pPos, const JointControllerInfo& rInfo) {
    MR::rotateMtxLocalYDegree(*pPos, _AC);
    return true;
}

bool SearchBeamer::calcJointBeamStart(TPos3f* pPos, const JointControllerInfo& rInfo) {
    pPos->setTrans(mBeamStart);
    return true;
}

bool SearchBeamer::calcJointBeamEnd(TPos3f* pPos, const JointControllerInfo& rInfo) {
    pPos->setTrans(mBeamEnd);
    return true;
}

// TODO -- float issues, missing frsp
void SearchBeamer::updatePropeller() {
    bool isStopped = false;

    if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopStart)) || isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStop)) ||
        isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff))) {
        isStopped = true;
    }

    if (isStopped) {
        _B0 -= 0.30f;
    } else {
        _B0 += 0.30f;
    }

    f32 v4 = 10.0f;

    if (_B0 < 10.0f) {
        v4 = v4;
    } else {
        v4 = 40.0f;

        if (_B0 > 40.0f) {
            v4 = v4;
        } else {
            v4 = _B0;
        }
    }

    _B0 = v4;
    _AC += v4;
}

void SearchBeamer::updateBeamEffect(bool a1) {
    if (a1) {
        if (MR::isEffectValid(this, "BeamEndAir")) {
            MR::deleteEffect(this, "BeamEndAir");
        }

        MR::emitEffect(this, "BeamEnd");
    } else {
        if (MR::isEffectValid(this, "BeamEnd")) {
            MR::deleteEffect(this, "BeamEnd");
        }

        MR::emitEffect(this, "BeamEndAir");
    }
}

void SearchBeamer::updateBeamShadow() {
    f32 dist = mPosition.distance(mBeamEnd);
    TVec3f v5;
    v5.z = dist;
    v5.x = 15.0f;
    v5.y = 15.0f;
    MR::setShadowVolumeBoxSize(this, "beam", v5);
    TVec3f v4;
    v4.x = 0.0f;
    v4.y = 0.0f;
    v4.z = dist / 2.0f;
    MR::setShadowDropPositionMtxPtr(this, "beam", MR::getJointMtx(this, "BeamStart"), v4);
}

void SearchBeamer::initBeamPos() {
    MR::copyJointPos(this, "Core", &mBeamStart);
    mBeamEnd.set< f32 >(mBeamStart);
    MR::hideMaterial(this, "lambert5_v");
}

void SearchBeamer::reformDirection(bool a1) {
    if (_94.dot(_A0) != 0.0f) {
        TVec3f v8;
        _A0.rejection(_94, v8);
        MR::normalize(&v8);
        f32 deg = a1 ? 15.0f : 3.0f;
        MR::turnVecToVecDegree(&_94, _94, v8, deg, TVec3f(0, 1, 0));
    }
}

// TODO -- stack issues
void SearchBeamer::bowToPlayer() {
    TVec3f v6;
    MR::getPlayerUpVec(&v6);
    TVec3f v12;
    JMAVECScaleAdd(v6, MR::getPlayerCenterPos(), &v12, -20.0f);
    TVec3f v11;
    v11.sub(v12, mPosition);
    _A0.rejection(_A0, v11);
    MR::normalize(&v11);
    TVec3f v10;
    v10.negate(_A0);
    MR::turnVecToVecDegree(&v11, v11, v10, 35.0f, TVec3f(0, 1, 0));
    MR::turnVecToVecDegree(&_94, _94, v11, 3.0f, TVec3f(0, 1, 0));
    TVec3f v9;
    MR::turnVecToPlane(&v9, _94, _A0);
    MR::clampVecAngleDeg(&_94, v9, 35.0f);
}

bool SearchBeamer::checkBeamDistiny(TVec3f* a1, TVec3f a2) const {
    TVec3f v12;
    v12.scale(_140, a2);

    if (!MR::getFirstPolyOnLineToMap(a1, nullptr, mPosition, v12)) {
        a1->add(v12, mPosition);
        return false;
    }

    return true;
}

bool SearchBeamer::isPlayerInTerritory() const {
    if (MR::isValidSwitchA(this) && !MR::isOnSwitchA(this)) {
        return false;
    }

    bool ret = false;

    if (MR::isNearPlayer(this, _140)) {
        if (MR::isFaceToPlayerHorizontalDegree(this, _94, 45.0f)) {
            ret = true;
        }
    }

    return ret;
}

bool SearchBeamer::tryNonActive() {
    if (MR::isNearPlayerAnyTime(this, 4000.0f)) {
        return false;
    }

    setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvNonActive));
    return true;
}

bool SearchBeamer::tryStopStart() {
    bool isStopped = false;

    if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopStart)) || isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStop)) ||
        isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff))) {
        isStopped = true;
    }

    if (isStopped) {
        return false;
    }

    if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvRecover))) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStop));
    } else {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopStart));
    }

    return true;
}

bool SearchBeamer::tryDPDSwoon() {
    if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvDPDSwoon))) {
        return false;
    }

    bool isStopped = false;

    if (isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopStart)) || isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStop)) ||
        isNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvStopForPlayerOff))) {
        isStopped = true;
    }

    if (isStopped) {
        return false;
    }

    if (!mBindStarPointer->tryStartPointBind()) {
        return false;
    }

    setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvDPDSwoon));
    return true;
}

void SearchBeamer::endNonActive() {
    MR::validateHitSensor(this, "body");
    MR::validateCollisionParts(this);
    MR::onCalcAnim(this);
}

void SearchBeamer::endDPDSwoon() {
    mBindStarPointer->kill();
}

void SearchBeamer::exeStopForPlayerOff() {
    reformDirection(true);
    if (MR::isStep(this, 5)) {
        setNerve(GET_NERVE(SearchBeamer, SearchBeamerNrvRecover));
    }
}

SearchBeamer::~SearchBeamer() {
}
