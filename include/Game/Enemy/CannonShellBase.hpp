#pragma once

#include "Game/Util/Array.hpp"

class CannonShellBase;

class CannonShellHolder {
public:
    CannonShellHolder(int num);

    void registerCannonShell(CannonShellBase*);
    /* void printInfo(); */  // Exists according to the Debug maps
    CannonShellBase* getValidShell() const;
    void killActiveShells() const;

    MR::AssignableArray< CannonShellBase* > mShells;
    s32 mShellCount;
};