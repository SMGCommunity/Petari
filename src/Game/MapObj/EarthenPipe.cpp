#include "Game/MapObj/EarthenPipe.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/GameAudio/AudStageBgmTable.hpp"

EarthenPipe::EarthenPipe(const char *pName) : LiveActor(pName) {
    _8C.x = 0.0f;
    _8C.y = 0.0f;
    _8C.z = 0.0f;
    _98.x = 0.0f;
    _98.y = 1.0f;
    _98.z = 0.0f;
    _A4 = 100.0f;
    mIsIgnoreGravity = false;
    mPipeMode = 0;
    _B0 = 0;
    mTopJointMtx = nullptr;
    mBottomJointMtx = nullptr;
    mHostActor = nullptr;
    _180 = 0.0f;
    mHorizExitForce = 5.0f;
    mVertExitForce = 20.0f;
    mMusicChangeIdx = -1;
    mMusicState = -1;
    _194 = 0;
    mPipeStreamModel = nullptr;
    _19C = 0;
    mCameraInfo = nullptr;
    _F0.identity();
    _120.identity();
    _150.identity();
    _BC.identity();
}

void EarthenPipe::init(const JMapInfoIter &rIter) {
    MR::createSceneObj(SceneObj_EarthenPipeMediator);
    MR::getSceneObj<EarthenPipeMediator*>(SceneObj_EarthenPipeMediator)->entry(this, rIter);
    MR::initDefaultPos(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mHorizExitForce);
    MR::getJMapInfoArg1NoInit(rIter, &mVertExitForce);
    MR::getJMapInfoArg2NoInit(rIter, &mPipeMode);
    MR::getJMapInfoArg4NoInit(rIter, &mMusicChangeIdx);
    MR::getJMapInfoArg5NoInit(rIter, &mMusicState);
    MR::getJMapInfoArg6NoInit(rIter, &mIsIgnoreGravity);
    s32 arg7 = 0;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);
    bool isWaterPipe = false;
    
    if (MR::isObjectName(rIter, "EarthenPipeInWater")) {
        isWaterPipe = true;
    }

    _8C.set<f32>(mPosition);
    MR::calcGravity(this);
    initModelManagerWithAnm("EarthenPipe", nullptr, false);
    mTopJointMtx = MR::getJointMtx(this, "Top");
    mBottomJointMtx = MR::getJointMtx(this, "Bottom");
    MR::startBrk(this, "EarthenPipe");
    MR::setBrkFrameAndStop(this, arg7);
    TPos3f v25;
    v25.identity();
    MR::makeMtxTR(v25.toMtxPtr(), this);
    TVec3f v23;
    v23.set(v25.mMtx[0][2], v25.mMtx[1][2], v25.mMtx[2][2]);

    if (mIsIgnoreGravity) {
        f32 z = v25.mMtx[2][1];
        f32 y = v25.mMtx[1][1];
        f32 x = v25.mMtx[0][1];
        _98.set<f32>(x, y, z);
    }
    else {
        _98.set<f32>(mGravity);
        f32 _x = _98.x;
        f32 mult = -1.0f;
        f32 x = _x * mult;
        f32 y = _98.y * mult;
        f32 z = _98.z * mult;
        
        _98.x = x;
        _98.y = y;
        _98.z = z;
    }

    TPos3f v24;
    v24.identity();
    MR::makeMtxUpFrontPos(&v24, _98, v23, _8C);
    PSMTXCopy(v24.toMtxPtr(), mTopJointMtx);
    JMath::gekko_ps_copy12(&_BC, &v24);
    TVec3f v22(_8C);
    TVec3f v21(_98);
    v21.scale(50.0f);
    v22.sub(v21);
    v24.mMtx[0][3] = v22.x;
    v24.mMtx[1][3] = v22.y;
    v24.mMtx[2][3] = v22.z;
    PSMTXCopy(v24.toMtxPtr(), mBottomJointMtx);
    MR::setBaseTRMtx(this, mTopJointMtx);
    MR::connectToSceneMapObjNoCalcAnimStrongLight(this);

    if (isWaterPipe) {
        initHitSensor(3);
    }
    else {
        initHitSensor(1);
    }

    MR::addHitSensorBinder(this, "binder", 8, 70.0f, TVec3f(0.0f, -50.0f, 0.0f));

    if (isWaterPipe) {
        MR::addHitSensorBinder(this, "BinderWaterUpper", 8, 200.0f, TVec3f(0.0f, 180.0f, 0.0f));
        MR::addHitSensorBinder(this, "BinderWaterLower", 8, 80.0f, TVec3f(0.0f, 50.0f, 0.0f));
    }

    if (mPipeMode == 3) {
        MR::initCollisionParts(this, "EarthenPipeS", getSensor("binder"), mTopJointMtx);
    }
    else {
        MR::initCollisionParts(this, "EarthenPipe", getSensor("binder"), mTopJointMtx);
    }

    f32 v14 = (100.0f * mScale.y);
    mScale.y = 1.0f;
    _A4 = v14;
    calcTrans(1.0f);
    initEffectKeeper(0, nullptr, false);
    MR::setEffectHostMtx(this, "LavaAppear", _BC.mMtx);
    MR::setEffectHostMtx(this, "LavaVanish", _BC.mMtx);
    initSound(4, false);
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "出現");
    initNerve(&NrvEarthenPipe::EarthenPipeNrvWait::sInstance);

    if ((mPipeMode - 1) <= 1u) {
        MR::invalidateCollisionParts(this);
        calcTrans(0.0f);
        MR::invalidateHitSensors(this);
        MR::hideModel(this);
        setNerve(&NrvEarthenPipe::EarthenPipeNrvHide::sInstance);
    }

    if (isWaterPipe) {
        mPipeStreamModel = MR::createPartsModelMapObj(this, "土管水流", "EarthenPipeStream", nullptr);
        MR::setClippingTypeSphere(mPipeStreamModel, 500.0f);
        MR::tryStartAllAnim(mPipeStreamModel, "EarthenPipeStream");
        mPipeStreamModel->appear();
        _19C = 1;
    }
    else {
        _19C = 0;
    }

    MR::useStageSwitchReadA(this, rIter);
    MR::useStageSwitchWriteB(this, rIter);
    MR::useStageSwitchSleep(this, rIter);
    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (mPipeStreamModel != nullptr) {
            MR::registerDemoSimpleCastAll(mPipeStreamModel);
        }
    }
    else if (MR::isEqualStageName("HeavensDoorGalaxy")) {
        MR::registerDemoSimpleCastAll(this);
    }

    makeActorAppeared();
}

