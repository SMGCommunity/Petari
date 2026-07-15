#include "Game/Enemy/Hanachan.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvHanachan {
    NEW_NERVE(HanachanPartsNrvWalk, HanachanParts, Walk);
    NEW_NERVE(HanachanPartsNrvBecomeAngry, HanachanParts, BecomeAngry);
    NEW_NERVE(HanachanPartsNrvAngryPursue, HanachanParts, AngryPursue);
    NEW_NERVE(HanachanPartsNrvAngryEnd, HanachanParts, AngryEnd);
    NEW_NERVE(HanachanPartsNrvWallHitEnd, HanachanParts, WallHitEnd);
    NEW_NERVE(HanachanPartsNrvTrample, HanachanParts, Trample);
    NEW_NERVE(HanachanPartsNrvOverturn, HanachanParts, Overturn);
    NEW_NERVE(HanachanPartsNrvOverturnBound, HanachanParts, OverturnBound);
    NEW_NERVE(HanachanPartsNrvOverturnWait, HanachanParts, OverturnWait);
    NEW_NERVE(HanachanPartsNrvRecover, HanachanParts, Recover);
    NEW_NERVE(HanachanPartsNrvHipDropped, HanachanParts, HipDropped);
    NEW_NERVE(HanachanPartsNrvOverturnHipDropped, HanachanParts, HipDropped);
    NEW_NERVE(HanachanPartsNrvBlow, HanachanParts, Blow);
    NEW_NERVE(HanachanPartsNrvStarPointerBind, HanachanParts, StarPointerBind);

    NEW_NERVE(HanachanNrvHanachanWalk, Hanachan, Walk);
    NEW_NERVE(HanachanNrvHanachanBecomeAngry, Hanachan, BecomeAngry);
    NEW_NERVE(HanachanNrvHanachanAngryPursue, Hanachan, AngryPursue);
    NEW_NERVE(HanachanNrvHanachanAngryEnd, Hanachan, AngryEnd);
    NEW_NERVE(HanachanNrvHanachanWallHitEnd, Hanachan, WallHitEnd);
    NEW_NERVE(HanachanNrvHanachanTrample, Hanachan, Trample);
    NEW_NERVE(HanachanNrvHanachanOverturn, Hanachan, Overturn);
    NEW_NERVE(HanachanNrvHanachanOverturnBound, Hanachan, OverturnBound);
    NEW_NERVE(HanachanNrvHanachanOverturnWait, Hanachan, OverturnWait);
    NEW_NERVE(HanachanNrvHanachanRecover, Hanachan, Recover);
    NEW_NERVE(HanachanNrvHanachanHipDropped, Hanachan, HipDropped);
    NEW_NERVE(HanachanNrvHanachanOverturnHipDropped, Hanachan, HipDropped);
    NEW_NERVE(HanachanNrvHanachanBlow, Hanachan, Blow);
    NEW_NERVE(HanachanNrvHanachanStarPointerBind, Hanachan, StarPointerBind);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindEnd, Hanachan, WallHitEnd);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindOverturn, Hanachan, StarPointerBind);
    NEW_NERVE(HanachanNrvHanachanStarPointerBindEndOverturn, Hanachan, WallHitEnd);
};  // namespace NrvHanachan

Hanachan::Hanachan(const char* pName) : LiveActor(pName) {
}
Hanachan::~Hanachan() {
}
