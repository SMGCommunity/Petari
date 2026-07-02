#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class BpkPlayer : public MaterialAnmPlayerBase {
public:
    BpkPlayer(const ResourceHolder*, J3DModelData*);

    virtual void attach(J3DAnmBase*, J3DModelData*);
    virtual void detach(J3DAnmBase*, J3DModelData*);
};
