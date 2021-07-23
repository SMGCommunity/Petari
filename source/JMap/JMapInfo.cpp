#include "JMap/JMapInfo.h"
#include "JGadget/hashcode.h"
#include "smg.h"

JMapInfo::JMapInfo()
{
    mData = nullptr;
    mName = "Undifined";
}

JMapInfo::~JMapInfo()
{

}

bool JMapInfo::attach(const void *src)
{
    if (!src)
        return false;
    
    mData = static_cast<const JMapData*>(src);
	return true;
}

void JMapInfo::setName(const char *pName)
{
    mName = pName;
}

const char* JMapInfo::getName() const
{
    return mName;
}

s32 JMapInfo::searchItemInfo(const char* pPath) const
{
    JMapDataPtr jmp(mData);

    const bool valid = jmp.valid();

    if (!valid)
    {
        return -1;
    }

    const int numData = mData->mNumData;
    const u32 hash  = JGadget::getHashCode(pPath);

    for (int i = 0; i < numData; ++i) 
    {
        if (hash == mData->mItems[i].mHash)
        {
            return i;
        }
    }
    
    return -1;
}

s32 JMapInfo::getValueType(const char *pPath) const
{
    s32 idx = searchItemInfo(pPath);

    if (idx < 0)
    {
        return 7;
    }

    return mData->mItems[idx].mType;
}

s32 JMapInfo::getValueFast(int a1, int a2, const char **pOut) const
{
    const char* data = (char*)mData + mData->mDataOffset + (a1 * mData->_C) + mData->mItems[a2].mOffsetData;
    const JMapData::JMapItem& inf = mData->mItems[a2];

    switch (inf.mType)
    {
        case 6:
            *pOut = (const char*)mData + calcDataElementOffset() + *(u32*)data;
            break;
        default:
            *pOut = (const char*)data;
            break;
    }

    return 1;    
}