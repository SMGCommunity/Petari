#pragma once

#include "Game/Animation/AnmPlayer.hpp"

class J3DAnmVisibilityFull;
class J3DModel;
class ResTable;

class BvaPlayer : public AnmPlayerBase {
public:
    BvaPlayer(const ResTable*, J3DModel*);

    void calc();
    J3DAnmVisibilityFull* getAnmVisibility();

public:
    /* 0x20 */ J3DModel* mModel;
};