void EarthenPipe::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    if (isNerve(&NrvEarthenPipe::EarthenPipeNrvHide::sInstance)) {
        MR::invalidateCollisionParts(this);
    }
}

bool EarthenPipe::tryShowUp() {
    MR::invalidateClipping(this);
    MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "出現", -1);
    if (mPipeMode != 1 && mPipeMode != 2) {
        return false;
    }

    setNerve(&NrvEarthenPipe::EarthenPipeNrvWaitToShowUp::sInstance);
    return true;
}

bool EarthenPipe::tryHideDown() {
    MR::validateClipping(this);
    MR::endMultiActorCamera(this, mCameraInfo, "出現", true, -1);

    if (!mPipeMode || (mPipeMode - 3) <= 1u) {
        setNerve(&NrvEarthenPipe::EarthenPipeNrvInvalid::sInstance);
        return false;
    }
    else {
        if (mPipeMode == 2) {
            mPipeMode = 0;
            setNerve(&NrvEarthenPipe::EarthenPipeNrvInvalid::sInstance);
            return false;
        }
        else {
            setNerve(&NrvEarthenPipe::EarthenPipeNrvWaitToHideDown::sInstance);
            return true;
        }     
    }
}

bool EarthenPipe::isNerveShowUp() const {
    bool ret;

    if (isNerve(&NrvEarthenPipe::EarthenPipeNrvShowUp::sInstance) || isNerve(&NrvEarthenPipe::EarthenPipeNrvShowUp::sInstance)) {
        ret = true;
    }
    else {
        ret = false;
    }

    return ret;
}

