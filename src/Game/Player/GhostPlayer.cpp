#include "Game/Player/GhostPlayer.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include <cstring>

GhostPlayer::GhostPlayer(const char* pName) : LiveActor(pName) {}
GhostPlayer::~GhostPlayer() {}

void GhostPlayer::makeArchiveList(NameObjArchiveListCollector* pArchiveList, const JMapInfoIter&) {
    char archiveName[0x100];
    strcpy(archiveName, "GhostData");
    strcat(archiveName, MR::getCurrentStageName());
    pArchiveList->addArchive(archiveName);

    if (MR::isPlayerLuigi()) {
        pArchiveList->addArchive("GhostLuigi");
    }
    else {
        pArchiveList->addArchive("GhostMario");
    }
}
