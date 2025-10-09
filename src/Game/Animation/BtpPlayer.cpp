#include "Game/Animation/BtpPlayer.hpp"
#include "Game/System/ResourceHolder.hpp"
// #include <JSystem/J3DGraphAnimator/J3DMaterialAttach.hpp>

BtpPlayer::BtpPlayer(const ResourceHolder* pResourceHolder, J3DModelData* pModelData) :
    MaterialAnmPlayerBase(pResourceHolder->mBtpResTable, pModelData)
{
    
}

// BtpPlayer::attach
// BtpPlayer::detach
