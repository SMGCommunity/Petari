#include "Game/NPC/CareTaker.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/NPCUtil.hpp"

Caretaker::Caretaker(const char* pName) : NPCActor(pName) {
}
Caretaker::~Caretaker() {
}

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
