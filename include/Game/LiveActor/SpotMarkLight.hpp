#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class SpotMarkLight : public PartsModel {
public:
    SpotMarkLight(LiveActor*, f32, f32, MtxPtr);

    virtual ~SpotMarkLight();
    virtual void control();
    virtual void calcAndSetBaseMtx();
};