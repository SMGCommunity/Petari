#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyNamePlate;

class GalaxyMapGalaxyPlain : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyMapGalaxyPlain`.
    /// @param pHost The pointer to the owning actor instance.
    GalaxyMapGalaxyPlain(const LayoutActor* pHost);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();

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
