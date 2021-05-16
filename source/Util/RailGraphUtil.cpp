#include "Util/RailGraphUtil.h"
#include "Util/MathUtil.h"

namespace MR
{
    void moveNextNode(RailGraphIter *pIter)
    {
        pIter->moveNodeNext();
    }

    void selectReverseEdge(RailGraphIter *pIter)
    {
        s32 edge = pIter->mSelectedEdge;
        pIter->moveNodeNext();
        pIter->selectEdge(edge);
    }

    bool isSelectedEdge(const RailGraphIter *pIter)
    {
        return pIter->isSelectedEdge();
    }

    bool isWatchedPrevEdge(const RailGraphIter *pIter)
    {
        return pIter->isWatchedPrevEdge();
    }

    RailGraphNode* getCurrentNodePosition(const RailGraphIter *pIter)
    {
        return pIter->getCurrentNode();
    }

    RailGraphNode* getNextNodePosition(const RailGraphIter *pIter)
    {
        return pIter->getNextNode();
    }

    void calcWatchEdgeVector(const RailGraphIter *pIter, JGeometry::TVec3<f32> *pOut)
    {
        RailGraphNode *pCurNode = pIter->getCurrentNode();
        RailGraphNode *pWatchNode = pIter->getWatchNode();

        JGeometry::TVec3<f32> temp;
        temp = pWatchNode->mPosition - pCurNode->mPosition;
        pOut->set(temp);
    }

    void calcWatchEdgeDirection(const RailGraphIter *pIter, JGeometry::TVec3<f32> *pOut)
    {
        calcWatchEdgeVector(pIter, pOut);
        MR::normalize(pOut);
    }

    s32 getSelectEdgeArg0(const RailGraphIter *pIter)
    {
        return pIter->getCurrentEdge()->mPointArg0;
    }

    s32 getSelectEdgeArg1(const RailGraphIter *pIter)
    {
        return pIter->getCurrentEdge()->mPointArg1;
    }

    s32 getSelectEdgeArg2(const RailGraphIter *pIter)
    {
        return pIter->getCurrentEdge()->mPointArg2;
    }

    s32 getSelectEdgeArg3(const RailGraphIter *pIter)
    {
        return pIter->getCurrentEdge()->mPointArg3;
    }

    s32 getSelectEdgeArg7(const RailGraphIter *pIter)
    {
        return pIter->getCurrentEdge()->mPointArg7;
    }
};