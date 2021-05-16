#include "Enemy/ItemGenerator.h"
#include "Util/ObjUtil.h"
#include "Util/SceneUtil.h"
#include "Util/SoundUtil.h"

ItemGenerator::ItemGenerator()
    : mItemCount(1), mType(1)
    { }

void ItemGenerator::setTypeNone()
{
    mType = 0;
    mItemCount = 0;
}

void ItemGenerator::setTypeCoin(s32 count)
{
    mItemCount = count;
    mType = 1;
}

void ItemGenerator::setTypeStarPeace(s32 count)
{
    mItemCount = count;
    mType = 2;
}

/* todo -- fix the vector initialization */
void ItemGenerator::generate(const LiveActor *pActor)
{
    if (mItemCount > 0)
    {
        if (mType == 2)
        {
            JGeometry::TVec3<f32> temp;
            temp = temp - pActor->mGravity;
            MR::appearStarPieceToDirection(pActor, pActor->mTranslation, temp, mItemCount, 10.0f, 40.0f, false);
        
            if (isUseFarSE())
            {
                MR::startSound(pActor, "SE_OJ_STAR_PIECE_BURST_F", -1, -1);
            }
            else
            {
                MR::startSound(pActor, "SE_OJ_STAR_PIECE_BURST", -1, -1);
            }
        }
        else
        {
            if (mType < 2)
            {
                if (mType >= 1)
                {
                    JGeometry::TVec3<f32> temp;
                    temp = temp - pActor->mGravity;
                    MR::appearCoinPopToDirection(pActor, pActor->mTranslation, temp, mItemCount);
                }
            }
        }
        
    }
}

bool ItemGenerator::isUseFarSE() const
{
    return MR::isEqualStageName("KoopaBattleVs2Galaxy");
}