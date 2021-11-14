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