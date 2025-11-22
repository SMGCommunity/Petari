#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class StrayTico;

class CollectTico : public LiveActor {
public:
    CollectTico(const char*);

    virtual ~CollectTico();
    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeCompleteDemo();
    void exeFlash();
    void exeAppearPowerStar();
    inline void exeTryStartDemo();
    s32 calcNoRescuedCount() const;
    void startAppearPowerStar();

    StrayTico** mStrayTicos;  // 0x8C
    s32 mTicoNum;             // 0x90
    TVec3f _94;
    u8 _A0;
};
