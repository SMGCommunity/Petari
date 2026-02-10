#include "Game/NPC/RunawayTico.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

RunawayTico::RunawayTico(const char* pName) : Tico(pName) {}
RunawayTico::~RunawayTico() {}

void RunawayTico::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter& rIter) {
    s32 arg0 = 0;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 == -1) {
        pArchiveList->addArchive("TicoBaby");
    }
    else {
        pArchiveList->addArchive("Tico");
        pArchiveList->addArchive("TicoMiddle");
        pArchiveList->addArchive("TicoLow");
    }
}
