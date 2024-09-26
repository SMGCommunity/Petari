#include "Game/NameObj/ModelChangableObjFactory.hpp"

namespace MR {
    void requestMountModelChangableObjArchives(const char *pName, s32 a2) {
        char buf[0x80];
        MR::getMapPartsObjectName(buf, 0x80, pName, a2);
        MR::mountAsyncArchiveByObjectOrLayoutName(buf, nullptr);
    }

    bool isReadResourceFromDVDAtModelChangableObj(const char *pName, s32 a2) {
        char buf[0x80];
        MR::getMapPartsObjectName(buf, 0x80, pName, a2);
        char archiveName[0x80];
        MR::makeObjectArchiveFileNameFromPrefix(archiveName, 0x80, buf, true);
        return MR::isLoadedFile(archiveName) == false;
    }
};