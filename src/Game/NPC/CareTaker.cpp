#include "Game/NPC/CareTaker.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/NPCActorItem.hpp"

Caretaker::Caretaker(const char* pName) : NPCActor(pName) {}
Caretaker::~Caretaker() {}

void Caretaker::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    NPCActorItem item("Caretaker");
    pArchiveList->addArchive("CaretakerMiddle");
    pArchiveList->addArchive("CaretakerLow");

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == 0) {
        pArchiveList->addArchive("BombTimer");

        if (MR::getNPCItemData(&item, 0)) {
            NPCActor::addArchive(pArchiveList, item);
        }
    }
}