void EarthenPipe::exeWait() {
    if (MR::isValidSwitchA(this)) {
        if (MR::isOnSwitchA(this)) {
            if (!MR::isValidHitSensor(this, "binder")) {
                MR::validateHitSensors(this);
                if (mPipeStreamModel != nullptr) {
                    mPipeStreamModel->appear();
                    MR::startAction(mPipeStreamModel, "Appear");
                    _19C = 1;
                }
            }
        }
        else if (MR::isValidHitSensor(this, "binder")) {
            MR::invalidateHitSensors(this);
            if (mPipeStreamModel != nullptr) {
                mPipeStreamModel->kill();
            }

            _19C = 0;
        }
    }
}

void EarthenPipe::exeReady() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("EarthenPipeReady", (const char*)0);
    }

    MR::blendMtx(_120.mMtx, _150.mMtx, MR::calcNerveRate(this, MR::getBckFrameMaxPlayer()), _F0.mMtx);
    if (MR::isBckStoppedPlayer()) {
        if (!MR::isPlayerDead()) {
            setNerve(&NrvEarthenPipe::EarthenPipeNrvPlayerIn::sInstance);
        }
    }
}

void EarthenPipe::exePlayerIn() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("EarthenPipeIn", (const char*)0);
        MR::startSystemSE("SE_DM_DOKAN_WARP", -1, -1);
        processBgmPlayerIn();
    }

    if (MR::isBckStopped(mHostActor) && !MR::isPlayerDead()) {
        JMath::gekko_ps_copy12(&_F0, _B0->getBaseMtx());
        if (_B0->tryShowUp()) {
            setNerve(&NrvEarthenPipe::EarthenPipeNrvTargetPipeShowUp::sInstance);
        }
        else {
            setNerve(&NrvEarthenPipe::EarthenPipeNrvPlayerOut::sInstance);
        }
    }
}

void EarthenPipe::exeTargetPipeShowUp() {
    if (MR::isFirstStep(this)) {
        MR::hidePlayer();
    }

    if (!_B0->isNerveShowUp()) {
        JMath::gekko_ps_copy12(&_F0, _B0->getBaseMtx());
        MR::showPlayer();
        setNerve(&NrvEarthenPipe::EarthenPipeNrvPlayerOut::sInstance);
    }
}

void EarthenPipe::exePlayerOut() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("EarthenPipeOut", (const char*)nullptr);
        MR::startSystemSE("SE_DM_DOKAN_WARP", -1, -1);
        processBgmPlayerOut();
    }

    if (MR::isBckStopped(mHostActor)) {
        MtxPtr baseMtx = _B0->getBaseMtx();
        TVec3f v6((baseMtx)[0][2], (baseMtx)[1][2], (baseMtx)[2][2]);
        TVec3f v5((baseMtx)[0][1], (baseMtx)[1][1], (baseMtx)[2][1]);
        v6.scale(_B0->mHorizExitForce);
        v5.scale(_B0->mVertExitForce);
        TVec3f v4(v5);

        if (!MR::isInWater(_B0->mPosition)) {
            v4.add(v6);
        }

        MR::startSound(mHostActor, "SE_PM_JUMP_M", -1, -1);
        MR::startSound(mHostActor, "SE_PV_JUMP_JOY", -1, -1);
        MR::startBckPlayer("EarthenPipeJump", (const char*)nullptr);
        MR::endBindAndPLayerForceWeakGravityJumpInputOff(this, v4);
        mHostActor = nullptr;
        _B0->tryHideDown();

        if (!_194) {
            if (MR::isValidSwitchB(this)) {
                MR::onSwitchB(this);
            }
        }

        setNerve(&NrvEarthenPipe::EarthenPipeNrvPlayerOut::sInstance);
    }
}

