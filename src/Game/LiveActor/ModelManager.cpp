#include "Game/LiveActor/ModelManager.hpp"
#include "Game/LiveActor/DisplayListMaker.hpp"
#include "Game/Animation/XanimePlayer.hpp"

void ModelManager::update() {
    XanimePlayer* player = mXanimePlayer;

    if (player) {
        player->updateBeforeMovement();
        player->updateAfterMovement();
    }

    if (mBtkPlayer) {
        mBtkPlayer->update();
    }

    if (mBrkPlayer) {
        mBrkPlayer->update();
    }

    if (mBtpPlayer) {
        mBtpPlayer->update();
    }

    if (mBpkPlayer) {
        mBpkPlayer->update();
    }

    if (mBvaPlayer) {
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
    mDisplayListMaker->onCurFlagBtk(reinterpret_cast<const J3DAnmBase*>(mBtkPlayer->mData));
}

void ModelManager::startBrk(const char *pBrkName) {
    stopBrk();
    mBrkPlayer->start(pBrkName);
    mDisplayListMaker->onCurFlagBrk(reinterpret_cast<const J3DAnmBase*>(mBrkPlayer->mData));
}

void ModelManager::startBtp(const char *pBtpName) {
    stopBtp();
    mBtpPlayer->start(pBtpName);
    mDisplayListMaker->onCurFlagBtp(reinterpret_cast<const J3DAnmBase*>(mBtpPlayer->mData));
}

void ModelManager::startBpk(const char *pBpkName) {
    stopBpk();
    mBpkPlayer->start(pBpkName);
    mDisplayListMaker->onCurFlagBpk(reinterpret_cast<const J3DAnmBase*>(mBpkPlayer->mData));
}

void ModelManager::startBva(const char *pBvaName) {
    stopBva();
    mBvaPlayer->start(pBvaName);
}

void ModelManager::stopBtk() {
    if (mBtkPlayer->mData) {
        mDisplayListMaker->offCurFlagBtk((const J3DAnmBase*)mBtkPlayer->mData);
        mBtkPlayer->stop();
    }
}

void ModelManager::stopBrk() {
    if (mBrkPlayer->mData) {
        mDisplayListMaker->offCurFlagBrk((const J3DAnmBase*)mBrkPlayer->mData);
        mBrkPlayer->stop();
    }
}

void ModelManager::stopBtp() {
    if (mBtpPlayer->mData) {
        mDisplayListMaker->offCurFlagBtp((const J3DAnmBase*)mBtpPlayer->mData);
        mBtpPlayer->stop();
    }
}

void ModelManager::stopBpk() {
    if (mBpkPlayer->mData) {
        mDisplayListMaker->offCurFlagBpk((const J3DAnmBase*)mBpkPlayer->mData);
        mBpkPlayer->stop();
    }
}

void ModelManager::stopBva() {
    if (mBvaPlayer->mData) {
        mBvaPlayer->stop();
    }
}

bool ModelManager::isBtkStopped() const {
    if (mBtkPlayer) {
        return mBtkPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBrkStopped() const {
    if (mBrkPlayer) {
        return mBrkPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBtpStopped() const {
    if (mBtpPlayer) {
        return mBtpPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBpkStopped() const {
    if (mBpkPlayer) {
        return mBpkPlayer->isStop();
    }

    return false;
}

bool ModelManager::isBvaStopped() const {
    if (mBvaPlayer) {
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