#include "Game/Enemy/ItemGenerator.hpp"
#include "Game/Util/SceneUtil.hpp"

ItemGenerator::ItemGenerator() {
    mNumType = 1;
    mType = Coin;
}

void ItemGenerator::setTypeNone() {
    mNumType = 0;
    mType = None;
}

void ItemGenerator::setTypeCoin(s32 arg0) {
    mNumType = arg0;
    mType = Coin; 
}

void ItemGenerator::setTypeStarPeace(s32 arg0) {
    mNumType = arg0;
    mType = StarPiece; 
}

bool ItemGenerator::isUseFarSE() const {
    return MR::isEqualStageName("KoopaBattleVs2Galaxy");
}
