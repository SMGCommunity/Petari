#include "Game/Enemy/Karon.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

namespace {
    static KaronParam hNoMoveNoTurnParam = { 0.0f, 1.2f, 0.80000001f, 0.0f };
    static KaronParam hNoMoveNoTurnOnAirParam = { 0.0f, 2.0f, 0.94f, 0.0f };
    static KaronParam hWalkparam = { 0.12f, 1.2f, 0.94f, 2.0f };
    static KaronParam hSearchParam = { 0.0f, 1.2f, 0.94f, 2.0f };
    static KaronParam hPursueParam = { 0.25f, 1.2f, 0.94f, 2.0f };
    static KaronParam hHitReactionOnGroundParam = { 0.0f, 1.2f, 0.80f, 5.0f };
    static KaronParam hHitReactionAirParam = { 0.0f, 2.0f, 0.94f, 5.0f };
};

namespace NrvKaron {
    NEW_NERVE(HostTypeNrvFixWait, Karon, FixWait);
    NEW_NERVE(HostTypeNrvWait, Karon, Wait);
    NEW_NERVE(HostTypeNrvStarPieceHitWait, Karon, Wait);
    NEW_NERVE(HostTypeNrvWalk, Karon, Walk);
    NEW_NERVE(HostTypeNrvTurn, Karon, Turn);
    NEW_NERVE(HostTypeNrvWalkOnRail, Karon, WalkOnRail);
    NEW_NERVE(HostTypeNrvSearch, Karon, Search);
    NEW_NERVE_ONEND(HostTypeNrvPursue, Karon, Pursue, Pursue);
    NEW_NERVE(HostTypeNrvBroken, Karon, Broken);
    NEW_NERVE(HostTypeNrvStepBroken, Karon, StepBroken);
    NEW_NERVE(HostTypeNrvRecover, Karon, Recover);
    NEW_NERVE(HostTypeNrvHitReaction, Karon, HitReaction);
    NEW_NERVE(HostTypeNrvStarPieceHit, Karon, HitReaction);
    NEW_NERVE(HostTypeNrvDeath, Karon, Death);
    NEW_NERVE(HostTypeNrvSinkDown, Karon, SinkDown);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, Karon, BindStarPointer, BindStarPointer);
};

void Karon::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvKaron::HostTypeNrvStarPieceHit::sInstance)) {
            MR::startAction(this, "StarPiece");
        }
        else {
            MR::startAction(this, "AttackHit");
        }
    }

    MR::startLevelSound(this, "SE_EV_LV_KARON_ATK_SUCCESS", -1, -1, -1);

    if (isNerve(&NrvKaron::HostTypeNrvStarPieceHit::sInstance)) {
        if (MR::isOnGround(this)) {
            MR::moveAndTurnToPlayer(this, hNoMoveNoTurnParam._0, hNoMoveNoTurnParam._4, hNoMoveNoTurnParam._8, hNoMoveNoTurnParam._C);
        }
        else {
            MR::moveAndTurnToPlayer(this, hNoMoveNoTurnOnAirParam._0, hNoMoveNoTurnOnAirParam._4, hNoMoveNoTurnOnAirParam._8, hNoMoveNoTurnOnAirParam._C);
        }
    }
    else if (MR::isOnGround(this)) {
        MR::moveAndTurnToPlayer(this, hHitReactionOnGroundParam._0, hHitReactionOnGroundParam._4, hHitReactionOnGroundParam._8, hHitReactionOnGroundParam._C);
    }
    else {
        MR::moveAndTurnToPlayer(this, hHitReactionAirParam._0, hHitReactionAirParam._4, hHitReactionAirParam._8, hHitReactionAirParam._C);
    }

    if (MR::isOnGround(this) && MR::isActionEnd(this)) {
        if (isNerve(&NrvKaron::HostTypeNrvStarPieceHit::sInstance)) {
            setNerve(&NrvKaron::HostTypeNrvStarPieceHitWait::sInstance);
        }
        else {
            setNerve(&NrvKaron::HostTypeNrvWait::sInstance);
        }
    }    
}

void Karon::exeDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Down");
        MR::startSound(this, "SE_EM_KARON_BROKEN", -1, -1);
        MR::startSound(this, "SE_EV_KARON_DAMAGE", -1, -1);
    }

    moveAndTurn(&hNoMoveNoTurnParam);
    
    if (MR::isActionEnd(this)) {
        kill();
    }
}
