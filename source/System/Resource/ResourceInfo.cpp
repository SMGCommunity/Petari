#include "System/Resource/ResourceInfo.h"
#include "MR/HashUtil.h"

ResFileInfo::ResFileInfo()
{
    this->mResName = 0;
    this->mHashCode = 0;
    this->mRes = 0;
    this->_8 = 0;
    this->_4 = 0;
    this->_C = 0;
}

ResTable::ResTable()
{
    this->mFileInfo = 0;
    this->mResCount = 0;
}

void ResTable::newFileInfoTable(u32 numEntries)
{
    this->mFileInfo = new ResFileInfo[numEntries];
}

ResFileInfo* ResTable::add(const char *name, void *res, bool a3)
{
    ResFileInfo* inf = &this->mFileInfo[this->mResCount];
    this->mFileInfo[this->mResCount].mRes = res;
    inf->setName(name, a3);
    s32 curCount = this->mResCount;
    this->mResCount = curCount + 1;
    return inf;
}

const char* ResTable::getResName(u32 idx) const
{
    return this->mFileInfo[idx].mResName;
}

void* ResTable::getRes(u32 idx) const
{
    return this->mFileInfo[idx].mRes;
}

void* ResTable::getRes(const char *name) const
{
    return this->findRes(name);
}

ResFileInfo* ResTable::findFileInfo(const char *name) const
{
    s32 resIndex = this->getResIndex(name);
    
    if (resIndex == -1)
        return 0;

    return &this->mFileInfo[resIndex];
}

ResFileInfo* ResTable::getFileInfo(u32 idx) const
{
    return &this->mFileInfo[idx];
}

bool ResTable::isExistRes(const char *name) const
{
    return this->getResIndex(name);
}

void* ResTable::findRes(const char *name) const
{
    s32 resIndex = this->getResIndex(name);

    if (resIndex == -1)
        return 0;

    return this->mFileInfo[resIndex].mRes;
}

s32 ResTable::getResIndex(const char *name) const
{
    s32 nameHash = MR::getHashCodeLower(name);
    s32 curIdx = 0;

    while(curIdx <= this->mResCount)
    {
        s32 hash = this->mFileInfo[curIdx].mHashCode;

        if (hash == nameHash)
            return curIdx;

        curIdx++;
    }

    return -1;
}