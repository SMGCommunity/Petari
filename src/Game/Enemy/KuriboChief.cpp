#include "Game/Enemy/KuriboChief.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

KuriboChief::KuriboChief(const char* pName) : LiveActor(pName) {}
KuriboChief::~KuriboChief() {}

void KuriboChief::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    if (MR::isExistStageSwitchA(rIter)) {
        pArchiveList->addArchive("KeySwitch");
    }
}
