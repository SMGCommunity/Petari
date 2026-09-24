#include "Game/Util/RailGraphUtil.hpp"
#include "Game/Map/RailGraph.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "math_types.hpp"

void RailGraphUtil_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace {
    inline void addRailToGraph(RailGraph* pGraph, JMapInfoIter currentRail, const JMapInfo* pPoints) {
        s32 pointCount = pPoints->getNumEntries();
        s32 firstNode;
        s32 previousNode = -1;
        RailGraphEdge edge;

        for (s32 i = 0; i < pointCount; i++) {
            TVec3f position;
            MR::getRailPointPos0(JMapInfoIter(pPoints, i), &position);
            s32 nearNode = MR::getNearNodeIndex(pGraph, position, 100.0f, nullptr);
            s32 node;
            if (nearNode == -1) {
                node = pGraph->addNode(position);
            } else {
                node = nearNode;
            }

            if (previousNode != -1 && node != previousNode) {
                pGraph->connectNodeTwoWay(previousNode, node, &edge);
            }

            if (i == 0) {
                firstNode = node;
            }

            previousNode = node;
            edge.setArgs(pPoints, i);
        }

        if (MR::isLoopRailPathIter(currentRail) && previousNode != -1 && firstNode != previousNode) {
            pGraph->connectNodeTwoWay(previousNode, firstNode, &edge);
        }
    }
}  // namespace

namespace MR {
    RailGraph* createRailGraphFromJMap(const JMapInfoIter& rIter) {
        RailGraph* pGraph = new RailGraph;
        const JMapInfo* pRailPoints = nullptr;
        JMapInfoIter railIter;
        bool hasRail = true;
        getRailInfo(&railIter, &pRailPoints, rIter);

        while (hasRail) {
            ::addRailToGraph(pGraph, railIter, pRailPoints);

            hasRail = getNextLinkRailInfo(&railIter, &pRailPoints, railIter);
        }

        return pGraph;
    }

    void RailGraphUtil_FORCE_EMIT(const RailGraphIter& rIter) {
        RailGraphIter copy(rIter);
    }

    RailGraphIter* createRailGraphIter(const RailGraph* pGraph) {
        return new RailGraphIter(pGraph->getIterator());
    }

    void moveNextNode(RailGraphIter* pIter) {
        pIter->moveNodeNext();
    }

    void moveNodeNearPosition(RailGraphIter* pRailGraphIter, const TVec3f& rPosition, f32 maxDistance, RailGraphNodeSelecter* pSelector) {
        pRailGraphIter->setNode(getNearNodeIndex(pRailGraphIter->mGraph, rPosition, maxDistance, pSelector));
    }

    void selectReverseEdge(RailGraphIter* pRailGraphIter) {
        s32 next = pRailGraphIter->mSelectedEdge;
        pRailGraphIter->moveNodeNext();
        pRailGraphIter->selectEdge(next);
    }

    void calcNextEdgeVector(const RailGraphIter* pIter, TVec3f* pEdge) {
        pEdge->set(pIter->getNextNode()->_0 - pIter->getCurrentNode()->_0);
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
        pEdge->set(pIter->getWatchNode()->_0 - pIter->getCurrentNode()->_0);
    }

    void calcWatchEdgeDirection(const RailGraphIter* pRailGraphIter, TVec3f* pVec) {
        calcWatchEdgeVector(pRailGraphIter, pVec);
        MR::normalize(pVec);
    }

    s32 getNearNodeIndex(const RailGraph* pGraph, const TVec3f& rPosition, f32 maxDistance, RailGraphNodeSelecter* pSelector) {
        if (maxDistance < 0.0f) {
            maxDistance = FLOAT_MAX;
        }

        f32 nearestDistance = maxDistance;
        s32 nearestNode = -1;
        s32 nodeCount = pGraph->_8;
        RailGraphIter iter = pGraph->getIterator();
        for (s32 i = 0; i < nodeCount; i++) {
            if (pSelector != nullptr) {
                iter.setNode(i);
                if (!pSelector->isSatisfy(iter)) {
                    continue;
                }
            }

            f32 distance = rPosition.distance(pGraph->getNode(i)->_0);
            if (distance < nearestDistance) {
                nearestDistance = distance;
                nearestNode = i;
            }
        }

        return nearestNode;
    }

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
}  // namespace MR
