#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class LavaHomeSeesawRotator;
class ProjmapEffectMtxSetter;

class LavaHomeSeesaw : public LiveActor {
public:
    LavaHomeSeesaw(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void control();
    virtual void calcAndSetBaseMtx();

    /* 0x8C */ LavaHomeSeesawRotator* mSeesawRotator;
    /* 0x90 */ ProjmapEffectMtxSetter* mMtxSetter;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ f32 mYOffset;
};
