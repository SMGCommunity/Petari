#include "Game/System/StationedArchiveLoader.h"
#include "Game/System/ResourceHolderManager.h"
#include "Game/SingletonHolder.h"
#include "Game/Util.h"

StationedArchiveLoader::Condition::~Condition() {

}

JKRHeap* StationedArchiveLoader::Condition::getProperHeap(const MR::StationedFileInfo *pInfo) const {
    return nullptr;
}

JKRHeap* StationedArchiveLoader::getProperHeap(const MR::StationedFileInfo *pInfo) {
    switch (pInfo->mHeapType) {
        case 0:
            return (JKRHeap*)MR::getStationedHeapNapa();
            break;
        case 1:
            return (JKRHeap*)MR::getStationedHeapGDDR3();
            break;
    }
    
    return nullptr;
}

void StationedArchiveLoader::loadResourcesFromTable(const StationedArchiveLoader::Condition &rCondition) {
    const MR::StationedFileInfo* info = MR::getStationedFileInfoTable();

    while (info->mArchive != nullptr) {
        if (rCondition.isExecute(info)) {
            JKRHeap* heap = rCondition.getProperHeap(info);

            if (heap == nullptr) {
                heap = StationedArchiveLoader::getProperHeap(info);
            }

            switch (info->mLoadType) {
                case 0:
                    MR::loadToMainRAM(info->mArchive, nullptr, heap, JKRDvdRipper::ALLOC_DIRECTION_1);
                    break;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    MR::mountArchive(info->mArchive, heap);
                    break;
            }
        }

        info++;
    }
}

void StationedArchiveLoader::createAndAddResourcesFromTable(const StationedArchiveLoader::Condition &rCondition) {
    const MR::StationedFileInfo* info = MR::getStationedFileInfoTable();

    while (info->mArchive != nullptr) {
        if (rCondition.isExecute(info)) {
            switch (info->mLoadType) {
                case 2:
                    SingletonHolder<ResourceHolderManager>::sInstance->createAndAddStationed(info->mArchive);
                    break;
                case 5:
                    SingletonHolder<ResourceHolderManager>::sInstance->createAndAddLayoutHolderStationed(info->mArchive);
                    break;
            }
        }
        
        info++;
    }
}

void StationedArchiveLoader::loadScenarioData(JKRHeap* pHeap) {
    DVDDir dir;
    DVDDirEntry entry;
    DVDOpenDir("/StageData", &dir);

    while (DVDReadDir(&dir, &entry) != 0) {
        if (entry.isDir == false) {
            continue;
        }

        char name[0x100];
        MR::makeScenarioArchiveFileName(name, 0x100, entry.name);

        if (MR::isFileExist(name, false)) {
            MR::mountArchive(name, pHeap);
        }
    }

    DVDCloseDir(&dir);
}