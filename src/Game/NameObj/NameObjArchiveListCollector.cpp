#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

NameObjArchiveListCollector::NameObjArchiveListCollector() {
    mCount = 0;
}

void NameObjArchiveListCollector::addArchive(const char *pArchive) {
    char* str = mArchiveNames[mCount];
    MR::copyString(str, pArchive, 0x40);
    mCount++;
}

const char* NameObjArchiveListCollector::getArchive(s32 idx) const {
    return mArchiveNames[idx];
}