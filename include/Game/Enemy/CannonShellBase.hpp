#pragma once

#include "Game/Util/Array.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/LiveActorUtil.hpp"

class CannonShellBase : public LiveActor {
public:
    CannonShellBase(const char* pName) : LiveActor(pName) { }

    virtual void launch(const TVec3f&, const TVec3f&) = 0;
    virtual f32 getBaseScale() const = 0;
    virtual s32 getLifetime() const = 0;
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