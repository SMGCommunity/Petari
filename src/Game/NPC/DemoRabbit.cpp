#include "Game/NPC/DemoRabbit.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

DemoRabbit::DemoRabbit(const char* pName) : NPCActor(pName) {}
DemoRabbit::~DemoRabbit() {}

void DemoRabbit::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    if (MR::getDemoCastID(rIter) == 0) {
        pArchiveList->addArchive("TrickRabbitBaby");
    }
    else {
        pArchiveList->addArchive("TrickRabbit");
    }
}
