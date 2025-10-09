#include "Game/Animation/BtkPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
// #include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>

BtkPlayer::BtkPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData) :
    MaterialAnmPlayerBase(pResourceHolder->mBtkResTable, pModelData)
{
    
}

// BtkPlayer::attach
// BtkPlayer::detach
