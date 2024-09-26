#pragma once

#include "Game/NameObj/NameObj.hpp"

class PlacementStateChecker : public NameObj {
public:
    PlacementStateChecker(const char *);

    virtual ~PlacementStateChecker();

    void setCurrentPlacementZoneId(int);
    void clearCurrentPlacementZoneId();
    int getCurrentPlacementZoneId() const;

private:
    int mCurrentZoneId;    // 0xC
};

namespace MR {
    PlacementStateChecker* getPlacementStateChecker();
};
