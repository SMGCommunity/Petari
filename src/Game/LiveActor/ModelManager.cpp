#include "Game/Animation/BckCtrl.hpp"
#include "Game/Animation/BpkPlayer.hpp"
#include "Game/Animation/BrkPlayer.hpp"
#include "Game/Animation/BtkPlayer.hpp"
#include "Game/Animation/BtpPlayer.hpp"
#include "Game/Animation/BvaPlayer.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/Animation/XanimeResource.hpp"
#include "Game/LiveActor/DisplayListMaker.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/MutexHolder.hpp"
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>

ModelManager::ModelManager()
    : mBtkPlayer(nullptr),
      mBrkPlayer(nullptr),
      mBtpPlayer(nullptr),
      mBpkPlayer(nullptr),
      mBvaPlayer(nullptr),
      mXanimeResourceTable(nullptr),
      mXanimePlayer(nullptr),
      mModel(nullptr),
      mModelResourceHolder(nullptr),
      mDisplayListMaker(nullptr) {
}

void ModelManager::update() {
    XanimePlayer* pXanimePlayer = mXanimePlayer;

    if (pXanimePlayer != nullptr) {
        pXanimePlayer->updateBeforeMovement();
        pXanimePlayer->updateAfterMovement();
    }

    if (mBtkPlayer != nullptr) {
        mBtkPlayer->update();
    }

    if (mBrkPlayer != nullptr) {
        mBrkPlayer->update();
    }

    if (mBtpPlayer != nullptr) {
        mBtpPlayer->update();
    }

    if (mBpkPlayer != nullptr) {
        mBpkPlayer->update();
    }

    if (mBvaPlayer != nullptr) {
        mBvaPlayer->update();
    }
}

void ModelManager::calcAnim() {
    calc();
    updateDL(true);
}

void ModelManager::calcView() {
    getJ3DModel()->viewCalc();
}

void ModelManager::entry() {
    getJ3DModel()->entry();
}

void ModelManager::newDifferedDLBuffer() {
    mDisplayListMaker->newDifferedDisplayList();
}

void ModelManager::updateDL(bool isDiffDL) {
    OSLockMutex(&MR::MutexHolder< 0 >::sMutex);

    bool isUpdateDL = mDisplayListMaker != nullptr && mDisplayListMaker->isValidDiff();

    if (isUpdateDL) {
        mDisplayListMaker->update();

        if (mBtkPlayer != nullptr) {
            mBtkPlayer->beginDiff();
        }

        if (mBrkPlayer != nullptr) {
            mBrkPlayer->beginDiff();
        }

        if (mBtpPlayer != nullptr) {
            mBtpPlayer->beginDiff();
        }

        if (mBpkPlayer != nullptr) {
            mBpkPlayer->beginDiff();
        }
    }

    getJ3DModel()->calcMaterial();

    if (isUpdateDL) {
        if (isDiffDL) {
            mDisplayListMaker->diff();
        }

        if (mBtkPlayer != nullptr) {
            mBtkPlayer->endDiff();
        }

        if (mBrkPlayer != nullptr) {
            mBrkPlayer->endDiff();
        }

        if (mBtpPlayer != nullptr) {
            mBtpPlayer->endDiff();
        }

        if (mBpkPlayer != nullptr) {
            mBpkPlayer->endDiff();
        }
    }

    OSUnlockMutex(&MR::MutexHolder< 0 >::sMutex);
}

void ModelManager::startBck(const char* pBckName, const char* pActorAnimName) {
    mXanimePlayer->changeAnimationBck(pBckName);
    mXanimePlayer->changeInterpoleFrame(0);
    changeBckSetting(pBckName, pActorAnimName, nullptr);
}

void ModelManager::startBckWithInterpole(const char* pBckName, s32 interpole) {
    mXanimePlayer->changeAnimationBck(pBckName);
    mXanimePlayer->changeInterpoleFrame(0);
    mXanimePlayer->changeInterpoleFrame(interpole);
}

void ModelManager::startBtk(const char* pBtkName) {
    stopBtk();
    mBtkPlayer->start(pBtkName);
    mDisplayListMaker->onCurFlagBtk(mBtkPlayer->mAnmRes);
}

void ModelManager::startBrk(const char* pBrkName) {
    stopBrk();
    mBrkPlayer->start(pBrkName);
    mDisplayListMaker->onCurFlagBrk(mBrkPlayer->mAnmRes);
}

void ModelManager::startBtp(const char* pBtpName) {
    stopBtp();
    mBtpPlayer->start(pBtpName);
    mDisplayListMaker->onCurFlagBtp(mBtpPlayer->mAnmRes);
}

void ModelManager::startBpk(const char* pBpkName) {
    stopBpk();
    mBpkPlayer->start(pBpkName);
    mDisplayListMaker->onCurFlagBpk(mBpkPlayer->mAnmRes);
}

void ModelManager::startBva(const char* pBvaName) {
    stopBva();
    mBvaPlayer->start(pBvaName);
}

void ModelManager::stopBtk() {
    if (mBtkPlayer->mAnmRes != nullptr) {
        mDisplayListMaker->offCurFlagBtk(mBtkPlayer->mAnmRes);
        mBtkPlayer->stop();
    }
}

void ModelManager::stopBrk() {
    if (mBrkPlayer->mAnmRes != nullptr) {
        mDisplayListMaker->offCurFlagBrk(mBrkPlayer->mAnmRes);
        mBrkPlayer->stop();
    }
}

void ModelManager::stopBtp() {
    if (mBtpPlayer->mAnmRes != nullptr) {
        mDisplayListMaker->offCurFlagBtp(mBtpPlayer->mAnmRes);
        mBtpPlayer->stop();
    }
}

