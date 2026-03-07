#include "Game/System/SaveDataBannerCreator.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game\Util\NerveUtil.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/nand.h"
#include "revolution/types.h"
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <JSystem/JKernel/JKRHeap.hpp>

namespace {
    NEW_NERVE(SaveDataBannerCreatorNoOperation, SaveDataBannerCreator, NoOperation);
    NEW_NERVE(SaveDataBannerCreatorCreateOnTemporary, SaveDataBannerCreator, CreateOnTemporary);
    NEW_NERVE(SaveDataBannerCreatorMoveToHomeDir, SaveDataBannerCreator, MoveToHomeDir);
};  

SaveDataBannerCreator::SaveDataBannerCreator() : NerveExecutor("BannerCreator") {
    mNANDRequestInfo = nullptr;
    mBanner = nullptr;
    mNANDRequestInfo = new NANDRequestInfo();
    initNerve(&SaveDataBannerCreatorNoOperation::sInstance);
    setupBannerInfo();
}

void SaveDataBannerCreator::execute() {
    setNerve(&SaveDataBannerCreatorCreateOnTemporary::sInstance);
}

bool SaveDataBannerCreator::isDone() const {
    return isNerve(&SaveDataBannerCreatorNoOperation::sInstance);
}

NANDResultCode SaveDataBannerCreator::getResultCode() const {
    return mNANDRequestInfo->mResult;
}

void SaveDataBannerCreator::exeCreateOnTemporary() {
    if (MR::isFirstStep(this)) {
        mNANDRequestInfo->setWriteSeq("/tmp/banner.bin", mBanner, 29344, 60, 0);
        MR::addRequestToNANDManager(mNANDRequestInfo);
    }

    if (mNANDRequestInfo->isDone()) {
        NANDResultCode resultCode = mNANDRequestInfo->mResult;
        if (resultCode.isSuccess()) {
            setNerve(&SaveDataBannerCreatorMoveToHomeDir::sInstance);
        }
        else {
            setNerve(&SaveDataBannerCreatorNoOperation::sInstance);
        }
    }
}

void SaveDataBannerCreator::exeMoveToHomeDir() {
    if (MR::isFirstStep(this)) {
        mNANDRequestInfo->setMove("/tmp/banner.bin", mHomeDir);
        MR::addRequestToNANDManager(mNANDRequestInfo);
    }

    if (mNANDRequestInfo->isDone()) {
        NANDResultCode resultCode = mNANDRequestInfo->mResult;
        if (resultCode.isSuccess()) {
            setNerve(&SaveDataBannerCreatorNoOperation::sInstance);
        }
        else {
            setNerve(&SaveDataBannerCreatorNoOperation::sInstance);
            return;
        }
    }
}

void SaveDataBannerCreator::setupBannerInfo() {
    /*
    mBanner = new (MR::getStationedHeapGDDR3(), 32) NANDBanner();
    JUTTexture dd = MR::loadTexFromArc("SaveIconBanner.arc", "SaveBanner.bti");
    MR::copyMemory(mBanner->bannerTexture, dd.mImage, 6000);
    char buf[30];
    snprintf(buf, sizeof(buf), "SaveIcon%02d.bti", 0);
    JUTTexture e = MR::loadTexFromArc("SaveIconBanner.arc", buf);
    MR::copyMemory(mBanner->reserved, e.mImage, 120);
    mBanner->iconSpeed = mBanner->iconSpeed & 0xFFFC | 3;
    mBanner->iconSpeed &= 0xFFF3u;
    NANDGetHomeDir(mHomeDir);
    */
}

void SaveDataBannerCreator::exeNoOperation() {}
