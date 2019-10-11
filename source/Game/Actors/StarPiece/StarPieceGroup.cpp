#include "Game/Actors/StarPiece/StarPieceGroup.h"
#include "MR/actor/ActorMovementUtil.h"

void StarPieceGroup::placementAllPiece()
{
    if (_A0 == 0)
    {
        placementPieceOnCircle();
    }
    else
    {
        if (_A1 == 0)
        {
            placementPieceOnRail();
        }
        else
        {
            placementPieceOnRailPoint();
        }
    }
}

void StarPieceGroup::placementPieceOnCircle()
{
    if (mNumStarbits != 1)
    {
        Mtx trsMtx;
        MR::makeMtxTRS(trsMtx, this);
    }
}