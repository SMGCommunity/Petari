#include "NameObj/NameObjArchiveListCollector.h"
#include "Util/StringUtil.h"

NameObjArchiveListCollector::NameObjArchiveListCollector() : mNumArchives(0) { }

void NameObjArchiveListCollector::addArchive(const char *pArchive)
{
    MR::copyString(mArchives[mNumArchives], pArchive, 0x40);
    mNumArchives++;
}

const char* NameObjArchiveListCollector::getArchive(u32 idx) const
{
    return mArchives[idx];
}