#pragma once

#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class DrawInfo;
    };  // namespace lyt
};  // namespace nw4r

class GalaxyMapCometIcon : public LayoutActor {
public:
    GalaxyMapCometIcon(const char*, LayoutActor*, const char*);

    /* 0x08 */ virtual ~GalaxyMapCometIcon();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void control();

    void calcAnimForCapture(const nw4r::lyt::DrawInfo&);
    void syncStatus();

private:
    /* 0x20 */ const char* _20;
    /* 0x24 */ LayoutActor* _24;
    /* 0x28 */ const char* _28;
};
