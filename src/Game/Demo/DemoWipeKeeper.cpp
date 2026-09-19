#include "Game/Demo/DemoWipeKeeper.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Screen/SceneWipeHolder.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

DemoWipeInfo::DemoWipeInfo() : mPartName(), mWipeName("フェードワイプ"), mWipeType(), mWipeFrame(-1) {
}

DemoWipeKeeper::DemoWipeKeeper(DemoExecutor* pExecutor) : DemoSheetKeeperBase(pExecutor) {
    JMapInfo* pMap = nullptr;
    const DemoExecutor* pSheetExecutor = mExecutor;
    s32 count = DemoFunction::createSheetParser(pSheetExecutor, getTypeString(), &pMap);
    mInfo.init(count);

    for (s32 i = 0; i < count; i++) {
        DemoWipeInfo info;
        MR::getCsvDataStrOrNULL(&info.mPartName, pMap, "PartName", i);
        MR::getCsvDataStrOrNULL(&info.mWipeName, pMap, "WipeName", i);
        MR::getCsvDataS32(&info.mWipeType, pMap, "WipeType", i);
        MR::getCsvDataS32(&info.mWipeFrame, pMap, "WipeFrame", i);
        mInfo.push_back(info);
    }
}

void DemoWipeKeeper::start() {
}

void DemoWipeKeeper::update() {
    DemoSheetKeeperInfoHolder< DemoWipeInfo >::update();
}

void DemoWipeKeeper::executeType(const DemoWipeInfo* pInfo) {
    if (MR::isDemoPartFirstStep(pInfo->mPartName)) {
        if (pInfo->mWipeType == 0) {
            SceneWipeHolderFunction::openWipe(pInfo->mWipeName, pInfo->mWipeFrame);
        } else if (pInfo->mWipeType == 1) {
            SceneWipeHolderFunction::closeWipe(pInfo->mWipeName, pInfo->mWipeFrame);
        } else if (pInfo->mWipeType == 2) {
            SceneWipeHolderFunction::forceOpenWipe(pInfo->mWipeName);
        } else if (pInfo->mWipeType == 3) {
            SceneWipeHolderFunction::forceCloseWipe(pInfo->mWipeName);
        }
    }
}

void DemoWipeKeeper::addInfo(const char* pPartName) {
    DemoWipeInfo info;
    info.mPartName = pPartName;
    mInfo.push_back(info);
}

inline const char* DemoWipeKeeper::getTypeString() const {
    return "Wipe";
}

inline const char* DemoWipeKeeper::getName() const {
    return "ワイプ";
}

#include "Game/Demo/DemoSheetKeeperInfoHolder.hpp"
