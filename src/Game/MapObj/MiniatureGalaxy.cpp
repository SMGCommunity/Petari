#include "Game/MapObj/MiniatureGalaxy.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include <cstring>

namespace {
    const char cDummyArchivePrefix[] = "ABCDEFGHIJKLMNOPQR";
    const char cMini[] = "Mini";
    const char cMiniKoopaGalaxy[] = "MiniKoopaGalaxy";
    const char cMiniatureGalaxyShadow[] = "MiniatureGalaxyShadow";
    const char cMiniatureGalaxySelect[] = "MiniatureGalaxySelect";
    const char cUnknown[] = "Unknown";
    const char cEyeLight[] = "EyeLight";
    const char cMiniatureGalaxyUnknown[] = "MiniatureGalaxyUnknown";
    const char cMiniHatenaGalaxy[] = "MiniHatenaGalaxy";
    const char cKoopaBattleVs[] = "KoopaBattleVs";
    const char cMiniatureGalaxyUnknownKoopa[] = "MiniatureGalaxyUnknownKoopa";
    const char cMiniatureGalaxyStarNumber[] = "MiniatureGalaxyStarNumber";
    const char cGalaxyNamePlate[] = "GalaxyNamePlate";
}

MiniatureGalaxy::MiniatureGalaxy(const char* pName) : LiveActor(pName) {}

void MiniatureGalaxy::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    const char* pObjectName;
    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    s32 archiveType = arg0;
    MR::getObjectName(&pObjectName, rIter);

    if (archiveType == 0) {
        pArchiveList->addArchive(pObjectName);
        pArchiveList->addArchive(cMiniatureGalaxyUnknown);
    }
    else if (archiveType == 1) {
        pArchiveList->addArchive(pObjectName);
        pArchiveList->addArchive(cMiniHatenaGalaxy);
    }
    else if (archiveType == 2) {
        const char* pStageName;
        u32 miniLen = strlen(cMini);
        s32 isUnknownKoopa = 1;
        pStageName = pObjectName + miniLen;

        if (!MR::isEqualSubString(pObjectName, cKoopaBattleVs)) {
            if (MR::isAlreadyVisitedStage(pStageName)) {
                isUnknownKoopa = 0;
            }
        }

        pArchiveList->addArchive(isUnknownKoopa ? cMiniKoopaGalaxy : pObjectName);
        pArchiveList->addArchive(cMiniatureGalaxyUnknownKoopa);
    }

    pArchiveList->addArchive(cMiniatureGalaxyShadow);
    pArchiveList->addArchive(cMiniatureGalaxySelect);
    pArchiveList->addArchive(cMiniatureGalaxyStarNumber);
    pArchiveList->addArchive(cGalaxyNamePlate);
}
