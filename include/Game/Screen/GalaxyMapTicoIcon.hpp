#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyMapTicoIcon : public LayoutActor {
public:
    GalaxyMapTicoIcon(int, const char*, LayoutActor*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();

    void syncStatus();
    void setModeNormal();
    void setModeDiscover();
    bool isModeDiscoverAndIsAppear() const;

public:
    /* 0x20 */ int mExclamationGalaxyIndex;
    /* 0x24 */ const char* mGalaxyName;
    /* 0x28 */ LayoutActor* mHost;
    /* 0x2C */ char* mPaneName;
    /* 0x30 */ u8 mMode;
};
