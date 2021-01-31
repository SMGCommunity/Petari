#pragma once

#include "NameObj/NameObj.h"

class PlacementStateChecker : public NameObj
{
public:
    PlacementStateChecker(const char *);

    void setCurrentPlacementZoneId(s32);
    void clearCurrentPlacementZoneId();
    s32 getCurrentPlacementZoneId() const;

private:
    s32 mCurrentPlacementZoneId; // _C
};