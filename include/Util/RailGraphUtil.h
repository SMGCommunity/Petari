#pragma once

#include "Map/Graph/RailGraphIter.h"

namespace MR
{
    RailGraphIter* createRailGraphIter(const RailGraph *);
    void moveNextNode(RailGraphIter *);
    void selectReverseEdge(RailGraphIter *);
    bool isSelectedEdge(const RailGraphIter *);
    bool isWatchedPrevEdge(const RailGraphIter *);
    RailGraphNode* getCurrentNodePosition(const RailGraphIter *);
    RailGraphNode* getNextNodePosition(const RailGraphIter *);
    void calcWatchEdgeVector(const RailGraphIter *, JGeometry::TVec3<f32> *);
    void calcWatchEdgeDirection(const RailGraphIter *, JGeometry::TVec3<f32> *);

    s32 getSelectEdgeArg0(const RailGraphIter *);
    s32 getSelectEdgeArg1(const RailGraphIter *);
    s32 getSelectEdgeArg2(const RailGraphIter *);
    s32 getSelectEdgeArg3(const RailGraphIter *);
    s32 getSelectEdgeArg7(const RailGraphIter *);
};