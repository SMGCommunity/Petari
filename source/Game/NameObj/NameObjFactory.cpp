#include "Game/NameObj/NameObjFactory.h"
#include "Game/Util.h"

bool NameObjFactory::isPlayerArchiveLoaderObj(const char *pArchive) {
    for (s32 i = 0; i < 0x8; i++) {
        if (MR::isEqualStringCase(cPlayerArchiveLoaderObjTable[i], pArchive)) {
            return true;
        }
    }

    return false;
}

// this isn't matching yet but I'm leaving it here to allow the createNameObj functions to generate
const NameObjFactory::Name2CreateFunc* NameObjFactory::getName2CreateFunc(const char *pName, const NameObjFactory::Name2CreateFunc *pTable) {
    if (!pTable) {
        pTable = cCreateTable;
    }

    Name2CreateFunc* last = (Name2CreateFunc*)cName2ArchiveNamesTable;

    for (const Name2CreateFunc* i = pTable; i != last; i++) {
        if (i->mName) {
            if (MR::isEqualStringCase(i->mName, pName)) {
                return i;
            }
        }
    }

    return 0;
}