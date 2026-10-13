#pragma once

#include "Game/Map/RailGraphIter.hpp"
#include "Game/Map/RailGraphNode.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class RailGraphNodeSelecter {
public:
    virtual bool isSatisfy(RailGraphIter& rIter) = 0;
};

namespace MR {
    RailGraph* createRailGraphFromJMap(const JMapInfoIter& rIter);
    RailGraphIter* createRailGraphIter(const RailGraph* pRailGraph);
    void moveNextNode(RailGraphIter* pRailGraphIter);
    void moveNodeNearPosition(RailGraphIter* pRailGraphIter, const TVec3f& rPosition, f32 maxDistance, RailGraphNodeSelecter* pSelector);
    void selectReverseEdge(RailGraphIter* pRailGraphIter);
    bool isSelectedEdge(const RailGraphIter* pRailGraphIter);
    bool isWatchedPrevEdge(const RailGraphIter* pRailGraphIter);
    TVec3f* getCurrentNodePosition(const RailGraphIter* pRailGraphIter);
    TVec3f* getNextNodePosition(const RailGraphIter* pRailGraphIter);
    void calcWatchEdgeVector(const RailGraphIter* pIter, TVec3f* pEdge) NO_INLINE;
    void calcWatchEdgeDirection(const RailGraphIter* pRailGraphIter, TVec3f* pVec);
    s32 getNearNodeIndex(const RailGraph* pGraph, const TVec3f& rPosition, f32 maxDistance, RailGraphNodeSelecter* pSelector);
    s32 getSelectEdgeArg0(const RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg1(const RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg2(const RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg3(const RailGraphIter* pRailGraphIter);
    s32 getWatchEdgeArg7(const RailGraphIter* pRailGraphIter);
};  // namespace MR
