#include "Game/Camera/CameraCover.h"
#include "Game/Camera/CameraDirector.h"
#include "Game/Camera/CameraHolder.h"
#include "Game/Camera/CameraMan.h"
#include "Game/Camera/CameraManGame.h"
#include "Game/Camera/CameraManEvent.h"
#include "Game/Camera/CameraManPause.h"
#include "Game/Camera/CameraManSubjective.h"
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

    // Some unknown allocation here, probably inlined constructor (_10)

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
    _58 = 0;
    _16C = 0;
    _170 = 1;
    _174 = 0;
    _178 = 0;
    mTargetMatrix = new CameraTargetMtx("カメラターゲットダミー");
    _1B0 = 0;
    _1B1 = 0;
    _1B2 = 0;
    _1B3 = 0;
    _1B4 = 0;
    _1BC = -100.0f;
    _1F0 = 0;
    _1F1 = 1;
    _1F2 = 0;

    // MR::connectToSceneCamera();
    // push();
    _180.identity();
    // JMATH::gekko_ps_copy12();
    // setInterpolation(0);
    // owned();
    _1C0.identity();
    // MR::createCenterScreenBlur();
}

CameraDirector::~CameraDirector() {

}

void CameraDirector::init(const JMapInfoIter &rIter) {
    
}
