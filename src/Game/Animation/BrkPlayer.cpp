#include "Game/Animation/BrkPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
// #include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>

BrkPlayer::BrkPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData) :
    MaterialAnmPlayerBase(pResourceHolder->mBrkResTable, pModelData)
{
    
}

// BrkPlayer::attach
// BrkPlayer::detach
