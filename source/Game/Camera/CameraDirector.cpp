#include "Game/Boss/BossStinkBug.h"
#include "Game/Camera/CameraLocalUtil.h"
#include "Game/Camera/CameraCover.h"
#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraMan.h"
#include "Game/Camera/CameraManGame.h"
#include "Game/Camera/CameraManEvent.h"
#include "Game/Camera/CameraManPause.h"
#include "Game/Camera/CameraManSubjective.h"
#include "Game/Camera/CameraParamChunk.h"
#include "Game/Camera/CameraParamChunkHolder.h"
#include "Game/Camera/CameraPoseParam.h"
#include "Game/Camera/CameraRailHolder.h"
#include "Game/Camera/CameraRegisterHolder.h"
#include "Game/Camera/CameraRotChecker.h"
#include "Game/Camera/CameraTargetHolder.h"
#include "Game/Camera/CameraShaker.h"
#include "Game/Camera/CameraTargetMtx.h"
#include "Game/Camera/CameraViewInterpolator.h"
#include "Game/Camera/GameCameraCreator.h"
#include "Game/Camera/OnlyCamera.h"
#include "Game/LiveActor/ActorCameraInfo.h"
#include "Game/Util/CameraUtil.h"
#include "Game/Util/ObjUtil.h"
#include "Game/Util/PlayerUtil.h"
#include "Game/Util/ScreenUtil.h"
#include "Game/Util/SequenceUtil.h"
#include <string.h>

u32 sMovementCounter;
const char *sSubjectiveCameraName = "主観カメラ";
const char *sStartCameraName = "スタートアニメカメラ";
const char *sTalkCameraName = "共通会話カメラ";

void CameraPoseParam::copyFrom(const CameraPoseParam &rOther) {
    _0.set(rOther._0); 
    _C.set(rOther._C);
    _18.set(rOther._18);
    _24.set(rOther._24);
    _30 = rOther._30; 
    _34.set(rOther._34); 
    _40.set(rOther._40);
    _4C = rOther._4C;
    _50 = rOther._50; 
    _54 = rOther._54;
}

char *CameraParamChunkID_Tmp::getBuffer(unsigned long size) {
    return &mBuffer[0];
}

bool CameraMan::isInterpolationOff() const {
    return false;
}

bool CameraMan::isCollisionOff() const {
    return false;
}

bool CameraMan::isZeroFrameMoveOff() const {
    return false;
}

bool CameraMan::isSubjectiveCameraOff() const {
    return false;
}

bool CameraMan::isCorrectingErpPositionOff() const {
    return false;
}

bool CameraMan::isEnableToReset() const {
    return false;
}

bool CameraMan::isEnableToRoundLeft() const {
    return false;
}

bool CameraMan::isEnableToRoundRight() const {
    return false;
}

void CameraMan::roundLeft() {

}

void CameraMan::roundRight() {

}

CameraDirector::CameraDirector(const char *pName) : NameObj(pName) {
    _C = 0;
    mStack = new CameraManStack();
    mOnlyCamera = new OnlyCamera("OnlyCamera");
    mPoseParam1 = new CameraPoseParam();
    mPoseParam2 = new CameraPoseParam();
    mHolder = new CameraHolder("カメラホルダー");
    mChunkHolder = new CameraParamChunkHolder(mHolder, "パラメータ");
    mCameraCreator = new GameCameraCreator(mChunkHolder);
    mRailHolder = new CameraRailHolder("カメラレール管理");
    mRegisterHolder = new CameraRegisterHolder("カメラレジスタ");
    mTargetHolder = new CameraTargetHolder();
    mShaker = new CameraShaker("振動");
    mViewInterpolator = new CameraViewInterpolator();
    mCover = new CameraCover("CameraCover");
    mRotChecker = new CameraRotChecker();
    mCameraManGame = new CameraManGame(mHolder, mChunkHolder, "ゲームカメラマン");
    mCameraManEvent = new CameraManEvent(mHolder, mChunkHolder, "イベントカメラマン");
    mCameraManPause = new CameraManPause("ポーズカメラマン");
    mCameraManSubjective = new CameraManSubjective("主観カメラマン");
    _58 = false;
    _16C = 0;
    _170 = true;
    _174 = 0;
    _178 = false;
    mTargetMatrix = new CameraTargetMtx("カメラターゲットダミー");
    mRequestCameraManReset = false;
    _1B1 = false;
    mIsSubjectiveCamera = false;
    _1B3 = false;
    _1B4 = 0;
    _1BC = -100.0f;
    _1F0 = false;
    _1F1 = true;
    _1F2 = false;

    MR::connectToSceneCamera(this);
    push(mCameraManGame);
    _180.identity();
    JMath::gekko_ps_copy12(&mTargetMatrix->_10, &_180);
    setInterpolation(0);
    mCameraManSubjective->owned(this);
    _1C0.identity();
    MR::createCenterScreenBlur();
}

