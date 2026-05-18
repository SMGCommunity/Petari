#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaSequencerVs1 : public KoopaSequencer {
public:
    KoopaSequencerVs1();

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
};
