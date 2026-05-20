#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Enemy/KoopaJrShipCannonShell.hpp"

class IronCannonShell : public KoopaJrShipCannonShell {
public:
    IronCannonShell(const char*);

    virtual ~IronCannonShell();

    virtual f32 getBaseScale() const;
    virtual s32 getLifeTime() const;
};

class IronCannonLauncherPoint : public LiveActor {
public:
    IronCannonLauncherPoint(const char*);

    virtual ~IronCannonLauncherPoint();
    virtual void init(const JMapInfoIter& rIter);

    virtual void initModelAndConnectToScene();

    void exeWait();
    void exeShot();

    void initBullet();
    bool tryShotBullet(f32);

    CannonShellHolder* mShellHolder; // 0x8C
    s32 mShotInterval; // 0x90 : Delay between cannon shots [ObjArg0]
    f32 mBulletSpeed; // 0x94 : Speed of each bullet [ObjArg1]
};

class IronCannonLauncher : public IronCannonLauncherPoint {
public:
    IronCannonLauncher(const char*);

    virtual ~IronCannonLauncher();
    virtual void init(const JMapInfoIter& rIter);

    virtual void initModelAndConnectToScene();

    void exeRelax();
    void exeWait();
    void exeShot();


    TVec3f mEffectMtx;  // 0x98
};
