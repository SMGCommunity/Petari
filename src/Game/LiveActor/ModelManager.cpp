#include "Game/Animation/BpkPlayer.hpp"
#include "Game/Animation/BrkPlayer.hpp"
#include "Game/Animation/BtkPlayer.hpp"
#include "Game/Animation/BtpPlayer.hpp"
#include "Game/Animation/BvaPlayer.hpp"
#include "Game/Animation/XanimePlayer.hpp"
#include "Game/LiveActor/DisplayListMaker.hpp"
#include "Game/LiveActor/ModelManager.hpp"

void ModelManager::update() {
    XanimePlayer* player = mXanimePlayer;

    if (player != nullptr) {
        player->updateBeforeMovement();
        player->updateAfterMovement();
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

// ModelManager::updateDL

void ModelManager::startBck(const char *pAnimName, const char *pUnk) {
    mXanimePlayer->changeAnimationBck(pAnimName);
    mXanimePlayer->changeInterpoleFrame(0);
    changeBckSetting(pAnimName, pUnk, 0);
}

void ModelManager::startBckWithInterpole(const char *pAnimName, s32 interpole) {
    mXanimePlayer->changeAnimationBck(pAnimName);
    mXanimePlayer->changeInterpoleFrame(0);
    mXanimePlayer->changeInterpoleFrame(interpole);
}

void ModelManager::startBtk(const char *pBtkName) {
    stopBtk();
    mBtkPlayer->start(pBtkName);
    mDisplayListMaker->onCurFlagBtk(mBtkPlayer->mAnmRes);
}

void ModelManager::startBrk(const char *pBrkName) {
    stopBrk();
    mBrkPlayer->start(pBrkName);
    mDisplayListMaker->onCurFlagBrk(mBrkPlayer->mAnmRes);
}

void ModelManager::startBtp(const char *pBtpName) {
    stopBtp();
    mBtpPlayer->start(pBtpName);
    mDisplayListMaker->onCurFlagBtp(mBtpPlayer->mAnmRes);
}

void ModelManager::startBpk(const char *pBpkName) {
    stopBpk();
    mBpkPlayer->start(pBpkName);
    mDisplayListMaker->onCurFlagBpk(mBpkPlayer->mAnmRes);
}

void ModelManager::startBva(const char *pBvaName) {
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
    if (mBvaPlayer->mAnmRes) {
        mBvaPlayer->stop();
    }
}

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

bool ModelManager::isBtkPlaying(const char *pName) const {
    return mBtkPlayer->isPlaying(pName);
}

bool ModelManager::isBrkPlaying(const char *pName) const {
    return mBrkPlayer->isPlaying(pName);
}

bool ModelManager::isBpkPlaying(const char *pName) const {
    return mBpkPlayer->isPlaying(pName);
}

bool ModelManager::isBtpPlaying(const char *pName) const {
    return mBtpPlayer->isPlaying(pName);
}

bool ModelManager::isBvaPlaying(const char *pName) const {
    return mBvaPlayer->isPlaying(pName);
}

void ModelManager::initJointTransform() {
    J3DModel* model = getJ3DModel();
    mXanimePlayer->mCore->enableJointTransform(model->mModelData);
}

ResourceHolder* ModelManager::getModelResourceHolder() const {
    return mModelResourceHolder;
}

J3DModelData* ModelManager::getJ3DModelData() const {
    return getJ3DModel()->mModelData;
}
