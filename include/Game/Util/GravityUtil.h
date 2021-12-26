#pragma once

#include "JSystem/JGeometry/TVec.h"

class GravityInfo;
class LiveActor;
class NameObj;

namespace MR {
    void calcGravityVectorOrZero(const LiveActor *, TVec3f *, GravityInfo *, unsigned long);
    void calcGravityVectorOrZero(const NameObj *, const TVec3f &, TVec3f *, GravityInfo *, unsigned long);
}