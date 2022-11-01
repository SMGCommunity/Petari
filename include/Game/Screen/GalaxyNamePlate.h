#pragma once

#include "Game/Screen/LayoutActor.h"

struct GalaxyNamePlateDrawerEntry {
    LayoutActor* mHostActor;    // _0
    u32 mZ;                     // _4
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

    const char* mGalaxyName;                    // _20
    bool _24;
    bool _25;
    bool mShowBalloonNozzle;                    // _26
    u8 _27;
    GalaxyNamePlateDrawerEntry* mDrawerEntry;   // _28
    s32 _2C;
    u8 _30;
};

namespace NrvGalaxyNamePlate {
    NERVE_DECL(GalaxyNamePlateNrvAppearReady, GalaxyNamePlate, GalaxyNamePlate::exeAppearReady);
    NERVE_DECL(GalaxyNamePlateNrvAppear, GalaxyNamePlate, GalaxyNamePlate::exeAppear);
    NERVE_DECL(GalaxyNamePlateNrvWait, GalaxyNamePlate, GalaxyNamePlate::exeWait);
};