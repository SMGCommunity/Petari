#include "Util/MathUtil.h"
#include "Util/RailUtil.h"
#include "Util/LiveActorUtil.h"
#include "Util/PlayerUtil.h"
#include "LiveActor/LiveActor.h"
#include "JSystem/JMath/JMath.h"

#include <revolution.h>

namespace MR
{
    void moveTransToOtherActorRailPos(LiveActor *pActor, const LiveActor *pRailActor)
    {
        pActor->mTranslation.set<f32>(pRailActor->mRailRider->mCurrentPos);
    }

    void moveTransToCurrentRailPos(LiveActor *pActor)
    {
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void moveCoord(LiveActor *pActor, f32 speed)
    {
        pActor->mRailRider->setSpeed(__fabs(speed));
        pActor->mRailRider->move();
    }

    void moveCoordAndTransToRailPoint(LiveActor *pActor, s32 pointNum)
    {
        RailRider* rider = pActor->mRailRider;

        f32 coord = rider->getPointCoord(pointNum);
        rider->setCoord(coord);
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    s32 moveCoordAndCheckPassPointNo(LiveActor *pActor, f32 speed)
    {
        s32 nextPointBeforeMove = pActor->mRailRider->getNextPointNo();
        pActor->mRailRider->setSpeed(__fabs(speed));
        pActor->mRailRider->move();
        s32 nextPointAfterMove = pActor->mRailRider->getNextPointNo();
        s32 ret = -1;

        if (nextPointBeforeMove != nextPointAfterMove)
        {
            ret = nextPointBeforeMove;
        }

        return ret;
    }

    s32 moveCoordAndFollowTransAndCheckPassPointNo(LiveActor *pActor)
    {
        s32 nextPointBeforeMove = pActor->mRailRider->getNextPointNo();
        MR::moveCoordAndFollowTrans(pActor);
        s32 nextPointAfterMove = pActor->mRailRider->getNextPointNo();
        s32 ret = -1;

        if (nextPointBeforeMove != nextPointAfterMove)
        {
            ret = nextPointBeforeMove;
        }

        return ret;
    }

    void moveCoordToRailPoint(LiveActor *pActor, s32 pointNo)
    {
        RailRider* rider = pActor->mRailRider;
        f32 coord = rider->getPointCoord(pointNo);
        rider->setCoord(coord);
    }

    void moveCoordToNearestPos(LiveActor *pActor, const JGeometry::TVec3<f32> &point)
    {
        pActor->mRailRider->moveToNearestPos(point);
    }

    void moveCoordAndFollowTrans(LiveActor *pActor, f32 speed)
    {
        MR::moveCoord(pActor, speed);
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void moveCoordToStartPos(LiveActor *pActor)
    {
        pActor->mRailRider->setCoord(0.0f);
    }

    void moveCoordToEndPos(LiveActor *pActor)
    {
        RailRider* rider = pActor->mRailRider;
        f32 length = rider->getTotalLength();
        rider->setCoord(length);
    }

    void moveCoordAndTransToNearestRailPos(LiveActor *pActor)
    {
        pActor->mRailRider->moveToNearestPos(pActor->mTranslation);
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void moveCoordAndTransToNearestRailPos(LiveActor *pActor, const JGeometry::TVec3<f32> &pos)
    {
        pActor->mRailRider->moveToNearestPos(pos);
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void moveCoordAndTransToNearestRailPoint(LiveActor *pActor)
    {
        pActor->mRailRider->moveToNearestPoint(pActor->mTranslation);
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void moveCoordAndTransToNextPoint(LiveActor *pActor)
    {
        pActor->mRailRider->moveToNextPoint();
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void moveCoordAndTransToRailStartPoint(LiveActor *pActor)
    {
        pActor->mRailRider->setCoord(0.0f);
        pActor->mTranslation.set<f32>(pActor->mRailRider->mCurrentPos);
    }

    void setRailDirectionToStart(LiveActor *pActor)
    {
        RailRider* rider = pActor->mRailRider;
        if (rider->_C)
        {
            rider->reverse();
        }
    }

    void setRailDirectionToEnd(LiveActor *pActor)
    {
        RailRider* rider = pActor->mRailRider;
        if (!rider->_C)
        {
            rider->reverse();
        }
    }

    void setRailDirectionCloseToCoord(LiveActor *pActor, f32 coord)
    {
        f32 length = pActor->mRailRider->mTotalLength;

        if (pActor->mRailRider->isLoop())
        {
            f32 totalLength = pActor->mRailRider->getTotalLength();

            f32 f0, f1, f2;

            if (length < coord)
            {
                f0 = totalLength - coord;
                f1 = coord - length;
                f2 = length + f0;
            }
            else
            {
                f0 = totalLength - length;
                f2 = length - coord;
                f1 = coord + f0;
            }

            if (f2 < f1)
            {
                MR::setRailDirectionToStart(pActor);
            }
            else
            {
                MR::setRailDirectionToEnd(pActor);  
            }
        }
        else
        {
            if (length < coord)
            {
                MR::setRailDirectionToEnd(pActor);
            }
            else
            {
                MR::setRailDirectionToStart(pActor);
            }
        }
    }

    void setRailDirectionCloseToNearestPos(LiveActor *pActor, const JGeometry::TVec3<f32> &pos)
    {
        f32 nearestCoord = pActor->mRailRider->calcNearestPos(pos);
        MR::setRailDirectionCloseToCoord(pActor, nearestCoord);
    }
    
    void reverseRailDirection(LiveActor *pActor)
    {
        pActor->mRailRider->reverse();
    }

    // todo -- needs some work
    f32 calcMovingDirectionAlongRail(LiveActor *pActor, JGeometry::TVec3<f32> *pVec, const JGeometry::TVec3<f32> &otherVec, f32 unk, bool unkbool, bool *pBool)
    {
        JGeometry::TVec3<f32> railPosition;
        JGeometry::TVec3<f32> railDirection;
        f32 coord = MR::calcNearestRailPosAndDirection(&railPosition, &railDirection, pActor, otherVec);
        pActor->mRailRider->setCoord(coord);

        if (pActor->mRailRider->isLoop())
        {
            if (pActor->mRailRider->isReachedGoal())
            {
                pActor->mRailRider->reverse();

                if (pBool)
                {
                    *pBool = 1;
                }
            }
            else
            {
                if (pBool)
                {
                    *pBool = 0;
                }
            }
        }

        if (!pActor->mRailRider->_C)
        {
            JGeometry::TVec3<f32> positionNeg;

            // takes X, Y, and Z from the position, and then negates it
            __asm
            {
                lfs f0, 0x34(r1)
                psq_l f1, 0(r3), 0, 0
                fneg f0, f0
                ps_neg f1, f1
                stfs f0, 0x1C(r1)
                psq_st f1, 0(r4), 0, 0
            }

            railPosition = positionNeg;
        }

        JGeometry::TVec3<f32> position(railPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&position, (Vec*)&otherVec, (Vec*)&position);

        if (unkbool)
        {
            MR::vecKillElement(position, pActor->mGravity, &position);
            MR::vecKillElement(railDirection, pActor->mGravity, &railDirection);
        }

        f32 ret = PSVECMag((Vec*)&position);
        f32 f4 = 1.0f / unk;
        position.x *= f4;
        position.y *= f4;
        position.z *= f4;

        JGeometry::TVec3<f32> vec(position);
        vec.add(railDirection);
        pVec->set<f32>(vec);
        MR::normalize(pVec);
        return ret;
    }

    f32 calcMovingDirectionAlongRailH(LiveActor *pActor, JGeometry::TVec3<f32> *direction, const JGeometry::TVec3<f32> &vec, f32 unk, bool *ptr)
    {
        return MR::calcMovingDirectionAlongRail(pActor, direction, vec, unk, 1, ptr);
    }

    // calcRailClippingInfo

    void initAndSetRailClipping(JGeometry::TVec3<f32> *vecPtr, LiveActor *pActor, f32 unk1, f32 unk2)
    {
        f32 clippingDist = 0.0f;
        MR::calcRailClippingInfo(vecPtr, &clippingDist, pActor, unk1, unk2);
        MR::setClippingTypeSphere(pActor, clippingDist, *vecPtr);
    }

    f32 calcNearestRailCoord(const LiveActor *pActor, const JGeometry::TVec3<f32> &vec)
    {
        return pActor->mRailRider->calcNearestPos(vec);
    }

    f32 calcNearestRailPos(JGeometry::TVec3<f32> *pOut, const LiveActor *pActor, const JGeometry::TVec3<f32> &pos)
    {
        f32 railPos = pActor->mRailRider->calcNearestPos(pos);
        pActor->mRailRider->calcPosAtCoord(pOut, railPos);
        return railPos;
    }

    f32 calcNearestRailDirection(JGeometry::TVec3<f32> *pOut, const LiveActor *pActor, const JGeometry::TVec3<f32> &pos)
    {
        f32 nearestPos = pActor->mRailRider->calcNearestPos(pos);
        pActor->mRailRider->calcDirectionAtCoord(pOut, nearestPos);
        return nearestPos;
    }

    f32 calcNearestRailPosAndDirection(JGeometry::TVec3<f32> *pPosition, JGeometry::TVec3<f32> *pDirection, const LiveActor *pActor, const JGeometry::TVec3<f32> &pos)
    {
        f32 nearestPos = pActor->mRailRider->calcNearestPos(pos);
        pActor->mRailRider->calcDirectionAtCoord(pDirection, nearestPos);
        pActor->mRailRider->calcPosAtCoord(pPosition, nearestPos);
        return nearestPos;
    }
    
    f32 calcRailPosNearestPlayer(JGeometry::TVec3<f32> *pPosition, const LiveActor *pActor)
    {
        JGeometry::TVec3<f32>* playerPos = MR::getPlayerPos();
        return MR::calcNearestRailPos(pPosition, pActor, *playerPos);
    }
};