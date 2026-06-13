#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class BtpPlayer : public MaterialAnmPlayerBase {
public:
    BtpPlayer(const ResourceHolder*, J3DModelData*);

    virtual void attach(J3DAnmBase*, J3DModelData*);
    virtual void detach(J3DAnmBase*, J3DModelData*);
};
