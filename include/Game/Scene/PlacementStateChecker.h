#pragma once

#include "Game/NameObj/NameObj.h"

class PlacementStateChecker : public NameObj {
public:
    PlacementStateChecker(const char *);

    virtual ~PlacementStateChecker();

    void setCurrentPlacementZoneId(int);
    void clearCurrentPlacementZoneId();
    int getCurrentPlacementZoneId() const;

private:
    int mCurrentZoneId;    // _C
};

namespace MR {
    PlacementStateChecker* getPlacementStateChecker();
};
