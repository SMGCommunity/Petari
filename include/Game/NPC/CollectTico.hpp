#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StrayTico;

class CollectTico : public LiveActor {
public:
    CollectTico(const char*);

    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeCompleteDemo();
    void exeFlash();
    void exeAppearPowerStar();
    inline void exeTryStartDemo();
    s32 calcNoRescuedCount() const;
    void startAppearPowerStar();

    /* 0x8C */ StrayTico** mStrayTicos;
    /* 0x90 */ s32 mTicoNum;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ u8 _A0;
};
