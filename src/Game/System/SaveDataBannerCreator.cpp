#include "Game/System/SaveDataBannerCreator.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NANDManager.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <cstdio>

namespace {
    NEW_NERVE(SaveDataBannerCreatorNoOperation, SaveDataBannerCreator, NoOperation);
    NEW_NERVE(SaveDataBannerCreatorCreateOnTemporary, SaveDataBannerCreator, CreateOnTemporary);
    NEW_NERVE(SaveDataBannerCreatorMoveToHomeDir, SaveDataBannerCreator, MoveToHomeDir);
};  // namespace

SaveDataBannerCreator::SaveDataBannerCreator() : NerveExecutor("BannerCreator"), mNANDRequestInfo(), mBanner() {
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

void SaveDataBannerCreator::exeNoOperation() {
}

void SaveDataBannerCreator::exeCreateOnTemporary() {
    if (MR::isFirstStep(this)) {
        mNANDRequestInfo->setWriteSeq("/tmp/banner.bin", mBanner, NAND_BANNER_SIZE(1),
                                      NAND_PERM_RGRP | NAND_PERM_WGRP | NAND_PERM_RUSR | NAND_PERM_WUSR, 0);
        MR::addRequestToNANDManager(mNANDRequestInfo);
    }

    if (!mNANDRequestInfo->isDone()) {
        return;
    }

    NANDResultCode resultCode = mNANDRequestInfo->mResult;

    if (resultCode.isSuccess()) {
        setNerve(&SaveDataBannerCreatorMoveToHomeDir::sInstance);
    } else {
        setNerve(&SaveDataBannerCreatorNoOperation::sInstance);
    }
}

void SaveDataBannerCreator::exeMoveToHomeDir() {
    if (MR::isFirstStep(this)) {
        mNANDRequestInfo->setMove("/tmp/banner.bin", mHomeDir);
        MR::addRequestToNANDManager(mNANDRequestInfo);
    }

    if (!mNANDRequestInfo->isDone()) {
        return;
    }

    NANDResultCode resultCode = mNANDRequestInfo->mResult;

    if (resultCode.isSuccess()) {
        setNerve(&SaveDataBannerCreatorNoOperation::sInstance);
    } else {
        setNerve(&SaveDataBannerCreatorNoOperation::sInstance);
        return;
    }
}

void SaveDataBannerCreator::setupBannerInfo() {
    mBanner = new (MR::getStationedHeapGDDR3(), 32) NANDBanner;

    const u16* title = reinterpret_cast< const u16* >(MR::getSystemMessageDirect("TITLE_MAIN"));
    const u16* comment = reinterpret_cast< const u16* >(MR::getSystemMessageDirect("TITLE_SUB"));
    NANDInitBanner(mBanner, NAND_BANNER_FLAG_ANIM_LOOP, title, comment);

    const ResTIMG* bannerTex = MR::loadTexFromArc("SaveIconBanner.arc", "SaveBanner.bti");
    MR::copyMemory(mBanner->bannerTexture, reinterpret_cast< const u8* >(bannerTex) + bannerTex->mImageDataOffset, NAND_BANNER_TEXTURE_SIZE);

    char iconTexName[32];
    snprintf(iconTexName, sizeof(iconTexName), "SaveIcon%02d.bti", 0);

    const ResTIMG* iconTex = MR::loadTexFromArc("SaveIconBanner.arc", iconTexName);
    MR::copyMemory(mBanner->iconTexture, reinterpret_cast< const u8* >(iconTex) + iconTex->mImageDataOffset, NAND_BANNER_ICON_SIZE);

    NANDSetIconSpeed(mBanner, 0, NAND_BANNER_ICON_ANIM_SPEED_NORMAL);
    NANDSetIconSpeed(mBanner, 1, NAND_BANNER_ICON_ANIM_SPEED_END);
    NANDGetHomeDir(mHomeDir);
}
