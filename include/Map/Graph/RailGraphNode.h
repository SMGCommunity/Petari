#ifndef RAILGRAPHNODE_H
#define RAILGRAPHNODE_H

#include "JGeometry/TVec3.h"
#include <revolution.h>

class RailGraphNode
{
public:
    RailGraphNode();

    JGeometry::TVec3<f32> mPosition; // _0
    s32 _C;
    u32 _10;
};

#endif // RAILGRAPHNODE_H