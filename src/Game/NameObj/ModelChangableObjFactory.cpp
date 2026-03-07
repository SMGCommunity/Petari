#include "Game/NameObj/ModelChangableObjFactory.hpp"
#include "Game/Boss/TripodBossFixParts.hpp"
#include "Game/Boss/TripodBossRailMoveParts.hpp"
#include "Game/Boss/TripodBossRotateParts.hpp"
#include "Game/Map/SunshadeMapParts.hpp"
#include "Game/MapObj/AssemblyBlock.hpp"
#include "Game/MapObj/ClipFieldMapParts.hpp"
#include "Game/MapObj/FlexibleSphere.hpp"
#include "Game/MapObj/MercatorFixParts.hpp"
#include "Game/MapObj/MercatorRailMoveParts.hpp"
#include "Game/MapObj/MercatorRotateParts.hpp"
#include "Game/MapObj/SimpleNormalMapObj.hpp"
#include "Game/NameObj/NameObjFactory.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include <cstring>

namespace {
    const Model2CreateFunc cModelChangableObjCreatorTable[] = {
        {
            nullptr,
            "AssemblyBlock",
            createNameObj< AssemblyBlock >,
        },
        {
            nullptr,
            "ClipFieldMapParts",
            createNameObj< ClipFieldMapParts >,
        },
        {
            nullptr,
            "FlexibleSphere",
            createNameObj< FlexibleSphere >,
        },
        {
            nullptr,
            "MercatorFixParts",
            createNameObj< MercatorFixParts >,
        },
        {
            nullptr,
            "MercatorRailMoveParts",
            createNameObj< MercatorRailMoveParts >,
        },
        {
            nullptr,
            "MercatorRotateParts",
            createNameObj< MercatorRotateParts >,
        },
        {
            nullptr,
            "TripodBossFixParts",
            createNameObj< TripodBossFixParts >,
        },
        {
            nullptr,
            "TripodBossRailMoveParts",
            createNameObj< TripodBossRailMoveParts >,
        },
        {
            nullptr,
            "TripodBossRotateParts",
            createNameObj< TripodBossRotateParts >,
        },
        {
            nullptr,
            "TripodBossRotateParts",
            createNameObj< TripodBossRotateParts >,
        },
        {
            nullptr,
            "SimpleNormalMapObj",
            createNameObj< SimpleNormalMapObj >,
        },
        {
            nullptr,
            "SunshadeMapParts",
            MR::createSunshadeMapParts,
        },
    };

    const Model2CreateFunc* getModel2CreateFunc(const char* pName) {
        for (s32 i = 0; i < sizeof(cModelChangableObjCreatorTable) / sizeof(*cModelChangableObjCreatorTable); i++) {
            const Model2CreateFunc* pModel2CreateFunc = &cModelChangableObjCreatorTable[i];
            bool v7;

            if (pModel2CreateFunc->_0 != nullptr) {
                v7 = strncmp(pName, pModel2CreateFunc->_0, strlen(pModel2CreateFunc->_0)) == 0;
            } else {
                v7 = strcmp(pName, pModel2CreateFunc->mArchiveName) == 0;
            }

            if (v7) {
                return pModel2CreateFunc;
            }
        }

        return nullptr;
    }
};  // namespace

namespace MR {
    CreatorFuncPtr getModelChangableObjCreator(const char* pName) {
        const Model2CreateFunc* pModel2CreateFunc = getModel2CreateFunc(pName);

        if (pModel2CreateFunc != nullptr) {
            return pModel2CreateFunc->mCreatorFunc;
        }

        return nullptr;
    }

    void requestMountModelChangableObjArchives(const char* pName, s32 a2) {
        char objectName[128];

        MR::getMapPartsObjectName(objectName, sizeof(objectName), pName, a2);
        MR::mountAsyncArchiveByObjectOrLayoutName(objectName, nullptr);
    }

    bool isReadResourceFromDVDAtModelChangableObj(const char* pName, s32 a2) {
        char objectName[128];
        char archiveName[128];

        MR::getMapPartsObjectName(objectName, sizeof(objectName), pName, a2);
        MR::makeObjectArchiveFileNameFromPrefix(archiveName, sizeof(archiveName), objectName, true);

        return !MR::isLoadedFile(archiveName);
    }
};  // namespace MR
