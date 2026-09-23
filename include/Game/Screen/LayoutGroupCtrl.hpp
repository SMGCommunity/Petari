#pragma once

#include "Game/Util/Array.hpp"
#include <revolution/types.h>

namespace nw4r {
    namespace lyt {
        class Pane;
        class Group;
    };  // namespace lyt
};  // namespace nw4r

class LayoutAnmPlayer;
class LayoutManager;

class LayoutGroupCtrl {
public:
    LayoutGroupCtrl(LayoutManager* pManager, const char* pName, u32 animLayerCount);

    void movement();
    void calcAnim();
    u32 getPaneNum() const;
    nw4r::lyt::Pane* getPane(u32 idx) const;

    /* 0x00 */ LayoutManager* mManager;
    /* 0x04 */ nw4r::lyt::Group* mGroup;
    /* 0x08 */ MR::AssignableArray< LayoutAnmPlayer* > mAnmPlayerArray;
    /* 0x10 */ bool _10;
};
