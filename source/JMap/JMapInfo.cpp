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
    if (src == 0)
        return 0;
    
    this->mMap = src;
    return 1;
}

void JMapInfo::setName(const char *name)
{
    this->mName = name;
}

const char* JMapInfo::getName() const
{
    return this->mName;
}