#include "Game/Util/RailUtil.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JMath/JMath.hpp>
#include <revolution/mtx.h>

namespace MR {
    void moveTransToOtherActorRailPos(LiveActor* pActor, const LiveActor* pOtherActor) {
        pActor->mPosition.set(getRailPos(pOtherActor));
    }

    void moveTransToCurrentRailPos(LiveActor* pActor) {
        pActor->mPosition.set(getRailPos(pActor));
    }

    void moveCoord(LiveActor* pActor, f32 speed) {
        setRailCoordSpeed(pActor, speed);
        moveRailRider(pActor);
    }

    void moveCoordAndTransToRailPoint(LiveActor* pActor, s32 index) {
        moveCoordToRailPoint(pActor, index);
        moveTransToCurrentRailPos(pActor);
    }

    s32 moveCoordAndCheckPassPointNo(LiveActor* pActor, f32 speed) {
        s32 prevNextPointNo = getNextRailPointNo(pActor);
        setRailCoordSpeed(pActor, speed);
        moveRailRider(pActor);
        return prevNextPointNo == getNextRailPointNo(pActor) ? -1 : prevNextPointNo;
    }

    s32 moveCoordAndFollowTransAndCheckPassPointNo(LiveActor* pActor) {
        s32 prevNextPointNo = getNextRailPointNo(pActor);
        moveCoordAndFollowTrans(pActor);
        return prevNextPointNo == getNextRailPointNo(pActor) ? -1 : prevNextPointNo;
    }

    void moveCoordToRailPoint(LiveActor* pActor, s32 index) {
        RailRider* railRider = pActor->mRailRider;
        railRider->setCoord(railRider->getPointCoord(index));
        // setRailCoord(pActor, getRailPointCoord(pActor, index)); // doesnt match
    }

    void moveCoordToNearestPos(LiveActor* pActor, const TVec3f& rPos) {
        pActor->mRailRider->moveToNearestPos(rPos);
    }

    void moveCoordAndFollowTrans(LiveActor* pActor, f32 speed) {
        moveCoord(pActor, speed);
        moveTransToCurrentRailPos(pActor);
    }

    void moveCoordAndFollowTrans(LiveActor* pActor) {
        moveRailRider(pActor);
        moveTransToCurrentRailPos(pActor);
    }

    void moveCoordToStartPos(LiveActor* pActor) {
        setRailCoord(pActor, 0.0f);
    }

    void moveCoordToEndPos(LiveActor* pActor) {
        RailRider* railRider = pActor->mRailRider;
        railRider->setCoord(railRider->getTotalLength());
        // setRailCoord(pActor, getRailTotalLength(pActor)); doesnt match??
    }

    void moveCoordAndTransToNearestRailPos(LiveActor* pActor) {
        pActor->mRailRider->moveToNearestPos(pActor->mPosition);
        moveTransToCurrentRailPos(pActor);
    }

    void moveCoordAndTransToNearestRailPos(LiveActor* pActor, const TVec3f& rPos) {
        pActor->mRailRider->moveToNearestPos(rPos);
        moveTransToCurrentRailPos(pActor);
    }

    void moveCoordAndTransToNearestRailPoint(LiveActor* pActor) {
        pActor->mRailRider->moveToNearestPoint(pActor->mPosition);
        moveTransToCurrentRailPos(pActor);
    }

    void moveCoordAndTransToNextPoint(LiveActor* pActor) {
        pActor->mRailRider->moveToNextPoint();
        moveTransToCurrentRailPos(pActor);
    }

    void moveCoordAndTransToRailStartPoint(LiveActor* pActor) {
        moveCoordToStartPos(pActor);
        moveTransToCurrentRailPos(pActor);
    }

    void setRailDirectionToStart(LiveActor* pActor) {
        if (!pActor->mRailRider->mIsNotReverse) {
            return;
        }
        reverseRailDirection(pActor);
    }

    void setRailDirectionToEnd(LiveActor* pActor) {
        if (pActor->mRailRider->mIsNotReverse) {
            return;
        }
        reverseRailDirection(pActor);
    }

