#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Util/Array.hpp"

class GalaxyMapCometIcon;
class GalaxyMapDomeIcon;
class GalaxyMapGalaxyDetail;
class GalaxyMapGalaxyPlain;
class GalaxyMapIcon;
class GalaxyMapMarioIcon;
class GalaxyMapTicoIcon;
class GalaxyMapTitle;
class IconAButton;

namespace nw4r {
    namespace lyt {
        class DrawInfo;
    };  // namespace lyt
};  // namespace nw4r

class GalaxyMap : public LayoutActor {
public:
    /// @brief Creates a new `GalaxyMap`.
    GalaxyMap();

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();

    void movementForCapture();
    void calcAnimForCapture(const nw4r::lyt::DrawInfo&);
    void drawForCapture(const nw4r::lyt::DrawInfo&);
    void setModeNormal();
    void setModeNewGalaxyDiscover();
    void setModeNewTicoGalaxyDiscover();
    void setModeNewDomeDiscover();
    void setModeTicoCometPray();
    void setModeCapture();
    void drawGalaxyInfo() const;
    bool isTransition() const;
    bool isPointingAnything() const;
    bool isShowDetail() const;
    void dispIconAButton();
    void changeToGalaxyMap();
    void changeToAstroMap();
    void forceToGalaxyMap();
    void forceToAstroMap();
    void initPaneCtrlPointing();
    void initDomeIcon();
    void initMarioIcon();
    void initTicoIcon();
    bool tryFocusIn();
    void updateGalaxyName();
    void drawGalaxyIconWithOrder(int) const;
    GalaxyMapIcon* getPointingGalaxyIcon() const;

    void exeIdle();
    void exeFocusIn();
    void exeFocusOut();
    void exeFadeinGalaxyMap();
    void exeFadeinAstroMap();
    void exeShowDetail();

    /* 0x20 */ MR::Vector< MR::AssignableArray< GalaxyMapIcon* > > mIcon;
    /* 0x2C */ MR::Vector< MR::AssignableArray< GalaxyMapCometIcon* > > mCometIcon;
    /* 0x38 */ MR::Vector< MR::AssignableArray< GalaxyMapDomeIcon* > > mDomeIcon;
    /* 0x44 */ MR::Vector< MR::AssignableArray< GalaxyMapTicoIcon* > > mTicoIcon;
    /* 0x50 */ GalaxyMapMarioIcon* mMarioIcon1;
    /* 0x54 */ GalaxyMapMarioIcon* mMarioIcon2;
    /* 0x58 */ GalaxyMapGalaxyPlain* mGalaxyPlain;
    /* 0x5C */ GalaxyMapGalaxyDetail* mGalaxyDetail;
    /* 0x60 */ GalaxyMapIcon* mPointingIcon;
    /* 0x64 */ GalaxyMapTitle* mTitle;
    /* 0x68 */ IconAButton* mIconAButton;
    /* 0x6C */ const char* mGalaxyName;
    /* 0x70 */ bool _70;
    /* 0x71 */ bool _71;
};