inline bool isNear(const LiveActor *actor) {
    return !(MR::isNearPlayer(actor, 100.0f));
}

void EarthenPipe::exeInvalid() {
    bool near;
    if (MR::isGreaterStep(this, 0x1E) && MR::isOnGroundPlayer() 
        || mPipeMode == 4 && isNear(this)
        || MR::isPlayerSwimming() && isNear(this)) {
            MR::validateClipping(this);
            MR::validateHitSensors(this);
            setNerve(&NrvEarthenPipe::EarthenPipeNrvWait::sInstance);
    }
}

void EarthenPipe::exeShowUp() {
    if (MR::isFirstStep(this)) {
        MR::showModel(this);
        MR::validateCollisionParts(this);
        MR::emitEffect(this, "LavaAppear");
        MR::startSystemSE("SE_SY_ITEM_APPEAR", -1, -1);
    }

    f32 easeIn = MR::getEaseInValue(getNerveStep() / 35.0f, 0.0f, 1.0f, 1.0f);
    calcTrans(easeIn);

    if (MR::isLessStep(this, 30)) {
        MR::startLevelSound(this, "SE_OJ_LV_PIPE_APPEAR_MAGMA", -1, -1, -1);
    }

    if (MR::isStep(this, 30)) {
        MR::deleteEffect(this, "LavaAppear");
        setNerve(&NrvEarthenPipe::EarthenPipeNrvShow::sInstance);
    }
}

void EarthenPipe::exeHideDown() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "LavaVanish");
    }

    f32 easeIn = MR::getEaseInValue((15 - getNerveStep()) / 15.0f, 0.0f, 1.0f, 1.0f);
    calcTrans(easeIn);
    if (MR::isLessStep(this, 15)) {
        MR::startLevelSound(this, "SE_OJ_LV_PIPE_VANISH_MAGMA", -1, -1, -1);
    }

    if (MR::isStep(this, 15)) {
        MR::hideModel(this);
        MR::invalidateCollisionParts(this);
        MR::validateClipping(this);
        MR::deleteEffect(this, "LavaVanish");
        setNerve(&NrvEarthenPipe::EarthenPipeNrvHide::sInstance);
    }
}

void EarthenPipe::control() {
    MR::updateMaterial(this);
    if (_19C) {
        MR::startLevelSound(this, "SE_OJ_LV_PIPE_IN_WATER", -1, -1, -1);
    }
}

// bool EarthenPipe::receiveOtherMsg(u32 msg, HitSensor *, HitSensor *) { }

void EarthenPipe::calcTrans(f32 a1) {
    mPosition.set<f32>(_98);
    mPosition.scale(a1 * _A4);
    mPosition.add(_8C);

    (mTopJointMtx)[0][3] = mPosition.x;
    (mTopJointMtx)[1][3] = mPosition.y;
    (mTopJointMtx)[2][3] = mPosition.z;
    calcAndSetBaseMtx();
    if (MR::isValidCollisionParts(this)) {
        MR::setCollisionMtx(this);
    }
}

void EarthenPipe::processBgmPlayerIn() {
    s32 idx = _B0->mMusicChangeIdx;
    if (idx >= 0) {
        u32 bgmId = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), idx);
        if (bgmId != -1 ) {
            s32 cur = AudWrap::getBgmMgr()->_10;
            if (cur != bgmId) {
                MR::stopStageBGM(60);
                return;
            }

            s32 state = _B0->mMusicState;
            if (state >= 0) {
                s32 bgmState = AudStageBgmTable::getBgmState(MR::getCurrentStageName(), state);
                if (bgmState >= 0) {
                    AudBgm* bgm = AudWrap::getStageBgm();
                    if (bgm != nullptr) {
                        bgm->changeTrackMuteState(bgmState, 240);
                    }
                }
            }
        }
    }
}

