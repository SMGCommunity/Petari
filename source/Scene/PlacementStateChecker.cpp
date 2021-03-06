#include "Scene/PlacementStateChecker.h"

PlacementStateChecker::PlacementStateChecker(const char *pName) : NameObj(pName)
{
    mCurrentPlacementZoneId = -1;
}

void PlacementStateChecker::setCurrentPlacementZoneId(s32 id)
{
    mCurrentPlacementZoneId = id;
}

void PlacementStateChecker::clearCurrentPlacementZoneId()
{
    mCurrentPlacementZoneId = -1;
}

s32 PlacementStateChecker::getCurrentPlacementZoneId() const
{
    return mCurrentPlacementZoneId;
}