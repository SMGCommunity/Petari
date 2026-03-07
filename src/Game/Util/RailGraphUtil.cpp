#include "Game/Util/RailGraphUtil.hpp"
#include "Game/Map/RailGraph.hpp"
#include "Game/Util/MathUtil.hpp"

namespace MR {
    RailGraphIter* createRailGraphIter(const RailGraph* pGraph) {
        return new RailGraphIter(pGraph->getIterator());
    }

    void moveNextNode(RailGraphIter* pIter) {
        pIter->moveNodeNext();
    }

    void moveNodeNearPosition(RailGraphIter* pRailGraphIter, const TVec3f& rVec, f32 f, RailGraphNodeSelecter* pSelector) {
        pRailGraphIter->setNode(getNearNodeIndex(pRailGraphIter->mGraph, rVec, f, pSelector));
    }

    void selectReverseEdge(RailGraphIter* pRailGraphIter) {
        s32 next = pRailGraphIter->mSelectedEdge;
        pRailGraphIter->moveNodeNext();
        pRailGraphIter->selectEdge(next);
    }

    bool isSelectedEdge(const RailGraphIter* pRailGraphIter) {
        return pRailGraphIter->isSelectedEdge();
    }

    bool isWatchedPrevEdge(const RailGraphIter* pRailGraphIter) {
        return pRailGraphIter->isWatchedPrevEdge();
    }

    TVec3f* getCurrentNodePosition(const RailGraphIter* pGraph) {
        return &pGraph->getCurrentNode()->_0;
    }

    TVec3f* getNextNodePosition(const RailGraphIter* pIter) {
        return &pIter->getNextNode()->_0;
    }

    void calcWatchEdgeVector(const RailGraphIter* pIter, TVec3f* pEdge) {
        pEdge->set< f32 >(pIter->getWatchNode()->_0 - pIter->getCurrentNode()->_0);
    }

    void calcWatchEdgeDirection(const RailGraphIter* pRailGraphIter, TVec3f* pVec) {
        calcWatchEdgeVector(pRailGraphIter, pVec);
        MR::normalize(pVec);
    }

    // MR::getNearNodeIndex

    s32 getSelectEdgeArg0(const RailGraphIter* pIter) {
        return pIter->getCurrentEdge()->mPointArg0;
    }

    s32 getSelectEdgeArg1(const RailGraphIter* pIter) {
        return pIter->getCurrentEdge()->mPointArg1;
    }

    s32 getSelectEdgeArg2(const RailGraphIter* pIter) {
        return pIter->getCurrentEdge()->mPointArg2;
    }

    s32 getSelectEdgeArg3(const RailGraphIter* pIter) {
        return pIter->getCurrentEdge()->mPointArg3;
    }

    s32 getWatchEdgeArg7(const RailGraphIter* pIter) {
        return pIter->getWatchEdge()->mPointArg7;
    }

};  // namespace MR
