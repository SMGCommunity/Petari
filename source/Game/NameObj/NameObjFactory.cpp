#include "Game/NameObj/NameObjFactory.h"
#include "Game/Map/PlanetMapCreator.h"
#include "Game/Util.h"

CreationFuncPtr NameObjFactory::getCreator(const char *pName) {
    if (PlanetMapCreatorFunction::isRegisteredObj(pName)) {
        return PlanetMapCreatorFunction::getPlanetMapCreator(pName);
    }
    
    const Name2CreateFunc* func = NameObjFactory::getName2CreateFunc(pName, nullptr);
    return !func ? nullptr : func->mCreationFunc;
}

void NameObjFactory::requestMountObjectArchives(const char *pName, const JMapInfoIter &rIter) {
    NameObjArchiveListCollector archiveList;
    NameObjFactory::getMountObjectArchiveList(&archiveList, pName, rIter);

    for (int i = 0; i < archiveList.mCount; i++) {
        MR::mountAsyncArchiveByObjectOrLayoutName(archiveList.getArchive(i), nullptr);
    }
}

bool NameObjFactory::isReadResourceFromDVD(const char *pName, const JMapInfoIter &rIter) {
    NameObjArchiveListCollector archiveList;
    NameObjFactory::getMountObjectArchiveList(&archiveList, pName, rIter);

    for (int i = 0; i < archiveList.mCount; i++) {
        if (!MR::isLoadedObjectOrLayoutArchive(archiveList.getArchive(i))) {
            return true;
        }
    }

    return false;
}

bool NameObjFactory::isPlayerArchiveLoaderObj(const char *pArchive) {
    for (u32 i = 0; i < 0x8; i++) {
        if (MR::isEqualStringCase(cPlayerArchiveLoaderObjTable[i], pArchive)) {
            return true;
        }
    }

    return false;
}

const NameObjFactory::Name2CreateFunc* NameObjFactory::getName2CreateFunc(const char *pName, const NameObjFactory::Name2CreateFunc *pTable) {
    if (!pTable) {
        pTable = cCreateTable;
    }


    const Name2CreateFunc* last = &(cCreateTable[0x49F]);

    for (const Name2CreateFunc* i = pTable; i != last; i++) {
        if (i->mName) {
            if (MR::isEqualStringCase(i->mName, pName)) {
                return i;
            }
        }
    }

    return 0;
}

void NameObjFactory::getMountObjectArchiveList(NameObjArchiveListCollector *pArchiveList, const char *pName, const JMapInfoIter &rIter) {
    if (PlanetMapCreatorFunction::isRegisteredObj(pName)) {
        PlanetMapCreatorFunction::makeArchiveList(pArchiveList, rIter, pName);
    }
    else {
        const Name2CreateFunc* creationFunc = NameObjFactory::getName2CreateFunc(pName, nullptr);

        if (creationFunc && creationFunc->mArchiveName != 0) { 
            pArchiveList->addArchive(creationFunc->mArchiveName);
        }

        for (NameObjFactory::Name2Archive* curArchive = cName2ArchiveNamesTable; curArchive != &cName2ArchiveNamesTable[0x1B9]; curArchive++) {
            if (MR::isEqualString(curArchive->mObjectName, pName)) {
                pArchiveList->addArchive(curArchive->mArchiveName);
            }
        }

        for (const NameObjFactory::Name2MakeArchiveListFunc* curFunc = cName2MakeArchiveListFuncTable; curFunc != &cName2MakeArchiveListFuncTable[0x5B]; curFunc++) {
            if (MR::isEqualString(curFunc->mName, pName)) {
                curFunc->mArchiveFunc(pArchiveList, rIter);
            }
        }
    }
}