#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <nw4r/lyt/drawInfo.h>

namespace nw4r {
    namespace lyt {
        class AnimTransform;
        class Group;
        class Layout;
        class Pane;
    };  // namespace lyt
};  // namespace nw4r

class LayoutGroupCtrl;
class LayoutHolder;
class LayoutPaneCtrl;

class LayoutManager {
public:
    LayoutManager(const char*, bool, u32, u32);

    void movement();
    void calcAnim();
    void draw() const;
    void addPaneCtrl(LayoutPaneCtrl*);
    LayoutPaneCtrl* createAndAddRootPaneCtrl(u32);
    LayoutPaneCtrl* createAndAddPaneCtrl(const char*, u32);
    LayoutPaneCtrl* getPaneCtrl(const char*) const;
    s32 getIndexOfPane(const char*) const;
    bool isExistPaneCtrl(const char*) const;
    void addGroupCtrl(LayoutGroupCtrl*);
    bool isPointing(const nw4r::lyt::Pane*, const TVec2f&) const;
    LayoutPaneCtrl* createAndAddGroupCtrl(const char*, u32);
    s32 getIndexOfGroupCtrl(const char*) const;
    void createPaneMtxRef(const char*);
    MtxPtr getPaneMtxRef(const char*) const;
    bool isExistPaneMtxRef(const char*) const;
    bool isPointing(const char*, const TVec2f&) const;
    nw4r::lyt::AnimTransform* getAnimTransform(const char*) const;
    void bindPaneCtrlAnim(LayoutPaneCtrl*, nw4r::lyt::AnimTransform*);
    void bindPaneCtrlAnimSub(u32&, nw4r::lyt::AnimTransform*);
    void unbindPaneCtrlAnim(LayoutPaneCtrl*, nw4r::lyt::AnimTransform*);
    void unbindPaneCtrlAnimSub(u32&, nw4r::lyt::AnimTransform*);
    void calcAnimWithoutLocationAdjust(const nw4r::lyt::DrawInfo&);
    nw4r::lyt::Group* getGroup(const char*) const;
    void initArc(const char*, const char*);
    void initDrawInfo();
    void initPaneInfo();
    void initPaneInfoRecursive(u32&, nw4r::lyt::Pane*);
    u32 countPanes(nw4r::lyt::Pane*);
    void initGroupCtrlList();
    void initTextBoxRecursive(nw4r::lyt::Pane*, nw4r::lyt::Pane*, const char*, u32);
    void animateRecursive(u32&, nw4r::lyt::Pane*);
    nw4r::lyt::Pane* getPane(const char*) const;
    nw4r::lyt::Pane* findPaneByName(const char*) const;
    void replaceIndDummyTexture();
    void removeUnnecessaryPanes(nw4r::lyt::Pane*);

    /* 0x00 */ LayoutHolder* mLayoutHolder;
    /* 0x04 */ nw4r::lyt::Layout* mLayout;
    /* 0x08 */ nw4r::lyt::AnimTransform** mAnimTransList;
    /* 0x0C */ nw4r::lyt::DrawInfo mDrawInfo;
    /* 0x60 */ bool mIsScreenHidden;
    /* 0x61 */ bool _61;
    /* 0x64 */ u32 _64;
    /* 0x68 */ u32 _68;
    /* 0x6C */ u32 _6C;
    /* 0x70 */ u32 _70;
    /* 0x74 */ u32 _74;
    /* 0x78 */ u32 _78;
};
