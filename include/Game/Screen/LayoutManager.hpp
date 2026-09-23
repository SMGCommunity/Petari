#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <nw4r/lyt/drawInfo.h>
#include <nw4r/lyt/layout.h>
#include <nw4r/lyt/pane.h>

namespace nw4r {
    namespace lyt {
        class AnimTransform;
        class Group;
        class Layout;
        class Pane;
        class TexMap;
    };  // namespace lyt
};  // namespace nw4r

class LayoutGroupCtrl;
class LayoutHolder;
class LayoutPaneCtrl;

struct LayoutGroupCtrlLink {
    /* 0x00 */ LayoutGroupCtrl* mGroupCtrl;
    /* 0x04 */ LayoutGroupCtrlLink* mNext;
};

struct LayoutPaneInfo {
    /* 0x00 */ const char* mName;
    /* 0x04 */ LayoutPaneCtrl* mPaneCtrl;
    /* 0x08 */ LayoutGroupCtrlLink* mGroupCtrlLink;
    /* 0x0C */ MtxPtr mMtxRef;
    /* 0x10 */ u32 mChildCount;
    /* 0x14 */ nw4r::lyt::Pane* mPane;
};

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
    LayoutGroupCtrl* createAndAddGroupCtrl(const char*, u32);
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
    nw4r::lyt::Pane* getPane(const char* pName) const {
        if (pName == nullptr) {
            return mLayout->mpRootPane;
        } else {
            return mLayout->mpRootPane->FindPaneByName(pName, true);
        }
    }

    nw4r::lyt::Pane* findPaneByName(const char*) const;
    void replaceIndDummyTexture();
    void removeUnnecessaryPanes(nw4r::lyt::Pane*);

    /* 0x00 */ LayoutHolder* mLayoutHolder;
    /* 0x04 */ nw4r::lyt::Layout* mLayout;
    /* 0x08 */ nw4r::lyt::AnimTransform** mAnimTransList;
    /* 0x0C */ nw4r::lyt::DrawInfo mDrawInfo;
    /* 0x60 */ bool mIsScreenHidden;
    /* 0x61 */ bool _61;
    /* 0x64 */ nw4r::lyt::TexMap* mIndDummyTexMap;
    /* 0x68 */ u32 mPaneCount;
    /* 0x6C */ LayoutPaneInfo* mPaneInfoList;
    /* 0x70 */ u32 mGroupCtrlCount;
    /* 0x74 */ LayoutGroupCtrl** mGroupCtrlList;
    /* 0x78 */ char* mLayoutName;
};
