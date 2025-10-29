#pragma once

#include <nw4r/lyt/drawInfo.h>

namespace nw4r {
    namespace lyt {
        class AnimTransform;
        class Layout;
        class Pane;
    };
};

class LayoutPaneCtrl;

class LayoutManager {
public:
    LayoutManager(const char *, bool, u32, u32);

    void movement();
    void calcAnim();
    void draw() const;

    nw4r::lyt::AnimTransform* getAnimTransform(const char *) const;
    void bindPaneCtrlAnim(LayoutPaneCtrl* pPaneCtrl, nw4r::lyt::AnimTransform*);
    void unbindPaneCtrlAnim(LayoutPaneCtrl*, nw4r::lyt::AnimTransform*);
    nw4r::lyt::Pane* getPane(const char *) const;

    u32 _0;
    nw4r::lyt::Layout* mLayout;     // 0x4
    nw4r::lyt::AnimTransform** mAnimTransList;
    nw4r::lyt::DrawInfo mDrawInfo;  // 0xC
    bool mIsScreenHidden;           // 0x60
    u8 _61;
    u8 _62;
    u8 _63;
    u32 _64;
    u32 _68;
    u32 _6C;
    u32 _70;
    u32 _74;
    u32 _78;
};
