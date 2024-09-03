#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/MessageUtil.hpp"

struct GalaxyNamePlateDrawerEntry {
    LayoutActor* mHostActor;    // 0x0
    u32 mZ;                     // 0x4
};

class GalaxyNamePlate : public LayoutActor {
public:
    GalaxyNamePlate(const char *, bool);

    virtual ~GalaxyNamePlate();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual void control();

    void showUnknown(bool);
    void showNew(bool);
    void showOpen(bool);
    void show(const wchar_t *, bool);
    void setPos3D(const TVec3f &);
    void setShowBalloonNozzle(bool);
    void show(const wchar_t *, s32, bool, bool);

    void exeAppearReady();
    void exeAppear();
    void exeWait();

    const char* mGalaxyName;                    // 0x20
    bool _24;
    bool _25;
    bool mShowBalloonNozzle;                    // 0x26
    u8 _27;
    GalaxyNamePlateDrawerEntry* mDrawerEntry;   // 0x28
    s32 _2C;
    u8 _30;
};

namespace NrvGalaxyNamePlate {
    NERVE_DECL(GalaxyNamePlateNrvAppearReady, GalaxyNamePlate, GalaxyNamePlate::exeAppearReady);
    NERVE_DECL(GalaxyNamePlateNrvAppear, GalaxyNamePlate, GalaxyNamePlate::exeAppear);
    NERVE_DECL(GalaxyNamePlateNrvWait, GalaxyNamePlate, GalaxyNamePlate::exeWait);
};