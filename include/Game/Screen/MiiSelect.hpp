#pragma once

#include "Game/Map/FileSelectIconID.hpp"
#include "Game/Screen/LayoutActor.hpp"

namespace nw4r {
    namespace lyt {
        class TexMap;
    };
};  // namespace nw4r

namespace MR {
    class BitArray;
};

namespace MiiSelectSub {
    class Page;
};

class ButtonPaneController;
class MiiSelectIcon;

class MiiSelect : public LayoutActor {
public:
    /// @brief Creates a new `MiiSelect`.
    /// @param pName A pointer to the null-terminated name of the object.
    MiiSelect(const char* pName);

    /// @brief Intializes the `MiiSelect` while being placed into a scene.
    /// @param[in] rIter The reference to an iterator over a `JMapInfo`.
    virtual void init(const JMapInfoIter& rIter);

    virtual void calcAnim();
    virtual void appear();
    virtual void control();

    void disappear();
    bool isAppearing() const;
    bool isSelected();
    bool isDummySelected();
    void getSelectedID(FileSelectIconID*);
    nw4r::lyt::TexMap* getSelectedMiiTexMap();
    void admitIcon();
    void prohibitIcon(const FileSelectIconID&);
    void invalidateSpecialMii(FileSelectIconID::EFellowID);
    void validateAllSpecialMii();
    void exeAppear();
    void exeWait();
    void exeScrollRight();
    void exeScrollLeft();
    void exeSelected();
    void exeDisappear();
    void exeDummySelected();
    void createButtons();
    void callbackLeft();
    void callbackRight();
    void appearButtons();
    void disappearButtons();
    void updateButtons();
    void collectValidMiiIndex();
    void createPage();
    void flipPage();
    void setCurrentPageGroupA();
    void setCurrentPageGroupB();
    void setCurrentPageNum();
    void validateAllIcon();
    void refresh();
    void getIconID(FileSelectIconID*, s32) const;
    void onSelect(s32, nw4r::lyt::TexMap*);
    void onSelectDummy();
    s32 getIconNum();

private:
    /* 0x020 */ ButtonPaneController* _20[2];
    /* 0x028 */ MR::BitArray* _28;
    /* 0x02C */ u16 _2C;
    /* 0x02E */ u16 _2E;
    /* 0x030 */ u32 _30[10];
    /* 0x058 */ u16 _58;
    /* 0x05C */ u32 _5C[10];
    /* 0x084 */ u8 _84[360];
    /* 0x1EC */ s32 _1EC;
    /* 0x1F0 */ s32 _1F0;
    /* 0x1F4 */ nw4r::lyt::TexMap* _1F4;
    /* 0x1F8 */ MiiSelectSub::Page* _1F8;
    /* 0x1FC */ MiiSelectSub::Page* _1FC;
    /* 0x200 */ u8 _200;
    /* 0x204 */ FileSelectIconID* _204;
};

namespace MiiSelectSub {
    class Page {
    public:
        /// @brief Creates a new `Page`.
        /// @param pHost A pointer to the owning actor instance.
        Page(MiiSelect* pHost);

        void refresh(s32);
        void movement();
        void calcAnim();
        void invalidateAllIcon();
        void validateAllIcon();
        void prohibitIcon(const FileSelectIconID&);

        /* 0x00 */ MiiSelectIcon* mIconArray[8];
        /* 0x20 */ bool _20;
        /* 0x24 */ MiiSelect* mHost;
        /* 0x28 */ s32 _28;
    };
};  // namespace MiiSelectSub
