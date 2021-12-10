#include "Game/LiveActor/ModelManager.h"
#include "Game/LiveActor/DisplayListMaker.h"
#include "Game/Animation/XanimePlayer.h"

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
