#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util/StringUtil.hpp"

NameObjArchiveListCollector::NameObjArchiveListCollector() : mArchiveNum() {
}

void NameObjArchiveListCollector::addArchive(const char* pName) {
    MR::copyString(mArchive[mArchiveNum], pName, ARRAY_SIZE(*mArchive));

    mArchiveNum++;
}

const char* NameObjArchiveListCollector::getArchive(s32 index) const {
    return mArchive[index];
}
