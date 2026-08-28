#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class BpkPlayer : public MaterialAnmPlayerBase {
public:
    BpkPlayer(const ResourceHolder*, J3DModelData*);

    /* 0x10 */ virtual void attach(J3DAnmBase*, J3DModelData*);
    /* 0x14 */ virtual void detach(J3DAnmBase*, J3DModelData*);
};
