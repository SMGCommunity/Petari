#pragma once

#include "Game/NameObj/NameObj.hpp"

class PlacementStateChecker : public NameObj {
public:
    PlacementStateChecker(const char*);

    void setCurrentPlacementZoneId(int);
    void clearCurrentPlacementZoneId();
    int getCurrentPlacementZoneId() const;

public:
    /* 0xC */ int mCurrentZoneId;
};

namespace MR {
    PlacementStateChecker* getPlacementStateChecker();
};  // namespace MR
