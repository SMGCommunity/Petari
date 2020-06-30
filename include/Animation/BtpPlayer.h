#pragma once

#include "Animation/AnmPlayer.h"
#include "J3DGraphBase/J3DModel.h"
#include "System/Resource/ResourceHolder.h"

class BtpPlayer : public MaterialAnmPlayerBase
{
public:
    BtpPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};