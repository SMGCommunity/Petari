#include "Game/NameObj/ModelChangableObjFactory.hpp"
#include <cstring>

namespace MR {
    inline const Model2CreateFunc* getModelCreateFunc(const char *pName) {
        for (u32 i = 0; i < 0xC; i++) {
            const Model2CreateFunc* func = &cModelChangableObjCreatorTable[i];
            bool v7;
            if (func->_0 != nullptr) {
                v7 = strncmp(pName, func->_0, strlen(func->_0)) == 0;
            }
            else {
                v7 = strcmp(pName, func->mArchiveName) == 0;
            }

            if (v7) {
                return func;
            }
        }

        return nullptr;
    }

    CreationFuncPtr getModelChangableObjCreator(const char *pName) {
        const Model2CreateFunc* func = getModelCreateFunc(pName);

        if (func != nullptr) {
            return func->mCreationFunc;
        }

        return nullptr;
    }

    void requestMountModelChangableObjArchives(const char *pName, s32 a2) {
        char buf[0x80];
        MR::getMapPartsObjectName(buf, sizeof(buf), pName, a2);
        MR::mountAsyncArchiveByObjectOrLayoutName(buf, nullptr);
    }

    bool isReadResourceFromDVDAtModelChangableObj(const char *pName, s32 a2) {
        char buf[0x80];
        MR::getMapPartsObjectName(buf, sizeof(buf), pName, a2);
        char archiveName[0x80];
        MR::makeObjectArchiveFileNameFromPrefix(archiveName, sizeof(archiveName), buf, true);

        return MR::isLoadedFile(archiveName) == false;
    }
};
