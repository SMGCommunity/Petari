#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class BrkPlayer : public MaterialAnmPlayerBase {
public:
    BrkPlayer(const ResourceHolder*, J3DModelData*);

    virtual void attach(J3DAnmBase*, J3DModelData*);
    virtual void detach(J3DAnmBase*, J3DModelData*);
};
