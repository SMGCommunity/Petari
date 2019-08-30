#include "JMap/JMapInfo.h"
#include "JGadget/hashcode.h"

JMapInfo::JMapInfo()
{
    this->_0 = 0;
    this->mName = "Undifined";
}

JMapInfo::~JMapInfo()
{
    delete this;
}

/*
bool JMapInfo::attach(const void *src)
{
    if (src == 0)
        return 0;
    
    this->_0 = src;
    return 1;
}
*/

void JMapInfo::setName(const char *name)
{
    this->mName = name;
}

const char* JMapInfo::getName() const
{
    return this->mName;
}

/*
s32 JMapInfo::searchItemInfo(const char *name) const
{
    u32 fieldCount;

    const BCSVHeader* header = this->mMap;

    if (header == 0x0)
       return -1; 
    else
    {
        if (header != 0)
        {
            fieldCount = header->mFieldCount;
        }
        else
        {
            fieldCount = 0;
        }
        
        u16 argHashCode = (u32)JGadget::getHashCode(name);
        s32 curIdx = 0;

        // we loop until we find our index based on the given string
        // if not, we return -1
        if (0 < fieldCount)
        {
            while(fieldCount != 0)
            {
                BCSVEntry* entry = this->mMap->mEntries[curIdx];

                if (entry->mNameHash == argHashCode)
                {
                    return curIdx;
                }

                curIdx++;
                fieldCount--;
            }
        }
    }

    return -1;
}
*/