void EarthenPipe::processBgmPlayerOut() {
    s32 idx = _B0->mMusicChangeIdx;
    if (idx >= 0) {
        u32 bgmId = AudStageBgmTable::getBgmId(MR::getCurrentStageName(), idx);
        if (bgmId != -1) {
            s32 cur = AudWrap::getBgmMgr()->_10;
            if (cur == bgmId && MR::isPlayingStageBgm()) {
                return;
            }

            AudWrap::startStageBgm(bgmId, false);
            s32 state = _B0->mMusicState;
            if (state >= 0) {
                s32 bgmState = AudStageBgmTable::getBgmState(MR::getCurrentStageName(), state);
                if (bgmState >= 0) {
                    AudBgm* bgm = AudWrap::getStageBgm();
                    if (bgm != nullptr) {
                        bgm->changeTrackMuteState(bgmState, 0);
                    }
                } 
            }
        }    
    }
}

EarthenPipeMediator::EarthenPipeMediator() : NameObj("土管仲介者") {
    mNumEntries = 0;
    mPipeEntries = nullptr;
    mPipeEntries = new Entry[0x20];

    for (s32 i = 0; i < 32; i++) {
        mPipeEntries[i]._0 = nullptr;
        mPipeEntries[i]._4 = nullptr;
        mPipeEntries[i].mPipeID = -1;
    }
}

void EarthenPipeMediator::entry(EarthenPipe *pPipe, const JMapInfoIter &rIter) {
    s32 pipeID = -1;
    MR::getJMapInfoArg3NoInit(rIter, &pipeID);

    for (s32 i = 0; i < mNumEntries; i++) {
        EarthenPipeMediator::Entry* entry = &mPipeEntries[i];
        
        if (pipeID == entry->mPipeID) {
            entry->_4 = pPipe;
            entry->_0->_B0 = pPipe;
            entry->_4->_B0 = entry->_0;
            return;
        }
    }

    EarthenPipeMediator::Entry* entry = &mPipeEntries[mNumEntries];
    entry->_0 = pPipe;
    entry->mPipeID = pipeID;
    mNumEntries++;
}

EarthenPipe::~EarthenPipe() {

}

EarthenPipeMediator::~EarthenPipeMediator() {

}

namespace NrvEarthenPipe {
    INIT_NERVE(EarthenPipeNrvHideDown);
    INIT_NERVE(EarthenPipeNrvWaitToHideDown);
    INIT_NERVE(EarthenPipeNrvShowUp);
    INIT_NERVE(EarthenPipeNrvWaitToShowUp);
    INIT_NERVE(EarthenPipeNrvShow);
    INIT_NERVE(EarthenPipeNrvHide);
    INIT_NERVE(EarthenPipeNrvInvalid);
    INIT_NERVE(EarthenPipeNrvPlayerOut);
    INIT_NERVE(EarthenPipeNrvTargetPipeShowUp);
    INIT_NERVE(EarthenPipeNrvPlayerIn);
    INIT_NERVE(EarthenPipeNrvReady);
    INIT_NERVE(EarthenPipeNrvWait);
};


void EarthenPipe::exeWaitToHideDown() {
    if (MR::isStep(this, 0x28)) {
        setNerve(&NrvEarthenPipe::EarthenPipeNrvHideDown::sInstance);
    }
}

void EarthenPipe::exeWaitToShowUp() {
    if (MR::isStep(this, 0x14)) {
        setNerve(&NrvEarthenPipe::EarthenPipeNrvShowUp::sInstance);
    }
}

MtxPtr EarthenPipe::getBaseMtx() const {
    return mTopJointMtx;
}

void EarthenPipe::calcAnim() {

}