CameraDirector::~CameraDirector() {

}

void CameraDirector::init(const JMapInfoIter &rIter) {
    
}

void CameraDirector::movement() {
    sMovementCounter++;
    backLastMtx();
    mTargetHolder->movement();
    updateCameraMan();
    calcPose();
    createViewMtx();
    TPos3f *invView = MR::getCameraInvViewMtx();
    JMath::gekko_ps_copy12(&getCurrentCameraMan()->mMatrix, invView);
    mPoseParam2->copyFrom(*getCurrentCameraMan()->mPoseParam);
    calcSubjective();
    mShaker->movement();
    checkStartCondition();
    checkEndOfEventCamera();
    mRotChecker->update();
    mRequestCameraManReset = false;
    _1B1 = false;
}

void CameraDirector::setTarget(CameraTargetObj *pTarget) {
    mTargetHolder->set(pTarget);
}

CameraTargetObj *CameraDirector::getTarget() {
    return mTargetHolder->get();
}

void CameraDirector::push(CameraMan *pMan) {
    if (mStack->mCount != 0) {
        CameraMan *man = getCurrentCameraMan();
        man->deactivate(this);
    }

    mStack->mElements[mStack->mCount++] = pMan;

    pMan->owned(this);
    pMan->activate(this);
}

CameraMan *CameraDirector::pop() {
    CameraMan *man = mStack->mElements[--mStack->mCount];

    man->deactivate(this);
    man->released(this);

    if (mStack->mCount != 0) {
        CameraMan *newMan = getCurrentCameraMan();
        newMan->activate(this);
    }

    return man;
}

void CameraDirector::backLastMtx() {
    if (_1F0) {
        MR::setCameraViewMtx(_1C0, false, false, TVec3f(0.0f, 0.0f, 0.0f));
        TPos3f *invView = MR::getCameraInvViewMtx();

        TVec3f invTrans;
        invView->getTrans(invTrans);
    }
}

CameraMan *CameraDirector::getCurrentCameraMan() const {
    return mStack->mElements[mStack->mCount - 1];
}

void CameraDirector::updateCameraMan() {
    if (mRequestCameraManReset) {
        resetCameraMan();
    }

    if (_1B1) {
        CameraMan *man = getCurrentCameraMan();
        man->_15 = true;
    }

    getCurrentCameraMan()->movement();
    controlCameraSE();
}

void CameraDirector::calcPose() {
    switchAntiOscillation();

    if (getCurrentCameraMan()->isCollisionOff()) {
        mViewInterpolator->_7C = true;
    }

    if (getCurrentCameraMan()->isCorrectingErpPositionOff()) {
        mViewInterpolator->_8A = false;
    }

    if (getCurrentCameraMan()->isZeroFrameMoveOff()) {
        mOnlyCamera->_3C = true;
    }

    mOnlyCamera->calcPose(getCurrentCameraMan());
    mPoseParam1->copyFrom(*mOnlyCamera->mPoseParam);
}

/*void CameraDirector::calcSubjective() {
    
}*/

bool CameraDirector::isInterpolationOff() {
    bool off = false;

    CameraMan *man = getCurrentCameraMan();

    if (man->isInterpolationOff() || mViewInterpolator->_9) {
        off = true;
    }

    return off;
}

void CameraDirector::switchAntiOscillation() {
    if (isInterpolationOff()) {
        mViewInterpolator->_8 = false;
    }
    else {
        mViewInterpolator->_8 = true;
    }
}

