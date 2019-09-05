#include "MR/RailGraphUtil.h"
#include "MR/MathUtil.h"

namespace MR
{
    void moveNextNode(RailGraphIter *iter)
    {
        iter->moveNodeNext();
    }

    void selectReverseEdge(RailGraphIter *iter)
    {
        s32 edge = iter->mSelectedEdge;
        iter->moveNodeNext();
        iter->selectEdge(edge);
    }

    bool isSelectedEdge(const RailGraphIter *iter)
    {
        return iter->isSelectedEdge();
    }

    bool isWatchedPrevEdge(const RailGraphIter *iter)
    {
        return iter->isWatchedPrevEdge();
    }

    RailGraphNode* getCurrentNodePosition(const RailGraphIter *iter)
    {
        return iter->getCurrentNode();
    }

    RailGraphNode* getNextNodePosition(const RailGraphIter *iter)
    {
        return iter->getNextNode();
    }

    void calcWatchEdgeVector(const RailGraphIter *iter, JGeometry::TVec3<f32> *out)
    {
        RailGraphNode* curNode = iter->getCurrentNode();
        RailGraphNode* watchNode = iter->getWatchNode();

        JGeometry::TVec3<f32> temp;
        temp = watchNode->mPosition - curNode->mPosition;
        out->set(temp);
    }

    void calcWatchEdgeDirection(const RailGraphIter *iter, JGeometry::TVec3<f32> *out)
    {
        calcWatchEdgeVector(iter, out);
        MR::normalize(out);
    }

    s32 getSelectEdgeArg0(const RailGraphIter *iter)
    {
        return iter->getCurrentEdge()->mPointArg0;
    }

    s32 getSelectEdgeArg1(const RailGraphIter *iter)
    {
        return iter->getCurrentEdge()->mPointArg1;
    }

    s32 getSelectEdgeArg2(const RailGraphIter *iter)
    {
        return iter->getCurrentEdge()->mPointArg2;
    }

    s32 getSelectEdgeArg3(const RailGraphIter *iter)
    {
        return iter->getCurrentEdge()->mPointArg3;
    }

    s32 getSelectEdgeArg7(const RailGraphIter *iter)
    {
        return iter->getCurrentEdge()->mPointArg7;
    }
};