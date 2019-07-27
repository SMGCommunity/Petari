#ifndef BTPPLAYER_H
#define BTPPLAYER_H

#include "Animation/AnmPlayer.h"
#include "Model/J3D/J3DModel.h"
#include "System/Resource/ResourceHolder.h"

class BtpPlayer : public MaterialAnmPlayerBase
{
public:
    BtpPlayer(const ResourceHolder *, J3DModelData *);

    virtual void attach(J3DAnmBase *, J3DModelData *);
    virtual void detach(J3DAnmBase *, J3DModelData *);
};

#endif // BTPPLAYER_H