void CameraDirector::createViewMtx() {
    TPos3f view;
    calcViewMtxFromPoseParam(&view, mPoseParam1);

    CameraPoseParam *poseParam = mPoseParam1;
    CameraTargetObj *target = _C;
    TVec3f &vec = poseParam->_C;

    CameraMan *man = getCurrentCameraMan();
    f32 fovy = CameraLocalUtil::getFovy(man);
    mViewInterpolator->updateCameraMtx(reinterpret_cast<MtxPtr>(&view), vec, target, fovy);
}

void CameraDirector::checkStartCondition() {
    if (_170 && getCurrentCameraMan() == mCameraManGame && _174++ > 30 && mTargetHolder->isMoving()) {
        _170 = false;
        mCameraManGame->endStartPosCamera();
    }
}

void CameraDirector::startEvent(long zoneID, const char * pName, const CameraTargetArg &rTargetArg, long a4) {
    mViewInterpolator->_A = false;
    removeEndEventAtLanding(zoneID, pName);

    if (getCurrentCameraMan() != mCameraManEvent) {
        if (mStack->mCount != 0) {
            CameraMan *gameMan = mCameraManGame;

            if (getCurrentCameraMan() == gameMan) {
                mCameraManEvent->mPoseParam->copyFrom(*gameMan->mPoseParam);
                TPos3f *invView = MR::getCameraInvViewMtx();
                JMath::gekko_ps_copy12(&mCameraManEvent->mMatrix, invView);
            }
        }

        push(mCameraManEvent);
    }

    mCameraManEvent->start(zoneID, pName, rTargetArg, a4);
}

void CameraDirector::endEvent(long zoneID, const char *pName, bool a3, long a4) {
    if (getCurrentCameraMan() == mCameraManEvent) {
        mCameraManEvent->end(zoneID, pName, a4);

        if (!mCameraManEvent->isActive()) {
            pop();

            if (!mViewInterpolator->_9 && a3 && getCurrentCameraMan() == mCameraManGame) {
                mCameraManGame->mPoseParam->copyFrom(*mPoseParam1);
                TPos3f *invView = MR::getCameraInvViewMtx();
                JMath::gekko_ps_copy12(&mCameraManGame->mMatrix, invView);
            }

            mViewInterpolator->_A = true;
        }
    }
}

/*void CameraDirector::endEventAtLanding(long a1, const char *pName, long a3) {
    if (getCurrentCameraMan() == mCameraManEvent) {
        _5C[_16C][0] = a1;
        strcpy(reinterpret_cast<char *>(_5C[_16C][1]), pName);
        _5C[_16C][0x21] = a3;
        _16C++;
    }
}*/

CameraParamChunkEvent *CameraDirector::getEventParameter(long zoneID, const char *pName) {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createEventID(zoneID, pName);

    return reinterpret_cast<CameraParamChunkEvent *>(mChunkHolder->getChunk(chunkID));
}

void CameraDirector::requestToResetCameraMan() {
    mRequestCameraManReset = true;
}

void CameraDirector::setInterpolation(unsigned long a1) {
    mViewInterpolator->setInterpolation(a1);

    if (a1 == 0 && !_170) {
        mViewInterpolator->_7C = true;
        mCover->cover(2);
    }
}

void CameraDirector::cover(unsigned long a1) {
    mCover->cover(a1);
}

void CameraDirector::closeCreatingCameraChunk() {
    mCameraCreator->scanArea();
    mCameraCreator->scanStartPos();
    createStartAnimCamera();
    createTalkCamera();
    createSubjectiveCamera();
    mCameraManGame->closeCreatingCameraChunk();
    mChunkHolder->loadCameraParameters();
    mChunkHolder->sort();
    _170 = true;
    mCameraManGame->startStartPosCamera(false);
}

void CameraDirector::initCameraCodeCollection(const char *a1, long a2) {
    mCameraCreator->initCameraCodeCollection(a1, a2);
}

void CameraDirector::registerCameraCode(unsigned long code) {
    mCameraCreator->registerCameraCode(code);
}

void CameraDirector::termCameraCodeCollection() {
    mCameraCreator->termCameraCodeCollection();
}

void CameraDirector::declareEvent(long zoneID, const char *pName) {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createEventID(zoneID, pName);

    mChunkHolder->createChunk(chunkID, NULL);
}

