#pragma once

#include "Game/Screen/LayoutActor.hpp"

class GalaxyMapTicoIcon : public LayoutActor {
public:
    GalaxyMapTicoIcon(int, const char*, LayoutActor*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void syncStatus();
    void setModeNormal();
    void setModeDiscover();
    bool isModeDiscoverAndIsAppear() const;

private:
    /* 0x20 */ int mExclamationGalaxyIndex;
    /* 0x24 */ const char* mGalaxyName;
    /* 0x28 */ LayoutActor* mHost;
    /* 0x2C */ char* mPaneName;
    /* 0x30 */ u8 mMode;
};
