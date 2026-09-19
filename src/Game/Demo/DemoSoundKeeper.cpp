#include "Game/Demo/DemoSoundKeeper.hpp"
#include "Game/Demo/DemoFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

DemoSoundInfo::DemoSoundInfo() : mPartName(), mBgm(""), mSystemSe(""), mReturnBgm(), mBgmWipeoutFrame(-1) {
}

DemoSoundKeeper::DemoSoundKeeper(DemoExecutor* pExecutor) : DemoSheetKeeperBase(pExecutor) {
    JMapInfo* pMap = nullptr;
    const DemoExecutor* pSheetExecutor = mExecutor;
    const s32& count = DemoFunction::createSheetParser(pSheetExecutor, getTypeString(), &pMap);
    mInfo.init(count);

    for (s32 i = 0; i < count; i++) {
        DemoSoundInfo info;
        pMap->getValue(i, "PartName", &info.mPartName);
        pMap->getValue(i, "Bgm", &info.mBgm);
        pMap->getValue(i, "SystemSe", &info.mSystemSe);
        pMap->getValue(i, "ReturnBgm", reinterpret_cast< s32* >(&info.mReturnBgm));
        pMap->getValue(i, "BgmWipeoutFrame", &info.mBgmWipeoutFrame);
        mInfo.push_back(info);
    }
}

void DemoSoundKeeper::update() {
    DemoSheetKeeperInfoHolder< DemoSoundInfo >::update();
}

void DemoSoundKeeper::addInfo(const char* pPartName) {
    DemoSoundInfo info;
    info.mPartName = pPartName;
    mInfo.push_back(info);
}

void DemoSoundKeeper::executeType(const DemoSoundInfo* pInfo) {
    if (MR::isDemoPartFirstStep(pInfo->mPartName)) {
        if (pInfo->mBgmWipeoutFrame >= 0 && isPermitBgmChange()) {
            MR::stopStageBGM(pInfo->mBgmWipeoutFrame);
        }

        if (!MR::isNullOrEmptyString(pInfo->mBgm) && isPermitBgmChange()) {
            MR::startStageBGM(pInfo->mBgm, false);
        }

        if (!MR::isNullOrEmptyString(pInfo->mSystemSe)) {
            MR::startSystemSE(pInfo->mSystemSe);
        }
    } else if (MR::isDemoPartLastStep(pInfo->mPartName) && pInfo->mReturnBgm && isPermitBgmChange()) {
        MR::startLastStageBGM();
    }
}

bool DemoSoundKeeper::isPermitBgmChange() {
    if (MR::isGalaxyRedCometAppearInCurrentStage() || MR::isGalaxyBlackCometAppearInCurrentStage()) {
        return false;
    }

    return true;
}

inline const char* DemoSoundKeeper::getTypeString() const {
    return "Sound";
}

inline const char* DemoSoundKeeper::getName() const {
    return "サウンド";
}

#include "Game/Demo/DemoSheetKeeperInfoHolder.hpp"
