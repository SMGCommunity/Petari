#include "Game/Enemy/OtaRock.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "Game/MapObj/CocoNut.hpp"

OtaRock::OtaRock(const char* pName) : LiveActor(pName) {}
OtaRock::~OtaRock() {}

void OtaRock::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    bool noCocoNut = false;
    MR::getJMapInfoArg0NoInit(rIter, &noCocoNut);

    if (!noCocoNut) {
        pArchiveList->addArchive(CocoNut::getModelName());
    }
}
