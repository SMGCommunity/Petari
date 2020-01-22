#ifndef BVAPLAYER_H
#define BVAPLAYER_H

#include "Animation/AnmPlayer.h"
#include "J3DGraphBase/J3DModel.h"

class BvaPlayer : public AnmPlayerBase
{
public:
    BvaPlayer(const ResTable *, J3DModel *);

    void calc();
    bool getAnmVisibility();
};

#endif // BVAPLAYER_H