#pragma once

#include "Game/Boss/KoopaSequencer.hpp"

class KoopaSequencerVs2 : public KoopaSequencer {
public:
    KoopaSequencerVs2();

private:
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 _28;
};
