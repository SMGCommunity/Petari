#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class DrawInfo;
    };
};  // namespace nw4r

class GalaxyNamePlate;

class GalaxyMapDomeIcon : public LayoutActor {
public:
    GalaxyMapDomeIcon(int, LayoutActor*, const char*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();

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
