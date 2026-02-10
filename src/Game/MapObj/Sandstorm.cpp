#include "Game/MapObj/Sandstorm.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

Sandstorm::Sandstorm(const char* pName) : LiveActor(pName) {}
Sandstorm::~Sandstorm() {}

void Sandstorm::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool useTrap = false;
    MR::getJMapInfoArg0NoInit(rIter, &useTrap);

    if (useTrap) {
        pArchiveList->addArchive("SunakazeKunTrap");
    }
}
