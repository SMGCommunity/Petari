#ifndef BEZIERRAIL_H
#define BEZIERRAIL_H

#include "JMap/JMapInfo.h"
#include "JMap/JMapInfoIter.h"
#include "Map/Rail/RailPart.h"

class BezierRail
{
public:
    BezierRail(const JMapInfoIter &, const JMapInfo *);
};

#endif // BEZIERRAIL_H