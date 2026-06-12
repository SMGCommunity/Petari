#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class BtkPlayer : public MaterialAnmPlayerBase {
public:
    BtkPlayer(const ResourceHolder*, J3DModelData*);

    virtual void attach(J3DAnmBase*, J3DModelData*);
    virtual void detach(J3DAnmBase*, J3DModelData*);
};
