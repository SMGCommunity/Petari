#pragma once
#include "Game/Screen/LayoutActor.hpp"
#include "nw4r/lyt/drawInfo.h"

#include "Game/Screen/GalaxyMapGalaxyPlain.hpp"
#include "Game/Screen/GalaxyMapGalaxyDetail.hpp"
#include "Game/Screen/GalaxyMapTitle.hpp"
#include "Game/Screen/GalaxyMapIcon.hpp"
#include "Game/Screen/GalaxyMapMarioIcon.hpp"
#include "Game/Screen/GalaxyMapTicoIcon.hpp"
#include "Game/Screen/GalaxyMapDomeIcon.hpp"
#include "Game/Screen/GalaxyMapCometIcon.hpp"
#include "Game/Util/Array.hpp"

class GalaxyMap : public LayoutActor {
public:

    GalaxyMap( const char* pName);

    virtual ~GalaxyMap();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();
    virtual void movement();
    virtual void calcAnim();
    virtual void draw() const;
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
    void isTransition() const;
    void isPointingAnything() const;
    void isShowDetail() const;
    void dispIconAButton();
    void changeToGalaxyMap();
    void changeToAstroMap();
    void forceToGalaxyMap();
    void forceToAstroMap();
    void exeFocusIn();
    void exeFocusOut();
    void exeFadeinGalaxyMap();
    void exeFadeinAstroMap();
    void exeShowDetail();
    void initPaneCtrlPointing();
    void initDomeIcon();
    void initMarioIcon();
    void initTicoIcon();
    void tryFocusIn();
    void updateGalaxyName();
    void drawGalaxyIconWithOrder(int) const;
    void getPointingGalaxyIcon() const;

    MR::Vector<MR::AssignableArray< GalaxyMapIcon* > > mGalaxyMapIconArray; // 0x20
    MR::Vector<MR::AssignableArray< GalaxyMapCometIcon* > > mGalaxyMapCometIconArray; // 0x2C
    MR::Vector<MR::AssignableArray< GalaxyMapDomeIcon* > > mGalaxyMapDomeIconArray; // 0x38
    MR::Vector<MR::AssignableArray< GalaxyMapTicoIcon* > > mGalaxyMapTicoIconArray; // 0x44
    GalaxyMapMarioIcon* mGalaxyMapMarioIcon;
    u32 _54;
    GalaxyMapGalaxyPlain* mGalaxyMapGalaxyPlain; // 0x58
    GalaxyMapGalaxyDetail* mGalaxyMapGalaxyDetail; // 0x5C
    u32 _60;
    GalaxyMapTitle* mGalaxyMapTitle; // 0x64
    IconAButton* mIconAButton;
    u32 _6C;
    bool _70;
    bool _71;
};
