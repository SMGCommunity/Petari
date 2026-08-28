#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyNamePlate;

class GalaxyMapGalaxyPlain : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyMapGalaxyPlain`.
    /// @param pHost The pointer to the owning actor instance.
    GalaxyMapGalaxyPlain(const LayoutActor* pHost);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void show(const char*, const char*);
    void adjustTransInScreen();
    void show(const wchar_t*, const char*);

private:
    /* 0x20 */ const LayoutActor* mHost;
    /* 0x24 */ const char* mPaneName;
    /* 0x28 */ GalaxyNamePlate* mNamePlate;
    /* 0x2C */ wchar_t* mGalaxyName;
    /* 0x30 */ bool _30;
};
