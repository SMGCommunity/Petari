#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class J3DAnmBase;
class J3DModelData;
class ResourceHolder;

class BpkPlayer : public MaterialAnmPlayerBase {
public:
    BpkPlayer(const ResourceHolder*, J3DModelData*);

    virtual void attach(J3DAnmBase*, J3DModelData*);
    virtual void detach(J3DAnmBase*, J3DModelData*);
};