    void setRailDirectionCloseToCoord(LiveActor* pActor, f32 coord) {
        f32 curCoord = getRailCoord(pActor);
        if (isLoopRail(pActor)) {
            f32 length = getRailTotalLength(pActor);
            f32 diff1, diff2;
            if (curCoord < coord) {
                diff1 = coord - curCoord;
                diff2 = curCoord + (length - coord);
            } else {
                diff2 = curCoord - coord;
                diff1 = coord + (length - curCoord);
            }

            if (diff2 < diff1) {
                setRailDirectionToStart(pActor);
            } else {
                setRailDirectionToEnd(pActor);
            }
        } else {
            if (curCoord < coord) {
                setRailDirectionToEnd(pActor);
            } else {
                setRailDirectionToStart(pActor);
            }
        }
    }

    void setRailDirectionCloseToNearestPos(LiveActor* pActor, const TVec3f& rPos) {
        setRailDirectionCloseToCoord(pActor, calcNearestRailCoord(pActor, rPos));
    }

    void reverseRailDirection(LiveActor* pActor) {
        pActor->mRailRider->reverse();
    }

    void calcMovingDirectionAlongRail(LiveActor* pActor, TVec3f* pDir, const TVec3f& rPos, f32 coord, bool horizontal, bool* pReachedGoal) {
        // FIXME : improper float register store, float regswaps
        // https://decomp.me/scratch/xYffl

        TVec3f pos, dir;
        calcNearestRailPosAndDirection(&pos, &dir, pActor, rPos);
        setRailCoord(pActor, coord);

        if (!isLoopRail(pActor) && isRailReachedGoal(pActor)) {
            reverseRailDirection(pActor);
            if (pReachedGoal != nullptr) {
                *pReachedGoal = true;
            }
        } else {
            if (pReachedGoal != nullptr) {
                *pReachedGoal = false;
            }
        }

        if (!isRailGoingToEnd(pActor)) {
            dir = dir.negateInline();
        }

        TVec3f fromInPos(pos - rPos);

        if (horizontal) {
            vecKillElement(fromInPos, pActor->mGravity, &fromInPos);
            vecKillElement(dir, pActor->mGravity, &dir);
        }

        fromInPos.mult(1.0f / fromInPos.length());

        pDir->set(fromInPos.addOperatorInLine(dir));
        normalizeOrZero(pDir);
    }

    void calcMovingDirectionAlongRailH(LiveActor* pActor, TVec3f* pDir, const TVec3f& rPos, f32 coord, bool* pReachedGoal) {
        calcMovingDirectionAlongRail(pActor, pDir, rPos, coord, true, pReachedGoal);
    }

    void calcRailClippingInfo(TVec3f* pCenter, f32* pRadius, const LiveActor* pActor, f32 delta, f32 padding) {
        // FIXME : improper stack order
        // https://decomp.me/scratch/2DXZy

        f32 totalLength = getRailTotalLength(pActor);
        s32 numPoints = totalLength / delta;
        TBox3f box;
        pActor->mRailRider->calcPosAtCoord(&box.f, 0.0f);
        pActor->mRailRider->calcPosAtCoord(&box.i, totalLength);

        TVec3f maxBound(MR::max(box.i.x, box.f.x), MR::max(box.i.y, box.f.y), MR::max(box.i.z, box.f.z));
        TVec3f minBound(MR::min(box.i.x, box.f.x), MR::min(box.i.y, box.f.y), MR::min(box.i.z, box.f.z));

        TBox3f clipBox;
        clipBox.set(minBound, maxBound);

        TVec3f pos;
        for (s32 idx = 1; idx < numPoints; idx++) {
            pActor->mRailRider->calcPosAtCoord(&pos, idx * delta);
            clipBox.extend(pos);
        }

        TVec3f padVec;
        padVec.set(padding);
        JMathInlineVEC::PSVECSubtract(clipBox.i, padVec, clipBox.i);  // TODO: smells like clipMin.sub(offsetVec)
        clipBox.f.add(padVec);
        JMAVECLerp(clipBox.f, clipBox.i, pCenter, 0.5f);

        TVec3f diameterVec;
        JMathInlineVEC::PSVECSubtract(clipBox.f, clipBox.i, diameterVec);
        *pRadius = diameterVec.length() * 0.5f;
    }

    void initAndSetRailClipping(TVec3f* pCenter, LiveActor* pActor, f32 delta, f32 padding) {
        f32 radius = 0.0f;
        calcRailClippingInfo(pCenter, &radius, pActor, delta, padding);
        setClippingTypeSphere(pActor, radius, pCenter);
    }

