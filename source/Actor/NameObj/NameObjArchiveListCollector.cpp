#include "Actor/NameObj/NameObjArchiveListCollector.h"
#include "MR/StringUtil.h"

NameObjArchiveListCollector::NameObjArchiveListCollector()
{
    this->mNumArchives = 0;
}

void NameObjArchiveListCollector::addArchive(const char *archive)
{
    MR::copyString(this->mArchives[this->mNumArchives], archive, 0x40);
    this->mNumArchives++;
}

const char* NameObjArchiveListCollector::getArchive(u32 idx) const
{
    return this->mArchives[idx];
}