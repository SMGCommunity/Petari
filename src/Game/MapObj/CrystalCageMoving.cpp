#include "Game/MapObj/CrystalCageMoving.hpp"
#include "JSystem/JMath/JMath.hpp"

CrystalCageMoving::CrystalCageMoving(const char *pName) : MapObjActor(pName) {
    mTicoModel = nullptr;
    mCameraInfo = nullptr;
    _FC.x = 0.0f;
    _FC.y = 0.0f;
    _FC.z = 0.0f;
    _108 = 0;
    _C8.identity();
}

void CrystalCageMoving::init(const JMapInfoIter &rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo info;
    info.setupHioNode("地形オブジェ");
    info.setupDefaultPos();
    info.setupConnectToScene();
    info.setupEffect(0);
    info.setupSound(4);
    info.setupNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvWaitBig::sInstance);
    info.setupRailMover();
    info.setupHitSensorCallBack();

    TVec3f vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;
    info.setupHitSensorParam(4, 350.0f, vec);
    initialize(rIter, info);
    _FC.set<f32>(mPosition);
    initDummyModel(rIter);
    MR::initActorCamera(this, rIter, &mCameraInfo);
    MR::startBck(this, "Wait", nullptr);

    if (MR::isDemoExist("脱出スピドラ出現")) {
        MR::registerDemoActionNerve(this, &NrvCrystalCageMoving::CrystalCageMovingNrvDemoTicoMove::sInstance, nullptr);
    }
}

void CrystalCageMoving::exeBreakBig() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "CoreWait", nullptr);
        MR::startActorCameraNoTarget(this, mCameraInfo, -1);
        MR::setSensorRadius(this, "body", 30.0f);
        _108 = 1;
        MR::emitEffect(this, "BreakOutside");
        MR::startSound(this, "SE_OJ_CRY_CAGE_MV_BREAK_EDG", -1, -1);
    }

    if (MR::isStep(this, 10)) {
        setNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvWaitSmall::sInstance);
    }
}

void CrystalCageMoving::exeBreakSmall() {
    if (MR::isFirstStep(this)) {
        startBreakDemo();
        MR::emitEffect(this, "BreakInside");
        MR::startSound(this, "SE_OJ_CRY_CAGE_MV_BREAK_OUT", -1, -1);
    }
}

void CrystalCageMoving::exeBreakAll() {
    if (MR::isFirstStep(this)) {
        startBreakDemo();
        MR::emitEffect(this, "BreakOutside");
        MR::emitEffect(this, "BreakInside");
        MR::startSound(this, "SE_OJ_CRY_CAGE_MV_BREAK_ALL", -1, -1);
    }
}

/*
void CrystalCageMoving::exeDemoTicoMove() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mTicoModel, "Fly", nullptr);
        MR::startSound(mTicoModel, "SE_SM_TICO_SPIN", -1, -1);
    }

    TVec3f stack_14;
    stack_14.subInline2(_FC, mPosition);
    f32 nerveRate = MR::calcNerveRate(this, 0x1E);
    TVec3f stack_8;
    JMAVECScaleAdd(&stack_14, &mPosition, &stack_8, nerveRate);
    _C8.setTrans(stack_8);

    if (MR::isStep(this, 0x1E)) {
        setNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvDemoTicoStop::sInstance);
    }
}
*/

void CrystalCageMoving::exeDemoTicoStop() {
    if (MR::isFirstStep(this)) {
        _C8.set(getBaseMtx());
        _C8.setTrans(_FC);
        MR::setBckRate(mTicoModel, 1.5f);
    }

    if (MR::isStep(this, 30)) {
        setNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvDemoTicoChange::sInstance);
    }
}

void CrystalCageMoving::exeDemoTicoChange() {
    if (MR::isFirstStep(this)) {
        MR::startBck(mTicoModel, "Metamorphosis", nullptr);
        MR::startSound(mTicoModel, "SE_SM_TICO_METAMORPHOSE", -1, -1);
        MR::setBckRate(mTicoModel, 1.5f);
    }

    if (MR::isBckStopped(mTicoModel)) {
        kill();
    }
}

void CrystalCageMoving::kill() {
    if (mTicoModel) {
        mTicoModel->kill();
    }

    MapObjActor::kill();
}

void CrystalCageMoving::control() {
    if (!isNerveTypeEnd()) {
        MapObjActor::control();
        _C8.set(MR::getJointMtx(this, 0));
        TVec3f trans;
        PSMTXMultVec(_C8.toMtxPtr(), &sDummyModelOffset, &trans);
        _C8.setTrans(trans);
    }
}

bool CrystalCageMoving::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (isNerveTypeEnd()) {
        return false;
    }

    if (msg == 69) {
        crashMario(a2, a3);
        return true;
    }

    return false;
}

void CrystalCageMoving::crashMario(HitSensor *a1, HitSensor *a2) {
    MR::tryRumblePadVeryStrong(this, 0);
    MR::shakeCameraVeryStrong();

    if (_108) {
        setNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvBreakSmall::sInstance);
    }
    else {
        f32 sensorDist = PSVECDistance(&a2->mPosition, &a1->mPosition);
        f32 sensorObjDist = PSVECDistance(&mPosition, &a1->mPosition);

        if (sensorDist < 30.0f && sensorObjDist < 450.0f) {
            setNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvBreakAll::sInstance);
        }
        else {
            setNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvBreakBig::sInstance);
        }
    }
}

