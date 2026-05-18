#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaSequencerVs3 : public KoopaSequencer {
public:
    KoopaSequencerVs3();

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
    /* 0x2C */ u32 _2C;
    /* 0x30 */ u32 _30;
    /* 0x34 */ u32 _34;
};