void CameraDirector::started() {
    _170 = false;
    mCameraManGame->endStartPosCamera();
}

void CameraDirector::setTargetActor(const LiveActor *pActor) {
    mTargetHolder->set(pActor);
}

void CameraDirector::setTargetPlayer(const MarioActor *pActor) {
    mTargetHolder->set(pActor);
}

bool CameraDirector::isRotatingHard() const {
    return mRotChecker->_30;
}

bool CameraDirector::isSubjectiveCamera() const {
    return mIsSubjectiveCamera;
}

bool CameraDirector::isEnableToControl() const {
    bool enable = false;

    if (!getCurrentCameraMan()->isSubjectiveCameraOff()) {
        bool change = true;
        bool equals = getCurrentCameraMan() == mCameraManEvent;

        if (equals && !isEventCameraActive(0, sSubjectiveCameraName)) {
            change = false;
        }

        if (change) {
            enable = true;
        }
    }

    return enable;
}

bool CameraDirector::isEnableToRoundLeft() const {
    return getCurrentCameraMan()->isEnableToRoundLeft();
}

bool CameraDirector::isEnableToRoundRight() const {
    return getCurrentCameraMan()->isEnableToRoundRight();
}

bool CameraDirector::isEnableToReset() const {
    return getCurrentCameraMan()->isEnableToReset();
}

bool CameraDirector::isEventCameraActive(long zoneID, const char *pName) const {
    if (getCurrentCameraMan() == mCameraManEvent) {
        return mCameraManEvent->isEventActive(zoneID, pName);
    }

    return false;
}

bool CameraDirector::isEventCameraActive() const {
    return getCurrentCameraMan() == mCameraManEvent;
}

void CameraDirector::startStartPosCamera(bool a1) {
    _170 = true;
    mCameraManGame->startStartPosCamera(a1);
}

bool CameraDirector::isInterpolatingNearlyEnd() const {
    if (getCurrentCameraMan() == mCameraManEvent && mCameraManEvent->doesNextChunkHaveInterpolation()) {
        return false;
    }

    return mViewInterpolator->isInterpolatingNearlyEnd();
}

bool CameraDirector::isForceCameraChange() const {
    return mViewInterpolator->_9;
}

f32 CameraDirector::getDefaultFovy() const {
    return 45.0f;
}

#ifdef NON_MATCHING
// No traces of a constructor for targetArg in the original code. Probably the
// CameraTargetArg(CameraTargetMatrix *) constructor has been inlined
void CameraDirector::startStartAnimCamera() {
    if (_178) {
        ActorCameraInfo info = ActorCameraInfo(-1, 0);
        CameraTargetArg targetArg = CameraTargetArg(mTargetMatrix);

        MR::startEventCamera(&info, sStartCameraName, targetArg, 0);
    }
}
#endif

bool CameraDirector::isStartAnimCameraEnd() const {
    if (_178) {
        return isAnimCameraEnd(0, sStartCameraName);
    }

    return true;
}

u32 CameraDirector::getStartAnimCameraFrame() const {
    if (_178) {
        return getAnimCameraFrame(0, sStartCameraName);
    }

    return 0;
}

void CameraDirector::endStartAnimCamera() {
    ActorCameraInfo info = ActorCameraInfo(-1, 0);
    MR::endEventCamera(&info, sStartCameraName, true, 0);
}

#ifdef NON_MATCHING
// No traces of a constructor for targetArg in the original code. Probably the
// CameraTargetArg() constructor has been inlined
void CameraDirector::startTalkCamera(const TVec3f &rPosition, const TVec3f &rUp, float axisX, float axisY, long a5) {
    CameraParamChunkID_Tmp chunkID = CameraParamChunkID_Tmp();
    chunkID.createEventID(0, sTalkCameraName);

    CameraParamChunk *chunk = mChunkHolder->getChunk(chunkID);

    if (chunk != NULL) {
        chunk->mGeneralParam->mWPoint.set(rPosition);
        chunk->mGeneralParam->mUp.set(rUp);
        chunk->mGeneralParam->mAxis.x = axisX;
        chunk->mGeneralParam->mAxis.y = axisY;
        chunk->mGeneralParam->mAxis.z = 0.0f;

        CameraTargetArg targetArg = CameraTargetArg();

        MR::setCameraTargetToPlayer(&targetArg);
        startEvent(0, sTalkCameraName, targetArg, a5);
    }
}
#endif

