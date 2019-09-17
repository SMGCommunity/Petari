#include "System/Resource/ResourceInfo.h"
#include "MR/HashUtil.h"

ResFileInfo::ResFileInfo()
{
    mResName = 0;
    mHashCode = 0;
    mRes = 0;
    _8 = 0;
    _4 = 0;
    _C = 0;
}

ResTable::ResTable()
{
    mFileInfo = 0;
    mResCount = 0;
}

void ResTable::newFileInfoTable(u32 numEntries)
{
    mFileInfo = new ResFileInfo[numEntries];
}

ResFileInfo* ResTable::add(const char *pName, void *pRes, bool a3)
{
    ResFileInfo *pInf = &mFileInfo[mResCount];
    mFileInfo[mResCount].mRes = pRes;
    pInf->setName(pName, a3);
    s32 curCount = mResCount;
    mResCount = curCount + 1;
    return pInf;
}

const char* ResTable::getResName(u32 idx) const
{
    return mFileInfo[idx].mResName;
}

void* ResTable::getRes(u32 idx) const
{
    return mFileInfo[idx].mRes;
}

void* ResTable::getRes(const char *pName) const
{
    return findRes(pName);
}

ResFileInfo* ResTable::findFileInfo(const char *pName) const
{
    s32 resIndex = getResIndex(pName);
    
    if (resIndex == -1)
    {
        return 0;
    }

    return &mFileInfo[resIndex];
}

ResFileInfo* ResTable::getFileInfo(u32 idx) const
{
    return &mFileInfo[idx];
}

bool ResTable::isExistRes(const char *pName) const
{
    return getResIndex(pName);
}

void* ResTable::findRes(const char *pName) const
{
    s32 resIndex = getResIndex(pName);

    if (resIndex == -1)
    {
        return 0;
    }

    return mFileInfo[resIndex].mRes;
}

s32 ResTable::getResIndex(const char *pName) const
{
    s32 nameHash = MR::getHashCodeLower(pName);
    s32 curIdx = 0;

    while(curIdx <= mResCount)
    {
        s32 hash = mFileInfo[curIdx].mHashCode;

        if (hash == nameHash)
        {
            return curIdx;
        }

        curIdx++;
    }

    return -1;
}