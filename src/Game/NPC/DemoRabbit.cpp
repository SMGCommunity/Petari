#include "Game/NPC/DemoRabbit.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

DemoRabbit::DemoRabbit(const char* pName) : NPCActor(pName) {
}
DemoRabbit::~DemoRabbit() {
}

void DemoRabbit::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    if (MR::getDemoCastID(rIter) == 0) {
        pCollector->addArchive("TrickRabbitBaby");
    } else {
        pCollector->addArchive("TrickRabbit");
    }
}