    f32 calcNearestRailCoord(const LiveActor* pActor, const TVec3f& rPos) {
        return pActor->mRailRider->calcNearestPos(rPos);
    }

    f32 calcNearestRailPos(TVec3f* pOutPos, const LiveActor* pActor, const TVec3f& rPos) {
        f32 coord = calcNearestRailCoord(pActor, rPos);
        pActor->mRailRider->calcPosAtCoord(pOutPos, coord);
        return coord;
    }

    f32 calcNearestRailDirection(TVec3f* pOutDir, const LiveActor* pActor, const TVec3f& rPos) {
        f32 coord = calcNearestRailCoord(pActor, rPos);
        pActor->mRailRider->calcDirectionAtCoord(pOutDir, coord);
        return coord;
    }

    f32 calcNearestRailPosAndDirection(TVec3f* pOutPos, TVec3f* pOutDir, const LiveActor* pActor, const TVec3f& rPos) {
        f32 coord = calcNearestRailCoord(pActor, rPos);
        pActor->mRailRider->calcDirectionAtCoord(pOutDir, coord);
        pActor->mRailRider->calcPosAtCoord(pOutPos, coord);
        return coord;
    }

    f32 calcRailPosNearestPlayer(TVec3f* pOutPos, const LiveActor* pActor) {
        return calcNearestRailPos(pOutPos, pActor, *MR::getPlayerPos());
    }
}  // namespace MR

namespace {
    void updateBoundingBox(const RailRider* pRailRider, TBox3f* pBox) {
        TVec3f pos(pRailRider->mCurPos);

        pBox->i.x = pBox->i.x >= pos.x ? pos.x : pBox->i.x;
        pBox->i.y = MR::min(pBox->i.y, pos.y);
        pBox->i.z = MR::min(pBox->i.z, pos.z);

        pBox->f.x = pBox->f.x >= pos.x ? pBox->f.x : pos.x;
        pBox->f.y = MR::max(pBox->f.y, pos.y);
        pBox->f.z = MR::max(pBox->f.z, pos.z);
    }
}  // namespace

namespace MR {
    void calcBoundingBox(RailRider* pRailRider, TBox3f* pBox, f32 delta) {
        f32 coord = pRailRider->mCoord;
        pRailRider->setCoord(0.0f);

        pBox->i.set(pRailRider->mCurPos);
        pBox->f.set(pBox->i);

        s32 numPoints = pRailRider->getTotalLength() / delta;

        for (s32 idx = 1; idx < numPoints; idx++) {
            pRailRider->setCoord(delta * idx);
            updateBoundingBox(pRailRider, pBox);
        }

        pRailRider->setCoord(pRailRider->getTotalLength());
        updateBoundingBox(pRailRider, pBox);
        pRailRider->setCoord(coord);
    }

    void calcBoundingBox(const LiveActor* pActor, TBox3f* pBox, f32 delta) {
        calcBoundingBox(pActor->mRailRider, pBox, delta);
    }

    f32 calcDifferenceRailCoord(const LiveActor* pActor, f32 coord1, f32 coord2) {
        f32 diff = coord2 - coord1;
        if (isLoopRail(pActor)) {
            f32 length = getRailTotalLength(pActor);
            if (diff > length / 2) {
                diff -= length;
            } else if (diff < -length / 2) {
                diff += length;
            }
        }

        if (isRailGoingToEnd(pActor)) {
            return diff;
        } else {
            return -diff;
        }
    }

    f32 calcDifferenceRailCoord(const LiveActor* pActor, f32 coord) {
        calcDifferenceRailCoord(pActor, getRailCoord(pActor), coord);
    }

    f32 calcDistanceHorizonToCurrentPos(const LiveActor* pActor) {
        // FIXME : reg scheduling and stack order
        // https://decomp.me/scratch/KkQjM

        TVec3f diff(getRailPos(pActor) - pActor->mPosition);
        TVec3f horizontal;
        horizontal.rejection(diff, pActor->mGravity);
        return PSVECMag(horizontal);
    }

    void calcRailPosAtCoord(TVec3f* pPos, const LiveActor* pActor, f32 coord) {
        pActor->mRailRider->calcPosAtCoord(pPos, coord);
    }

