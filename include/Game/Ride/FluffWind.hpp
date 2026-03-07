#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class FluffWindEffect : public LiveActor {
public:
    FluffWindEffect(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorDead();
    virtual void startClipped();

    void initEffectInfo(const TVec3f&, const TVec3f&, const TVec3f&, const char*, f32);

    void exeBrowWind();

    /* 0x8C */ const char* mEffectName;
    /* 0x90 */ TPos3f mMtx;
    /* 0xC0 */ s32 mTimer;
};

class FluffWind : public LiveActor {
public:
    FluffWind(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void makeActorDead();

    /* 0x8C */ s32 mNumEffects;
    /* 0x90 */ FluffWindEffect** mEffects;
};

class FluffWindHolder : public LiveActorGroup {
public:
    FluffWindHolder();

    void calcWindInfo(const TVec3f& rPosition, TVec3f* pWindDirection, f32* pWindStrength) const;
};

namespace FluffFunction {
    bool calcFluffWindInfo(const TVec3f& rPosition, TVec3f* pWindDirection, f32* pWindStrength);
};
