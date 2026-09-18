#pragma once

#include "Game/Map/RailGraphIter.hpp"
#include "Game/Map/RailGraphNode.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
class RailGraphNodeSelecter {
public:
    virtual bool isSatisfy(RailGraphIter& rIter) = 0;
};

namespace MR {
    RailGraph* createRailGraphFromJMap(const JMapInfoIter& rIter);
    RailGraphIter* createRailGraphIter(const RailGraph* pRailGraph);
    void moveNodeNearPosition(RailGraphIter* pRailGraphIter, const TVec3f& rPosition, f32 maxDistance, RailGraphNodeSelecter* pSelector);
    TVec3f* getNextNodePosition(const RailGraphIter* pRailGraphIter);
    TVec3f* getCurrentNodePosition(const RailGraphIter* pRailGraphIter);
    void selectReverseEdge(RailGraphIter* pRailGraphIter);
    void moveNextNode(RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg0(const RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg1(const RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg2(const RailGraphIter* pRailGraphIter);
    s32 getSelectEdgeArg3(const RailGraphIter* pRailGraphIter);
    bool isSelectedEdge(const RailGraphIter* pRailGraphIter);
    bool isWatchedPrevEdge(const RailGraphIter* pRailGraphIter);
    s32 getWatchEdgeArg7(const RailGraphIter* pRailGraphIter);
    void calcNextEdgeVector(const RailGraphIter* pIter, TVec3f* pEdge);
    void calcWatchEdgeVector(const RailGraphIter* pIter, TVec3f* pEdge) NO_INLINE;
    void calcWatchEdgeDirection(const RailGraphIter* pRailGraphIter, TVec3f* pVec);

    s32 getNearNodeIndex(const RailGraph* pGraph, const TVec3f& rPosition, f32 maxDistance, RailGraphNodeSelecter* pSelector);
}  // namespace MR
