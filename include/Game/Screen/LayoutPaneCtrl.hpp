#pragma once

#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry.hpp>
#include <revolution/types.h>

namespace nw4r {
    namespace lyt {
        class Pane;
    };  // namespace lyt
};  // namespace nw4r

class J3DFrameCtrl;
class LayoutAnmPlayer;
class LayoutManager;

class LayoutPaneCtrl {
public:
    LayoutPaneCtrl(LayoutManager* pHost, const char* pPaneName, u32 animLayerNum);

    void movement();
    void calcAnim();
    void start(const char* pAnimName, u32 layer);
    void stop(u32 layer);
    bool isAnimStopped(u32 layer) const;
    void reflectFollowPos();
    J3DFrameCtrl* getFrameCtrl(u32 layer) const;
    void recalcChildGlobalMtx(nw4r::lyt::Pane* pPane);

    /* 0x00 */ LayoutManager* mHost;
    /* 0x04 */ nw4r::lyt::Pane* mPane;
    /* 0x08 */ s32 mPaneIndex;
    /* 0x0C */ MR::AssignableArray< LayoutAnmPlayer* > mAnmPlayerArray;
    /* 0x14 */ s32 mFollowType;
    /* 0x18 */ const TVec2f* mFollowPos;
};
