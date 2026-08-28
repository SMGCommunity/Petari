#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class SpotMarkLight : public PartsModel {
public:
    SpotMarkLight(LiveActor*, f32, f32, MtxPtr);

    /* 0x08 */ virtual ~SpotMarkLight();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
};