void CrystalCageMoving::updateHitSensor(HitSensor *pSensor) {
    if (!_108) {
        pSensor->mPosition.set<f32>(mPosition);
    }
    else {
        f32 radius = pSensor->mRadius;
        TMtx34f joint_mtx;
        joint_mtx.set(MR::getJointMtx(this, nullptr));
        TVec3f joint_pos;
        f32 z = joint_mtx.mMtx[2][1];
        f32 y = joint_mtx.mMtx[1][1];
        f32 x = joint_mtx.mMtx[0][1];
        joint_pos.set(x, y, z);
        TVec3f stack_14;
        JMAVECScaleAdd(&joint_pos, &mPosition, &stack_14, (-450.0f + radius));
        TVec3f stack_8;
        JMAVECScaleAdd(&joint_pos, &mPosition, &stack_8, (450.0f - radius));
        MR::calcPerpendicFootToLineInside(&pSensor->mPosition, *MR::getPlayerPos(), stack_14, stack_8);
    }
} 

void CrystalCageMoving::connectToScene(const MapObjActorInitInfo &rInfo) {
    if (rInfo.mConnectToScene) {
        MR::connectToSceneCrystal(this);
    }
}

void CrystalCageMoving::initDummyModel(const JMapInfoIter &rIter) {
    _C8.set(MR::getJointMtx(this, 0));
    TVec3f stack_8;
    PSMTXMultVec(_C8.toMtxPtr(), &sDummyModelOffset, &stack_8);
    _C8.setTrans(stack_8);
    mTicoModel = new ModelObj("動くクリスタルケージ中身", "Tico", _C8.toMtxPtr(), 0x21, -2, -2, false);
    mTicoModel->initWithoutIter();
    ModelObj* tico = mTicoModel;
    tico->mScale.x = 3.0f;
    tico->mScale.y = 3.0f;
    tico->mScale.z = 3.0f;
    MR::startBrk(mTicoModel, "ColorChange");
    MR::setBrkFrameAndStop(mTicoModel, 0.0f);

    if (MR::isDemoCast(this, nullptr)) {
        MR::tryRegisterDemoCast(mTicoModel, rIter);
    }
}

void CrystalCageMoving::startBreakDemo() {
    MR::startSound(this, "SE_OJ_CRY_CAGE_MV_TICO_APR", -1, -1);
    MR::requestStartTimeKeepDemo(this, "脱出スピドラ出現", nullptr, nullptr, "クリスタル破壊");
    MR::hideModel(this);
    ModelObj* tico = mTicoModel;
    tico->mScale.x = 1.0f;
    tico->mScale.y = 1.0f;
    tico->mScale.z = 1.0f;
}

bool CrystalCageMoving::isNerveTypeEnd() const {
    bool ret = false;

    if (isNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvBreakBig::sInstance) || 
    isNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvBreakSmall::sInstance) || 
    isNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvBreakAll::sInstance) ||
    isNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvDemoTicoMove::sInstance) ||
    isNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvDemoTicoStop::sInstance) ||
    isNerve(&NrvCrystalCageMoving::CrystalCageMovingNrvDemoTicoChange::sInstance)) {
        ret = true;
    }

    return ret;
}

CrystalCageMoving::~CrystalCageMoving() {

}

namespace NrvCrystalCageMoving {
    INIT_NERVE(CrystalCageMovingNrvWaitBig);
    INIT_NERVE(CrystalCageMovingNrvBreakBig);
    INIT_NERVE(CrystalCageMovingNrvWaitSmall);
    INIT_NERVE(CrystalCageMovingNrvBreakSmall);
    INIT_NERVE(CrystalCageMovingNrvBreakAll);
    INIT_NERVE(CrystalCageMovingNrvDemoTicoMove);
    INIT_NERVE(CrystalCageMovingNrvDemoTicoStop);
    INIT_NERVE(CrystalCageMovingNrvDemoTicoChange);

    void CrystalCageMovingNrvDemoTicoChange::execute(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        cage->exeDemoTicoChange();
    }

    void CrystalCageMovingNrvDemoTicoStop::execute(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        cage->exeDemoTicoStop();
    }

    void CrystalCageMovingNrvDemoTicoMove::execute(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        cage->exeDemoTicoMove();
    }

    void CrystalCageMovingNrvBreakAll::execute(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        cage->exeBreakAll();
    }

    void CrystalCageMovingNrvBreakSmall::execute(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        cage->exeBreakSmall();
    }

    void CrystalCageMovingNrvWaitSmall::execute(Spine *) const {

    }

    void CrystalCageMovingNrvBreakBig::executeOnEnd(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        MR::endActorCamera(cage, cage->mCameraInfo, true, -1);
    }

    void CrystalCageMovingNrvBreakBig::execute(Spine *pSpine) const {
        CrystalCageMoving* cage = reinterpret_cast<CrystalCageMoving*>(pSpine->mExecutor);
        cage->exeBreakBig();
    }

    void CrystalCageMovingNrvWaitBig::execute(Spine *) const {

    }
};
