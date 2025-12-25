#pragma once

#include "Game/Util/Array.hpp"
#include <JSystem/JGeometry.hpp>
#include <revolution/types.h>

namespace nw4r {
    namespace lyt {
        class Pane;
    };
};  // namespace nw4r

class J3DFrameCtrl;
class LayoutAnmPlayer;
class LayoutManager;

class LayoutPaneCtrl {
public:
    /// @brief Creates a new `LayoutGroupCtrl`.
    /// @param pHost A pointer to the owning `LayoutManager` instance.
    /// @param pPaneName A pointer to the null-terminated name of the pane to associate with.
    /// @param animLayerNum The maximum number of animations to support at once.
    LayoutPaneCtrl(LayoutManager* pHost, const char* pPaneName, u32 animLayerNum);

    void movement();
    void calcAnim();
    void start(const char*, u32);
    void stop(u32);
    bool isAnimStopped(u32) const;
    void reflectFollowPos();
    J3DFrameCtrl* getFrameCtrl(u32) const;
    void recalcChildGlobalMtx(nw4r::lyt::Pane*);

    /* 0x00 */ LayoutManager* mHost;
    /* 0x04 */ nw4r::lyt::Pane* mPane;
    /* 0x08 */ s32 mPaneIndex;
    /* 0x0C */ MR::AssignableArray< LayoutAnmPlayer* > mAnmPlayerArray;
    /* 0x14 */ u32 mFollowType;
    /* 0x18 */ const TVec2f* mFollowPos;
};
