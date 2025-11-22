#pragma once

#include "Game/Map/RailGraphIter.hpp"
#include "Game/Map/RailGraphNode.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
class RailGraphNodeSelecter;
namespace MR {
    RailGraph* createRailGraphFromJMap(const JMapInfoIter& rIter);
    RailGraphIter* createRailGraphIter(const RailGraph* pRailGraph);
    void moveNodeNearPosition(RailGraphIter* pRailGraphIter, const TVec3f& rVec, f32 f, RailGraphNodeSelecter* pSelector);
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
    void calcWatchEdgeDirection(const RailGraphIter* pRailGraphIter, TVec3f* pVec);
}  // namespace MR
