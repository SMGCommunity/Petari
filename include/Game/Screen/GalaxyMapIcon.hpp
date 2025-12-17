#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class DrawInfo;
    };  // namespace lyt
};  // namespace nw4r

class ButtonPaneController;

class GalaxyMapIcon : public LayoutActor {
public:
    GalaxyMapIcon(const char*, LayoutActor*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void appear();
    virtual void control();

    void calcAnimForCapture(const nw4r::lyt::DrawInfo&);
    void setModeNormal();
    void setModeNewGalaxyDiscover();
    bool isStatusNew() const;
    void onPointing();
    void activatePointing();
    void deactivatePointing();
    s32 examineIconStatus() const;
    bool isBlink(int) const;
    bool isNewOnDiscoverMode() const;
    bool isPointing() const;
    void syncStatus();

    /* 0x20 */ ButtonPaneController* mButtonPaneController;
    /* 0x24 */ const char* mGalaxyName;
    /* 0x28 */ LayoutActor* mHost;
    /* 0x2C */ const char* mPaneName;
    /* 0x30 */ u8 mMode;
};