    void calcRailPosFrontCoord(TVec3f* pPos, const LiveActor* pActor, f32 frontDist) {
        // FIXME: MR::clamp inline is finnicky
        // https://decomp.me/scratch/UTDeK

        f32 coord = getRailCoord(pActor);
        coord = isRailGoingToEnd(pActor) ? coord + frontDist : coord - frontDist;

        if (isLoopRail(pActor)) {
            coord = repeat(coord, 0.0f, getRailTotalLength(pActor));
        } else {
            // this is clamp(coord, 0.0f, getRailTotalLength(pActor));
            f32 length = getRailTotalLength(pActor);
            coord = coord < 0.0f ? 0.0f : coord > length ? length : coord;
        }

        calcRailPosAtCoord(pPos, pActor, coord);
    }

    void calcRailDirectionAtCoord(TVec3f* pDir, const LiveActor* pActor, f32 coord) {
        pActor->mRailRider->calcDirectionAtCoord(pDir, coord);
    }

    void calcRailPosAndDirectionAtCoord(TVec3f* pPos, TVec3f* pDir, const LiveActor* pActor, f32 coord) {
        calcRailPosAtCoord(pPos, pActor, coord);
        calcRailDirectionAtCoord(pDir, pActor, coord);
    }

    void calcRailStartPos(TVec3f* pPos, const LiveActor* pActor) {
        calcRailPosAtCoord(pPos, pActor, 0.0f);
    }

    void calcRailEndPos(TVec3f* pPos, const LiveActor* pActor) {
        calcRailPosAtCoord(pPos, pActor, getRailTotalLength(pActor));
    }

    void calcRailPointPos(TVec3f* pPos, const LiveActor* pActor, int index) {
        pActor->mRailRider->copyPointPos(pPos, index);
    }

    void calcRailStartPointPos(TVec3f* pPos, const LiveActor* pActor) {
        calcRailPointPos(pPos, pActor, 0);
    }

    void calcRailEndPointPos(TVec3f* pPos, const LiveActor* pActor) {
        calcRailPointPos(pPos, pActor, getRailPointNum(pActor) - 1);
    }

    void calcRailStartPointDirection(TVec3f* pDir, const LiveActor* pActor) {
        pActor->mRailRider->calcDirectionAtCoord(pDir, 0.0f);
    }

    void calcRailEndPointDirection(TVec3f* pDir, const LiveActor* pActor) {
        pActor->mRailRider->calcDirectionAtCoord(pDir, getRailTotalLength(pActor));
    }

    void calcRailStartPointPosAndDirection(TVec3f* pPos, TVec3f* pDir, const LiveActor* pActor) {
        calcRailStartPointPos(pPos, pActor);
        calcRailStartPointDirection(pDir, pActor);
    }

    void calcRailEndPointPosAndDirection(TVec3f* pPos, TVec3f* pDir, const LiveActor* pActor) {
        calcRailEndPointPos(pPos, pActor);
        calcRailEndPointDirection(pDir, pActor);
    }

    void calcDistanceToNextRailPoint(const LiveActor* pActor, f32* pDist) {
        // FIXME: improper stack size, acces, and load order of pActor->mRailRider in r3
        // https://decomp.me/scratch/wgqFG

        RailRider* railRider = pActor->mRailRider;
        f32 nextPointCoord = railRider->getNextPointCoord();
        if (isNearZero(nextPointCoord)) {
            if (isRailGoingToEnd(pActor)) {
                f32 coord = railRider->mCoord;
                *pDist = getRailTotalLength(pActor) - coord;

            } else {
                f32 coord = railRider->mCoord;
                *pDist = coord;
            }
        } else {
            f32 coord = railRider->mCoord;
            *pDist = __fabsf(pActor->mRailRider->getNextPointCoord() - coord);
        }
    }

    void calcDistanceToCurrentAndNextRailPoint(const LiveActor* pActor, f32* pCurrDist, f32* pNextDist) {
        // FIXME : regswap and improper re-load of pActor->mRailRider
        // https://decomp.me/scratch/Z1FEl

        RailRider* railRider = pActor->mRailRider;
        f32 currPointCoord = railRider->getCurrentPointCoord();
        f32 nextPointCoord = railRider->getNextPointCoord();

        if (isNearZero(currPointCoord)) {
            if (isRailGoingToEnd(pActor)) {
                f32 coord = railRider->mCoord;
                *pCurrDist = coord;

            } else {
                f32 coord = railRider->mCoord;
                *pCurrDist = railRider->getTotalLength() - coord;
            }
        } else {
            f32 coord = railRider->mCoord;
            *pCurrDist = __fabsf(coord - currPointCoord);
        }

        if (isNearZero(nextPointCoord)) {
            if (isRailGoingToEnd(pActor)) {
                f32 coord = railRider->mCoord;
                *pNextDist = railRider->getTotalLength() - coord;
            } else {
                f32 coord = railRider->mCoord;
                *pNextDist = coord;
            }
        } else {
            f32 coord = railRider->mCoord;
            *pNextDist = __fabsf(railRider->getNextPointCoord() - coord);
        }
    }

