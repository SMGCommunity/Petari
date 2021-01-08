#pragma once

#include "Animation/AnmPlayer.h"
#include "J3DGraphAnimator/J3DModel.h"

class BvaPlayer : public AnmPlayerBase
{
public:
    BvaPlayer(const ResTable *, J3DModel *);

    void calc();
    bool getAnmVisibility();
};