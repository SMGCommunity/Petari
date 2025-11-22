#include "Game/System/ResourceHolderManager.hpp"
#include "Game/System/StationedArchiveLoader.hpp"
#include "Game/System/StationedFileInfo.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>

StationedArchiveLoader::Condition::~Condition() {
}

JKRHeap* StationedArchiveLoader::Condition::getProperHeap(const MR::StationedFileInfo* pInfo) const {
    return nullptr;
}

JKRHeap* StationedArchiveLoader::getProperHeap(const MR::StationedFileInfo* pInfo) {
    switch (pInfo->mHeapType) {
    case 0:
        return MR::getStationedHeapNapa();
    case 1:
        return MR::getStationedHeapGDDR3();
    default:
        return nullptr;
    }
}

void StationedArchiveLoader::loadResourcesFromTable(const StationedArchiveLoader::Condition& rCondition) {
    for (const MR::StationedFileInfo* pInfo = MR::getStationedFileInfoTable(); pInfo->mArchive != nullptr; pInfo++) {
        if (!rCondition.isExecute(pInfo)) {
            continue;
        }

        JKRHeap* pHeap = rCondition.getProperHeap(pInfo);

        if (pHeap == nullptr) {
            pHeap = StationedArchiveLoader::getProperHeap(pInfo);
        }

        switch (pInfo->mLoadType) {
        case 0:
            MR::loadToMainRAM(pInfo->mArchive, nullptr, pHeap, JKRDvdRipper::ALLOC_DIRECTION_1);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            MR::mountArchive(pInfo->mArchive, pHeap);
            break;
        }
    }
}

void StationedArchiveLoader::createAndAddResourcesFromTable(const Condition& rCondition) {
    for (const MR::StationedFileInfo* pInfo = MR::getStationedFileInfoTable(); pInfo->mArchive != nullptr; pInfo++) {
        if (!rCondition.isExecute(pInfo)) {
            continue;
        }

        switch (pInfo->mLoadType) {
        case 2:
        case 3:
        case 4:
            SingletonHolder<ResourceHolderManager>::get()->createAndAddStationed(pInfo->mArchive);
            break;
        case 5:
            SingletonHolder<ResourceHolderManager>::get()->createAndAddLayoutHolderStationed(pInfo->mArchive);
            break;
        default:
            break;
        }
    }
}

void StationedArchiveLoader::loadScenarioData(JKRHeap* pHeap) {
    DVDDir      dir;
    DVDDirEntry dirEntry;

    DVDOpenDir("/StageData", &dir);

    while (DVDReadDir(&dir, &dirEntry)) {
        if (!dirEntry.isDir) {
            continue;
        }

        char name[256];
        MR::makeScenarioArchiveFileName(name, sizeof(name), dirEntry.name);

        if (MR::isFileExist(name, false)) {
            MR::mountArchive(name, pHeap);
        }
    }

    DVDCloseDir(&dir);
}