    void calcRailRateToNextPoint(f32* pRate, const LiveActor* pActor) {
        RailRider* railRider = pActor->mRailRider;
        f32 currPointCoord = railRider->getCurrentPointCoord();
        f32 nextPointCoord = railRider->getNextPointCoord();
        f32 coord = railRider->mCoord;

        if (nextPointCoord == currPointCoord) {
            *pRate = 0.0f;
        } else {
            *pRate = (coord - currPointCoord) / (nextPointCoord - currPointCoord);
        }
    }

    bool isExistRail(const LiveActor* pActor) {
        return pActor->mRailRider != nullptr;
    }

    s32 getRailPointNum(const LiveActor* pActor) {
        return pActor->mRailRider->getPointNum();
    }

    s32 getCurrentRailPointNo(const LiveActor* pActor) {
        return pActor->mRailRider->mCurPoint;
    }

    s32 getNextRailPointNo(const LiveActor* pActor) {
        return pActor->mRailRider->getNextPointNo();
    }

    f32 getRailTotalLength(const LiveActor* pActor) {
        return pActor->mRailRider->getTotalLength();
    }

    f32 getRailPartLength(const LiveActor* pActor, int index) {
        return pActor->mRailRider->getPartLength(index);
    }

    f32 getRailPointCoord(const LiveActor* pActor, int index) {
        return pActor->mRailRider->getPointCoord(index);
    }

    const TVec3f& getRailPos(const LiveActor* pActor) {
        return pActor->mRailRider->mCurPos;
    }

    const TVec3f& getRailPointPosStart(const LiveActor* pActor) {
        return pActor->mRailRider->mStartPos;
    }

    const TVec3f& getRailPointPosEnd(const LiveActor* pActor) {
        return pActor->mRailRider->mEndPos;
    }

    const TVec3f& getRailDirection(const LiveActor* pActor) {
        return pActor->mRailRider->mCurDirection;
    }

    f32 getRailCoord(const LiveActor* pActor) {
        return pActor->mRailRider->mCoord;
    }

    f32 getRailCoordSpeed(const LiveActor* pActor) {
        return pActor->mRailRider->mSpeed;
    }

    void setRailCoord(LiveActor* pActor, f32 coord) {
        pActor->mRailRider->setCoord(coord);
    }

    void setRailCoordSpeed(LiveActor* pActor, f32 speed) {
        pActor->mRailRider->setSpeed(__fabsf(speed));
    }

    void accelerateRailCoordSpeed(LiveActor* pActor, f32 accel) {
        f32 speed = pActor->mRailRider->mSpeed + accel;
        if (speed < 0.0f) {
            speed = -speed;
            pActor->mRailRider->reverse();
        }
        pActor->mRailRider->setSpeed(speed);
    }

    void adjustmentRailCoordSpeed(LiveActor* pActor, f32 target, f32 rate) {
        f32 speed = pActor->mRailRider->mSpeed;

        if (__fabsf(speed - target) < rate) {
            speed = target;
        } else if (speed < target) {
            speed += rate;
        } else if (speed > target) {
            speed -= rate;
        }

        pActor->mRailRider->setSpeed(speed);
    }

    void slowDownRailCoordSpeed(LiveActor* pActor, f32 ratio) {
        pActor->mRailRider->setSpeed(pActor->mRailRider->mSpeed * ratio);
    }

    void moveRailRider(LiveActor* pActor) {
        pActor->mRailRider->move();
    }

    bool isLoopRail(const LiveActor* pActor) {
        return pActor->mRailRider->isLoop();
    }

    bool isRailReachedGoal(const LiveActor* pActor) {
        return pActor->mRailRider->isReachedGoal();
    }

    bool isRailReachedNearGoal(const LiveActor* pActor, f32 range) {
        if (isRailGoingToEnd(pActor)) {
            f32 length = getRailTotalLength(pActor);
            if (length - range <= getRailCoord(pActor)) {
                return true;
            }
        } else {
            if (getRailCoord(pActor) < range) {
                return true;
            }
        }
        return false;
    }

