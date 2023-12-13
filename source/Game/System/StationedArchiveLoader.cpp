#include "Game/System/StationedArchiveLoader.hpp"
#include "Game/System/ResourceHolderManager.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util.hpp"

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
    // grab the stationed file info table
    const MR::StationedFileInfo* info = MR::getStationedFileInfoTable();

    // while the current archive is not nullptr (the last entry in the list is a null entry)
    while (info->mArchive != nullptr) {
        // if the condition of the loader is to execute the loading of archives
        if (rCondition.isExecute(info)) {
            // grab the proper heap for loading the archive (0 = NAPA, 1 = GDDR)
            JKRHeap* heap = rCondition.getProperHeap(info);

            // if there is not a specific proper heap to use, we try the archive loader's specified heap
            if (heap == nullptr) {
                heap = StationedArchiveLoader::getProperHeap(info);
            }

            // load the file into memory based on the load type
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

        // increment to next entry
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