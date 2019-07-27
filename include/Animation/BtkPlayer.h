#ifndef BTKPLAYER_H
#define BTKPLAYER_H

#include "Animation/AnmPlayer.h"
#include "Model/J3D/J3DModel.h"
#include "System/Resource/ResourceHolder.h"

class BtkPlayer : public MaterialAnmPlayerBase
{
public:
    BtkPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};

#endif // BTKPLAYER_H