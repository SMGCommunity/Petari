#include "Game/Animation/BpkPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
// #include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>

BpkPlayer::BpkPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData) :
    MaterialAnmPlayerBase(pResourceHolder->mBpkResTable, pModelData)
{
    
}

// BpkPlayer::attach
// BpkPlayer::detach
