#include "JMap/JMapInfo.h"
#include "JGadget/hashcode.h"

JMapInfo::JMapInfo()
{
    mData = nullptr;
    mName = "Undifined";
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