    bool isRailReachedEdge(const LiveActor* pActor) {
        return pActor->mRailRider->isReachedEdge();
    }

    bool isRailGoingToEnd(const LiveActor* pActor) {
        return pActor->mRailRider->mIsNotReverse;
    }

    bool isRailReachedNearNextPoint(const LiveActor* pActor, f32 range) {
        f32 coord = getRailPointCoord(pActor, getNextRailPointNo(pActor));
        if (isRailGoingToEnd(pActor)) {
            return coord - getRailCoord(pActor) < range;
        } else {
            return getRailCoord(pActor) - coord < range;
        }
    }

    bool isRailReachedHorizonCurrentPos(const LiveActor* pActor, f32 range) {
        return MR::calcDistanceHorizonToCurrentPos(pActor) < range;
    }

    bool getRailArg0NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg0", pArg);
    }

    bool getRailArg1NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg1", pArg);
    }

    bool getRailArg2NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg2", pArg);
    }

    bool getRailArg3NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg3", pArg);
    }

    bool getRailArg4NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg4", pArg);
    }

    bool getRailArg5NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg5", pArg);
    }

    bool getRailArg6NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg6", pArg);
    }

    bool getRailArg7NoInit(const LiveActor* pActor, s32* pArg) {
        return pActor->mRailRider->getRailArgNoInit("path_arg7", pArg);
    }

    bool getRailArg1WithInit(const RailRider* pRailRider, s32* pArg) {
        return pRailRider->getRailArgWithInit("path_arg1", pArg);
    }

}  // namespace MR

namespace {
    bool getRailPointArgF32WithInit(const LiveActor* pActor, s32 argNum, s32 index, f32* pArg) NO_INLINE {
        RailRider* railRider = pActor->mRailRider;
        s32 arg = *pArg;
        bool b = railRider->getPointArgS32WithInit(getRailPointArgName(argNum), &arg, index);
        *pArg = arg;
        return b;
    }

    bool getRailPointArgF32NoInit(const LiveActor* pActor, s32 argNum, s32 index, f32* pArg) NO_INLINE {
        RailRider* railRider = pActor->mRailRider;
        s32 arg = *pArg;
        bool b = railRider->getPointArgS32NoInit(getRailPointArgName(argNum), &arg, index);
        *pArg = arg;
        return b;
    }

    bool getRailCurrentPointArgF32WithInit(const LiveActor* pActor, s32 argNum, f32* pArg) NO_INLINE {
        RailRider* railRider = pActor->mRailRider;
        s32 arg = *pArg;
        bool b = railRider->getCurrentPointArgS32WithInit(getRailPointArgName(argNum), &arg);
        *pArg = arg;
        return b;
    }

    bool getRailCurrentPointArgF32NoInit(const LiveActor* pActor, s32 argNum, f32* pArg) NO_INLINE {
        RailRider* railRider = pActor->mRailRider;
        s32 arg = *pArg;
        bool b = railRider->getCurrentPointArgS32NoInit(getRailPointArgName(argNum), &arg);
        *pArg = arg;
        return b;
    }

    bool getRailNextPointArgF32NoInit(const LiveActor* pActor, s32 argNum, f32* pArg) NO_INLINE {
        RailRider* railRider = pActor->mRailRider;
        s32 arg = *pArg;
        bool b = railRider->getNextPointArgS32NoInit(getRailPointArgName(argNum), &arg);
        *pArg = arg;
        return b;
    }

    bool getRailNextPointArgBoolNoInit(const LiveActor* pActor, s32 argNum, bool* pArg) NO_INLINE {
        RailRider* railRider = pActor->mRailRider;
        s32 arg = -1;
        bool b = railRider->getNextPointArgS32NoInit(getRailPointArgName(argNum), &arg);
        if (!b) {
            return false;
        }
        if (arg != -1) {
            *pArg = true;
        } else {
            *pArg = false;
        }
        return true;
    }
}  // namespace

