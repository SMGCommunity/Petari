#include "jmap/JMapInfo.h"

#include "jgadget/hash.h"

JMapInfo::JMapInfo()
{
    this->mMap = 0;
    this->mName = "Undifined";
}

JMapInfo::~JMapInfo()
{
    delete this;
}

bool JMapInfo::attach(const void *src)
{
    if (src)
    {
        this->mMap = src;
        return true;
    }
    else
        return false;
    
}

void JMapInfo::setName(const char *name)
{
    this->mName = name;
}

const char* JMapInfo::getName() const
{
    return this->mName;
}

s32 JMapInfo::searchItemInfo(const char *name) const
{
    /* TODO */
}