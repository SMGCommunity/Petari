#include "Game/LiveActor/ActorAnimKeeper.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    const char* sFileName = "ActorAnimCtrl";
}

ActorAnimKeeper::ActorAnimKeeper(LiveActor* pActor) : mActor(pActor), mNumInfo(0), mInfoArray(nullptr), mCurrentInfo(0) {
    if (!initAnimData()) {
        return;
    }
    start("デフォルト");
}

ActorAnimKeeper* ActorAnimKeeper::tryCreate(LiveActor* pActor) {
    const ResourceHolder* pResourceHolder = MR::getResourceHolder(pActor);

    if (!MR::isExistFileInArc(pResourceHolder, "%s.bcsv", ::sFileName)) {
        return nullptr;
    }

    if (pResourceHolder->mMotionResTable->mCount == 0 && pResourceHolder->mBtkResTable->mCount == 0 && pResourceHolder->mBrkResTable->mCount == 0 &&
        pResourceHolder->mBpkResTable->mCount == 0 && pResourceHolder->mBtpResTable->mCount == 0 && pResourceHolder->mBvaResTable->mCount == 0) {
        return nullptr;
    }

    return new ActorAnimKeeper(pActor);
}

namespace {
    const char* getAnimName(const ActorAnimKeeperInfo* pKeeperInfo, const ActorAnimDataInfo& rDataInfo) {
        const char* name = rDataInfo.mName;
        if (name == nullptr) {
            name = pKeeperInfo->mName;
        }
        return name;
    };
}  // namespace

bool ActorAnimKeeper::start(const char* pName) {
    ActorAnimKeeperInfo* info = findAnimInfo(pName);
    mCurrentInfo = info;
    if (mCurrentInfo == nullptr) {
        return false;
    }

    const char* BckName = ::getAnimName(info, info->mBckInfo);
    const char* BtkName = ::getAnimName(info, info->mBtkInfo);
    const char* BrkName = ::getAnimName(info, info->mBrkInfo);
    const char* BpkName = ::getAnimName(info, info->mBpkInfo);
    const char* BtpName = ::getAnimName(info, info->mBtpInfo);
    const char* BvaName = ::getAnimName(info, info->mBvaInfo);

    if (MR::isBckExist(mActor, BckName) && (!info->mBckInfo.mIsKeepAnim || !MR::isBckPlaying(mActor, BckName))) {
        if (MR::isEqualStringCase(BckName, info->mName)) {
            MR::startBck(mActor, BckName, nullptr);
        } else {
            MR::startBck(mActor, BckName, info->mName);
        }
    }

    if (MR::isBtkExist(mActor, BtkName) && (!info->mBtkInfo.mIsKeepAnim || !MR::isBtkPlaying(mActor, BtkName))) {
        MR::startBtk(mActor, BtkName);
    }

    if (MR::isBrkExist(mActor, BrkName) && (!info->mBrkInfo.mIsKeepAnim || !MR::isBrkPlaying(mActor, BrkName))) {
        MR::startBrk(mActor, BrkName);
    }

    if (MR::isBpkExist(mActor, BpkName) && (!info->mBpkInfo.mIsKeepAnim || !MR::isBpkPlaying(mActor, BpkName))) {
        MR::startBpk(mActor, BpkName);
    }

    if (MR::isBtpExist(mActor, BtpName) && (!info->mBtpInfo.mIsKeepAnim || !MR::isBtpPlaying(mActor, BtpName))) {
        MR::startBtp(mActor, BtpName);
    }

    if (MR::isBvaExist(mActor, BvaName) && (!info->mBvaInfo.mIsKeepAnim || !MR::isBvaPlaying(mActor, BvaName))) {
        MR::startBva(mActor, BvaName);
    }

    return true;
}

void ActorAnimKeeper::update() {}

bool ActorAnimKeeper::isPlaying(const char* pName) const {
    ActorAnimKeeperInfo* info = findAnimInfo(pName);

    if (info != nullptr) {
        return MR::isBckPlaying(mActor, getAnimName(info, info->mBckInfo));
    }

    return MR::isBckPlaying(mActor, pName);
}

namespace {
    void getCsvDataAnimData(ActorAnimDataInfo* pDataInfo, const char* pAnimType, JMapInfo* pInfo, s32 index) {
        char pOut[0x80];
        snprintf(pOut, sizeof(pOut), "%sName", pAnimType);
        MR::getCsvDataStrOrNULL(&pDataInfo->mName, pInfo, pOut, index);

        snprintf(pOut, sizeof(pOut), "%sStartFrame", pAnimType);
        MR::getCsvDataF32(&pDataInfo->mStartFrame, pInfo, pOut, index);

        snprintf(pOut, sizeof(pOut), "%sIsKeepAnim", pAnimType);
        MR::getCsvDataU8(&pDataInfo->mIsKeepAnim, pInfo, pOut, index);
    }
}  // namespace

bool ActorAnimKeeper::initAnimData() {
    const ResourceHolder* pResourceHolder = MR::getResourceHolder(mActor);
    JMapInfo* csvParser = MR::tryCreateCsvParser(pResourceHolder, "%s.bcsv", ::sFileName);

    if (csvParser == nullptr) {
        return false;
    }

    s32 numInfo = 0;
    if (csvParser != nullptr) {
        numInfo = MR::getCsvDataElementNum(csvParser);
    }
    mNumInfo = numInfo;

    ActorAnimKeeperInfo* infoArray = new ActorAnimKeeperInfo[mNumInfo];
    mInfoArray = infoArray;

    for (int i = 0; i < mNumInfo; i++) {
        ActorAnimKeeperInfo* currentInfo = &infoArray[i];
        MR::getCsvDataStr(&currentInfo->mName, csvParser, "ActorAnimName", i);
        ::getCsvDataAnimData(&currentInfo->mBckInfo, "Bck", csvParser, i);
        ::getCsvDataAnimData(&currentInfo->mBtkInfo, "Btk", csvParser, i);
        ::getCsvDataAnimData(&currentInfo->mBrkInfo, "Brk", csvParser, i);
        ::getCsvDataAnimData(&currentInfo->mBpkInfo, "Bpk", csvParser, i);
        ::getCsvDataAnimData(&currentInfo->mBtpInfo, "Btp", csvParser, i);
        ::getCsvDataAnimData(&currentInfo->mBvaInfo, "Bva", csvParser, i);
    }
    return true;
}

ActorAnimKeeperInfo* ActorAnimKeeper::findAnimInfo(const char* pName) const {
    ActorAnimKeeperInfo* currentInfo;
    for (int i = 0; i < mNumInfo; i++) {
        currentInfo = &mInfoArray[i];
        if (MR::isEqualStringCase(currentInfo->mName, pName)) {
            return currentInfo;
        }
    }
    return nullptr;
}
