#include "Game/Enemy/ItemGenerator.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

ItemGenerator::ItemGenerator() {
    mNumType = 1;
    mType = TYPE_COIN;
}

void ItemGenerator::setTypeNone() {
    mType = TYPE_NONE;
    mNumType = 0;
}

void ItemGenerator::setTypeCoin(s32 arg0) {
    mNumType = arg0;
    mType = TYPE_COIN; 
}

void ItemGenerator::setTypeStarPeace(s32 arg0) {
    mNumType = arg0;
    mType = TYPE_STARPIECE; 
}

void ItemGenerator::generate(const LiveActor* pActor) {
    if (mNumType > 0) {
        switch (mType) {
            case 1:
            MR::appearCoinPopToDirection(pActor, pActor->mPosition, -pActor->mGravity, mNumType);
            break;
            case 2:
            MR::appearStarPieceToDirection(pActor, pActor->mPosition,  -pActor->mGravity, mNumType, 10.0f, 40.0f, false);
            if (ItemGenerator::isUseFarSE()) {
                MR::startSound(pActor, "SE_OJ_STAR_PIECE_BURST_F", -1, -1);
            }
            else {
                MR::startSound(pActor, "SE_OJ_STAR_PIECE_BURST", -1, -1);
            }
            break;
        }
    }
}

//Gets Inlined :(
bool ItemGenerator::isUseFarSE() const {
    return MR::isEqualStageName("KoopaBattleVs2Galaxy");
}
