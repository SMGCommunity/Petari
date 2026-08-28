#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class DrawInfo;
    };  // namespace lyt
};  // namespace nw4r

class GalaxyNamePlate;

class GalaxyMapDomeIcon : public LayoutActor {
public:
    GalaxyMapDomeIcon(int, LayoutActor*, const char*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x14 */ virtual void movement();
    /* 0x18 */ virtual void draw() const;
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void calcAnimForCapture(const nw4r::lyt::DrawInfo&);
    void setModeNormal();
    void setModeNewDomeDiscover();
    s32 calcDomeStatus() const;
    bool isComplete() const;
    void syncStatus();
    bool hasNewGalaxy() const;

    /* 0x20 */ int _20;
    /* 0x24 */ LayoutActor* mHost;
    /* 0x28 */ const char* mPaneName;
    /* 0x2C */ const char* _2C;
    /* 0x30 */ GalaxyNamePlate* mNamePlate;
    /* 0x34 */ u8 mMode;
};
