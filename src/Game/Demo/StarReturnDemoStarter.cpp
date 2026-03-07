#include "Game/Demo/StarReturnDemoStarter.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/System/GameSequenceFunction.hpp"

StarReturnDemoStarter::StarReturnDemoStarter(const char* pName) : LiveActor(pName) {}
StarReturnDemoStarter::~StarReturnDemoStarter() {}

void StarReturnDemoStarter::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter&) {
    if (GameSequenceFunction::isGrandStarAtResultSequence()) {
        pArchiveList->addArchive("GrandStar");
    }

    pArchiveList->addArchive("SpinTico");
    pArchiveList->addArchive("MarioHair");
    pArchiveList->addArchive("SpinDriverPath");
}
