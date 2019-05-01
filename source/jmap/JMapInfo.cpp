#include "jmap/JMapInfo.h"

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
        this->mMap = (void*)src;
        return true;
    }
    else
        return false;
    
}

void JMapInfo::setName(const char *name)
{
    this->mName = (char*)name;
}