void CameraDirector::endTalkCamera(bool a1, long a2) {
    endEvent(0, sTalkCameraName, a1, a2);
}

void CameraDirector::startSubjectiveCamera(long a1) {    
    _170 = false;
    mCameraManGame->endStartPosCamera();
    mIsSubjectiveCamera = true;

    if (!_1B3) {
        _1B3 = true;
        _1B4 = 0;

        mCameraManSubjective->activate(this);
        f32 nearZ = MR::getNearZ();
        _1BC = nearZ;

        MR::setNearZ(10.0f);
        MR::turnOffDOFInSubjective();
    }

    if (_1B4 < 20) {
        MR::startCenterScreenBlur(20, 15.0f, 80, 5, 10);
    }
}

void CameraDirector::endSubjectiveCamera(long a1) {
    bool bVar1 = a1 == 0 || a1 == 1;

    if (mIsSubjectiveCamera == true) {
        mIsSubjectiveCamera = false;

        if (!bVar1) {
            MR::startCenterScreenBlur(_1B4, 15.0f, 80, 5, 10);
        }
    }

    if (_1B3 && bVar1) {
        _1B4 = 0;
    }
}

bool CameraDirector::isAnimCameraEnd(long zoneID, const char *pName) const {
    CameraManEvent *eventMan = mCameraManEvent;
    CameraMan *man = getCurrentCameraMan();

    if (man == eventMan) {
        return eventMan->isAnimCameraEnd(zoneID, pName);
    }

    return true;
}

u32 CameraDirector::getAnimCameraFrame(long zoneID, const char *pName) const {
    CameraManEvent *eventMan = mCameraManEvent;
    CameraMan *man = getCurrentCameraMan();

    if (man == eventMan) {
        return eventMan->getAnimCameraFrame(zoneID, pName);
    }

    return 0;
}

void CameraDirector::pauseOnAnimCamera(long zoneID, const char *pName) {
    CameraManEvent *eventMan = mCameraManEvent;
    CameraMan *man = getCurrentCameraMan();

    if (man == eventMan) {
        eventMan->pauseOnAnimCamera(zoneID, pName);
    }
}

void CameraDirector::pauseOffAnimCamera(long zoneID, const char *pName) {
    CameraManEvent *eventMan = mCameraManEvent;
    CameraMan *man = getCurrentCameraMan();

    if (man == eventMan) {
        eventMan->pauseOffAnimCamera(zoneID, pName);
    }
}

void CameraDirector::zoomInGameCamera() {
    mCameraManGame->zoomIn();
}

void CameraDirector::zoomOutGameCamera() {
    mCameraManGame->zoomOut();
}

/*void CameraDirector::checkEndOfEventCamera() {
    if (_16C != 0 && mTargetHolder->isOnGround()) {
        for (u32 i = 0; i < _16C; i++) {
            endEvent(_5C[i][0], reinterpret_cast<const char *>(_5C[i][4]), true, _5C[i][33]);
        }

        _16C = 0;
    }
}*/

/*void CameraDirector::controlCameraSE() {

}*/

/*void CameraDirector::removeEndEventAtLanding(long, const char *) {

}*/

/*void CameraDirector::calcViewMtxFromPoseParam(TPos3f *, const CameraPoseParam *) {

}*/

bool CameraDirector::isPlayableCameraSE(bool a1) {
    if (MR::isDemoActive()) {
        return false;
    }

    if (MR::isPowerStarGetDemoActive()) {
        return false;
    }

    if (MR::isStageStateScenarioOpeningCamera()) {
        return false;
    }

    if (MR::isExecScenarioStarter()) {
        return false;
    }

    if (MR::isPlayerDead()) {
        return false;
    }

    if (MR::isEqualStageName("FileSelect")) {
        return false;
    }

    if (MR::isEqualStageName("EpilogueDemoStage")) {
        return false;
    }

    if (a1 && mIsSubjectiveCamera) {
        return false;
    }

    return true;
}

void CameraDirector::resetCameraMan() {
    setInterpolation(0);
}