namespace MR {
    bool getRailPointArg0WithInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32WithInit(getRailPointArgName(0), pArg, index);
    }

    bool getRailPointArg2WithInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32WithInit(getRailPointArgName(2), pArg, index);
    }

    bool getRailPointArg0NoInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32NoInit(getRailPointArgName(0), pArg, index);
    }

    bool getRailPointArg1NoInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32NoInit(getRailPointArgName(1), pArg, index);
    }

    bool getRailPointArg4NoInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32NoInit(getRailPointArgName(4), pArg, index);
    }

    bool getRailPointArg6NoInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32NoInit(getRailPointArgName(6), pArg, index);
    }

    bool getRailPointArg7NoInit(const LiveActor* pActor, s32 index, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getPointArgS32NoInit(getRailPointArgName(7), pArg, index);
    }

    bool getRailPointArg2WithInit(const LiveActor* pActor, s32 index, f32* pArg) {
        return getRailPointArgF32WithInit(pActor, 2, index, pArg);
    }

    bool getRailPointArg3WithInit(const LiveActor* pActor, s32 index, f32* pArg) {
        return getRailPointArgF32WithInit(pActor, 3, index, pArg);
    }

    bool getRailPointArg0NoInit(const LiveActor* pActor, s32 index, f32* pArg) {
        return getRailPointArgF32NoInit(pActor, 0, index, pArg);
    }

    bool getRailPointArg1NoInit(const LiveActor* pActor, s32 index, f32* pArg) {
        return getRailPointArgF32NoInit(pActor, 1, index, pArg);
    }

    bool getCurrentRailPointArg0WithInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getCurrentPointArgS32WithInit(getRailPointArgName(0), pArg);
    }

    bool getCurrentRailPointArg1WithInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getCurrentPointArgS32WithInit(getRailPointArgName(1), pArg);
    }

    bool getCurrentRailPointArg0NoInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getCurrentPointArgS32NoInit(getRailPointArgName(0), pArg);
    }

    bool getCurrentRailPointArg1NoInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getCurrentPointArgS32NoInit(getRailPointArgName(1), pArg);
    }

    bool getCurrentRailPointArg5NoInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getCurrentPointArgS32NoInit(getRailPointArgName(5), pArg);
    }

    bool getCurrentRailPointArg7NoInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getCurrentPointArgS32NoInit(getRailPointArgName(7), pArg);
    }

    bool getCurrentRailPointArg0WithInit(const LiveActor* pActor, f32* pArg) {
        return getRailCurrentPointArgF32WithInit(pActor, 0, pArg);
    }

    bool getCurrentRailPointArg0NoInit(const LiveActor* pActor, f32* pArg) {
        return getRailCurrentPointArgF32NoInit(pActor, 0, pArg);
    }

    bool getCurrentRailPointArg1NoInit(const LiveActor* pActor, f32* pArg) {
        return getRailCurrentPointArgF32NoInit(pActor, 1, pArg);
    }

    bool getNextRailPointArg0WithInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getNextPointArgS32WithInit(getRailPointArgName(0), pArg);
    }

    bool getNextRailPointArg1NoInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getNextPointArgS32NoInit(getRailPointArgName(1), pArg);
    }

    bool getNextRailPointArg0NoInit(const LiveActor* pActor, f32* pArg) {
        return getRailNextPointArgF32NoInit(pActor, 0, pArg);
    }

    bool getNextRailPointArg1NoInit(const LiveActor* pActor, f32* pArg) {
        return getRailNextPointArgF32NoInit(pActor, 1, pArg);
    }

    bool getNextRailPointArg2WithInit(const LiveActor* pActor, bool* pArg) {
        *pArg = false;
        return getRailNextPointArgBoolNoInit(pActor, 2, pArg);
    }

    bool getRailArg0WithInit(const LiveActor* pActor, s32* pArg) {
        RailRider* railRider = pActor->mRailRider;
        return railRider->getRailArgWithInit("path_arg0", pArg);
    }
}  // namespace MR

namespace {
    const char* getRailPointArgName(s32 argNum) {
        if (argNum == 0) {
            return "point_arg0";
        }
        if (argNum == 1) {
            return "point_arg1";
        }
        if (argNum == 2) {
            return "point_arg2";
        }
        if (argNum == 3) {
            return "point_arg3";
        }
        if (argNum == 4) {
            return "point_arg4";
        }
        if (argNum == 5) {
            return "point_arg5";
        }
        if (argNum == 6) {
            return "point_arg6";
        }
        if (argNum == 7) {
            return "point_arg7";
        }
        return nullptr;
    }
}  // namespace
