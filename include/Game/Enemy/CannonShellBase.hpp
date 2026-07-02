#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/Array.hpp"

class CannonShellBase : public LiveActor {
public:
    CannonShellBase(const char* pName) : LiveActor(pName) {
    }

    virtual ~CannonShellBase();
    virtual void launch(const TVec3f& rStartPos, const TVec3f& rVelocity) = 0;
    // These two virtuals get moved around based on the TU so they can't be in the CPP
    virtual f32 getBaseScale() const {
        return 1.0f;
    }
    virtual s32 getLifeTime() const {
        return 360;
    }
};

class CannonShellHolder {
public:
    CannonShellHolder(int num);

    void registerCannonShell(CannonShellBase*);
    /* void printInfo(); */  // Exists according to the Debug maps
    CannonShellBase* getValidShell() const;
    void killActiveShells() const;

    MR::Vector< MR::AssignableArray< CannonShellBase* > > mShells;
};
