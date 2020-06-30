#pragma once

#include "Animation/AnmPlayer.h"
#include "J3DGraphBase/J3DModel.h"
#include "System/Resource/ResourceHolder.h"

class BtkPlayer : public MaterialAnmPlayerBase
{
public:
    BtkPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};