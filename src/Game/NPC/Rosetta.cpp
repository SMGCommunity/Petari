#include "Game/NPC/Rosetta.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

void Rosetta::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    const char* pObjectName = nullptr;

    if (!MR::getObjectName(&pObjectName, rIter)) {
        return;
    }

    for (s32 i = 0; i < pCollector->getArchiveNum(); i++) {
        if (MR::isEqualString(pCollector->getArchive(i), pObjectName)) {
            return;
        }
    }

    pCollector->addArchive(pObjectName);
}