void ModelManager::stopBpk() {
    if (mBpkPlayer->mAnmRes != nullptr) {
        mDisplayListMaker->offCurFlagBpk(mBpkPlayer->mAnmRes);
        mBpkPlayer->stop();
    }
}

void ModelManager::stopBva() {
    if (mBvaPlayer->mAnmRes != nullptr) {
        mBvaPlayer->stop();
    }
}

// ModelManager::getBckCtrl

J3DFrameCtrl* ModelManager::getBtkCtrl() const {
    return &mBtkPlayer->mFrameCtrl;
}

J3DFrameCtrl* ModelManager::getBrkCtrl() const {
    return &mBrkPlayer->mFrameCtrl;
}

J3DFrameCtrl* ModelManager::getBtpCtrl() const {
    return &mBtpPlayer->mFrameCtrl;
}

J3DFrameCtrl* ModelManager::getBpkCtrl() const {
    return &mBpkPlayer->mFrameCtrl;
}

J3DFrameCtrl* ModelManager::getBvaCtrl() const {
    return &mBvaPlayer->mFrameCtrl;
}

// ModelManager::isBckStopped

bool ModelManager::isBtkStopped() const {
    if (mBtkPlayer != nullptr) {
        return mBtkPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBrkStopped() const {
    if (mBrkPlayer != nullptr) {
        return mBrkPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBtpStopped() const {
    if (mBtpPlayer != nullptr) {
        return mBtpPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBpkStopped() const {
    if (mBpkPlayer != nullptr) {
        return mBpkPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBvaStopped() const {
    if (mBvaPlayer != nullptr) {
        return mBvaPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBtkPlaying(const char* pBtkName) const {
    return mBtkPlayer->isPlaying(pBtkName);
}

bool ModelManager::isBrkPlaying(const char* pBrkName) const {
    return mBrkPlayer->isPlaying(pBrkName);
}

bool ModelManager::isBpkPlaying(const char* pBpkName) const {
    return mBpkPlayer->isPlaying(pBpkName);
}

bool ModelManager::isBtpPlaying(const char* pBtpName) const {
    return mBtpPlayer->isPlaying(pBtpName);
}

bool ModelManager::isBvaPlaying(const char* pBvaName) const {
    return mBvaPlayer->isPlaying(pBvaName);
}

void ModelManager::initJointTransform() {
    mXanimePlayer->mCore->enableJointTransform(getJ3DModelData());
}

// ModelManager::getJointTransform

/*
ResourceHolder* ModelManager::getResourceHolder() const {
    if (mXanimeResourceTable == nullptr) {
        return mModelResourceHolder;
    }

    return mXanimeResourceTable->_6C;
}
*/

ResourceHolder* ModelManager::getModelResourceHolder() const {
    return mModelResourceHolder;
}

J3DModel* ModelManager::getJ3DModel() const {
    if (mXanimePlayer == nullptr) {
        return mModel;
    }

    return mXanimePlayer->mModel;
}

J3DModelData* ModelManager::getJ3DModelData() const {
    return getJ3DModel()->mModelData;
}

const char* ModelManager::getPlayingBckName() const {
    if (mXanimePlayer != nullptr) {
        return mXanimePlayer->getCurrentBckName();
    }

    return nullptr;
}

// ModelManager::initModelAndAnimation

void ModelManager::initMaterialAnm() {
    ResourceHolder* pResourceHolder = getResourceHolder();
    J3DModelData*   pModelData = getJ3DModelData();

    if (pResourceHolder->mMaterialBuf == nullptr) {
        pResourceHolder->newMaterialAnmBuffer(pModelData);
    }

    if (pResourceHolder->mBtkResTable->mCount != 0) {
        mBtkPlayer = new BtkPlayer(pResourceHolder, pModelData);
    }

    if (pResourceHolder->mBrkResTable->mCount != 0) {
        mBrkPlayer = new BrkPlayer(pResourceHolder, pModelData);
    }

    if (pResourceHolder->mBtpResTable->mCount != 0) {
        mBtpPlayer = new BtpPlayer(pResourceHolder, pModelData);
    }

    if (pResourceHolder->mBpkResTable->mCount != 0) {
        mBpkPlayer = new BpkPlayer(pResourceHolder, pModelData);
    }
}

void ModelManager::initVisibilityAnm() {
    ResourceHolder* pResourceHolder = getResourceHolder();

    if (pResourceHolder->mBvaResTable->mCount != 0) {
        mBvaPlayer = new BvaPlayer(pResourceHolder->mBvaResTable, getJ3DModel());
    }
}

void ModelManager::calc() {
    if (mXanimePlayer != nullptr) {
        mXanimePlayer->calcAnm(0);
    }

    if (mBvaPlayer != nullptr) {
        mBvaPlayer->calc();
    }

    OSLockMutex(&MR::MutexHolder< 0 >::sMutex);
    getJ3DModel()->calc();
    OSUnlockMutex(&MR::MutexHolder< 0 >::sMutex);

    if (mXanimePlayer != nullptr) {
        mXanimePlayer->clearAnm(0);
    }
}

void ModelManager::changeBckSetting(const char* pBckName, const char* pActorAnimName, XanimePlayer* pXanimePlayer) {
    if (pXanimePlayer == nullptr) {
        pXanimePlayer = mXanimePlayer;
    }

    BckCtrl* pBckCtrl = getResourceHolder()->mBckCtrl;

    if (pBckCtrl != nullptr) {
        if (pActorAnimName == nullptr) {
            pActorAnimName = pBckName;
        }

        pBckCtrl->changeBckSetting(pActorAnimName, pXanimePlayer);
    }
}

// ModelManager::init
