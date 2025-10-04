#pragma once

#include <JSystem/JGeometry/TBox.hpp>
#include <revolution.h>

class IconAButton;
class J3DFrameCtrl;
class LayoutActor;
class LayoutHolder;
class Nerve;
class ResTIMG;
class SimpleLayout;

namespace nw4r {
    namespace lyt {
        class DrawInfo;
        class Pane;
        class TexMap;
        class TextBox;
    };

    namespace ut {
        class Font;
        class Rect;
    };
};

class TextBoxRecursiveOperation {
public:
    /// @brief Destroys the `TextBoxRecursiveOperation`.
    virtual ~TextBoxRecursiveOperation() {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const = 0;
};

class TextBoxRecursiveSetMessage : public TextBoxRecursiveOperation {
public:
    /// @brief Creates a new `TextBoxRecursiveSetMessage`.
    TextBoxRecursiveSetMessage(const wchar_t* pMessage) :
        mMessage(pMessage)
    {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const;

private:
    /* 0x4 */ const wchar_t* mMessage;
};

class TextBoxRecursiveSetArgNumber : public TextBoxRecursiveOperation {
public:
    /// @brief Creates a new `TextBoxRecursiveSetArgNumber`.
    TextBoxRecursiveSetArgNumber(s32 arg, s32 param2) :
        mArg(arg),
        _8(param2)
    {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const;

private:
    /* 0x4 */ s32 mArg;
    /* 0x8 */ s32 _8;
};

class TextBoxRecursiveSetArgString : public TextBoxRecursiveOperation {
public:
    /// @brief Creates a new `TextBoxRecursiveSetArgString`.
    TextBoxRecursiveSetArgString(const wchar_t* pArg, s32 param2) :
        mArg(pArg),
        _8(param2)
    {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const;

private:
    /* 0x4 */ const wchar_t* mArg;
    /* 0x8 */ s32 _8;
};

class TextBoxRecursiveSetVerticalPosition : public TextBoxRecursiveOperation {
public:
    /// @brief Creates a new `TextBoxRecursiveSetVerticalPosition`.
    TextBoxRecursiveSetVerticalPosition(u8 position) :
        mPosition(position)
    {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const;

private:
    /* 0x4 */ u8 mPosition;
};

class TextBoxRecursiveSetHorizontalPosition : public TextBoxRecursiveOperation {
public:
    /// @brief Creates a new `TextBoxRecursiveSetHorizontalPosition`.
    TextBoxRecursiveSetHorizontalPosition(u8 position) :
        mPosition(position)
    {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const;

private:
    /* 0x4 */ u8 mPosition;
};

class TextBoxRecursiveSetFont : public TextBoxRecursiveOperation {
public:
    /// @brief Creates a new `TextBoxRecursiveSetFont`.
    TextBoxRecursiveSetFont(nw4r::ut::Font* pFont) :
        mFont(pFont)
    {}

    virtual void execute(nw4r::lyt::TextBox* pTextBox) const;

private:
    /* 0x4 */ nw4r::ut::Font* mFont;
};

namespace MR {
    LayoutHolder* createAndAddLayoutHolder(const char*);
    LayoutHolder* createAndAddLayoutHolderRawData(const char*);
    LayoutHolder* createAndAddPaneCtrl(LayoutActor*, const char*, u32);
    LayoutHolder* createAndAddGroupCtrl(LayoutActor*, const char*, u32);
    bool isExistPaneCtrl(LayoutActor*, const char*);
    u8 getPaneAlpha(const LayoutActor*, const char*);
    void setInfluencedAlphaToChild(const LayoutActor*);
    void setLayoutAlpha(const LayoutActor*, u8);
    void setLayoutAlphaFloat(const LayoutActor*, f32);
    void setPaneAlpha(const LayoutActor*, const char*, u8);
    void setPaneAlphaFloat(const LayoutActor*, const char*, f32);
    void executeTextBoxRecursive(LayoutActor*, const char*, const TextBoxRecursiveOperation&);
    void setTextBoxGameMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxLayoutMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxSystemMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxMessageRecursive(LayoutActor*, const char*, const wchar_t*);
    void setTextBoxFormatRecursive(LayoutActor*, const char*, const wchar_t*, ...);
    void setTextBoxArgNumberRecursive(LayoutActor*, const char*, s32, s32);
    void setTextBoxArgStringRecursive(LayoutActor*, const char*, const wchar_t*, s32);
    void setTextBoxArgGameMessageRecursive(LayoutActor*, const char*, const char*, s32);
    void setTextBoxVerticalPositionTopRecursive(LayoutActor*, const char*);
    void setTextBoxVerticalPositionCenterRecursive(LayoutActor*, const char*);
    void setTextBoxVerticalPositionBottomRecursive(LayoutActor*, const char*);
    void setTextBoxHorizontalPositionLeftRecursive(LayoutActor*, const char*);
    void setTextBoxHorizontalPositionCenterRecursive(LayoutActor*, const char*);
    void updateClearTimeTextBox(LayoutActor*, const char*, u32);
    void updateMinuteAndSecondTextBox(LayoutActor*, const char*, u32);
    void setTextBoxFontRecursive(LayoutActor*, const char*, nw4r::ut::Font*);
    void showPane(LayoutActor*, const char*);
    void showPaneRecursive(LayoutActor*, const char*);
    void hidePane(LayoutActor*, const char*);
    void hidePaneRecursive(LayoutActor*, const char*);
    bool isHiddenPane(const LayoutActor*, const char*);
    void showScreen(LayoutActor*);
    void hideScreen(LayoutActor*);
    void setFollowPos(const TVec2f*, const LayoutActor*, const char*);
    void setFollowTypeReplace(const LayoutActor*, const char*);
    void setFollowTypeAdd(const LayoutActor*, const char*);
    void copyPaneTrans(TVec2f*, const LayoutActor*, const char*);
    f32 getPaneTransX(const LayoutActor*, const char*);
    f32 getPaneTransY(const LayoutActor*, const char*);
    void setLayoutPosAtPaneTrans(LayoutActor*, const LayoutActor*, const char*);
    void copyPaneScale(TVec2f*, const LayoutActor*, const char*);
    void setPaneScale(const LayoutActor*, f32, f32, const char*);
    void setLayoutScaleAtPaneScale(LayoutActor*, const LayoutActor*, const char*);
    void copyPaneRotate(TVec3f*, const LayoutActor*, const char*);
    void setPaneRotate(const LayoutActor*, f32, f32, f32, const char*);
    void setLayoutScalePosAtPaneScaleTrans(LayoutActor*, const LayoutActor*, const char*);
    void setLayoutScalePosAtPaneScaleTransIfExecCalcAnim(LayoutActor*, const LayoutActor*, const char*);
    nw4r::lyt::TexMap* createLytTexMap(const char*, const char*);
    nw4r::lyt::TexMap* createLytTexMap(ResTIMG*);
    nw4r::lyt::TexMap* getLytTexMap(LayoutActor*, const char*, u8);
    void replacePaneTexture(LayoutActor*, const char*, const nw4r::lyt::TexMap*, u8);
    void startAnim(LayoutActor*, const char*, u32);
    void startAnimAtFirstStep(LayoutActor*, const char*, u32);
    void startPaneAnim(LayoutActor*, const char*, const char*, u32);
    void startPaneAnimAtStep(LayoutActor*, const char*, const char*, s32, u32);
    void startPaneAnimAtFirstStep(LayoutActor*, const char*, const char*, u32);
    void startAnimReverseOneTime(LayoutActor*, const char*, u32);
    void startPaneAnimReverseOneTime(LayoutActor*, const char*, const char*, u32);
    void startAnimAndSetFrameAndStop(LayoutActor*, const char*, f32, u32);
    void setAnimFrameAndStop(LayoutActor*, f32, u32);
    void setAnimFrameAndStopAtEnd(LayoutActor*, u32);
    void setAnimFrameAndStopAdjustTextWidth(LayoutActor*, const char*, u32);
    void setAnimFrameAndStopAdjustTextHeight(LayoutActor*, const char*, u32);
    void setPaneAnimFrameAndStop(LayoutActor*, const char*, f32, u32);
    void setPaneAnimFrameAndStopAtEnd(LayoutActor*, const char*, u32);
    void setAnimFrame(LayoutActor*, f32, u32);
    void setPaneAnimFrame(LayoutActor*, const char*, f32, u32);
    void setAnimRate(LayoutActor*, f32, u32);
    void setPaneAnimRate(LayoutActor*, const char*, f32, u32);
    void stopAnim(LayoutActor*, u32);
    void stopPaneAnim(LayoutActor*, const char*, u32);
    bool isAnimStopped(const LayoutActor*, u32);
    bool isPaneAnimStopped(const LayoutActor*, const char*, u32);
    f32 getAnimFrame(const LayoutActor*, u32);
    f32 getPaneAnimFrame(const LayoutActor*, const char*, u32);
    s16 getAnimFrameMax(const LayoutActor*, u32);
    s16 getPaneAnimFrameMax(const LayoutActor*, const char*, u32);
    s16 getAnimFrameMax(const LayoutActor*, const char*);
    J3DFrameCtrl* getAnimCtrl(const LayoutActor*, u32);
    J3DFrameCtrl* getPaneAnimCtrl(const LayoutActor*, const char*, u32);
    void emitEffect(LayoutActor*, const char*);
    void deleteEffect(LayoutActor*, const char*);
    void forceDeleteEffect(LayoutActor*, const char*);
    void deleteEffectAll(LayoutActor*);
    void forceDeleteEffectAll(LayoutActor*);
    void setEffectHostMtx(LayoutActor*, const char*, MtxPtr);
    void setEffectRate(LayoutActor*, const char*, f32);
    void setEffectDirectionalSpeed(LayoutActor*, const char*, f32);
    void pauseOffEffectAll(LayoutActor*);
    bool isRegisteredEffect(const LayoutActor*, const char*);
    void copyLayoutDrawInfoWithAspect(nw4r::lyt::DrawInfo*, const LayoutActor*, bool);
    void calcAnimLayoutWithDrawInfo(const LayoutActor*, const nw4r::lyt::DrawInfo&);
    void drawLayoutWithDrawInfoWithoutProjectionSetup(const LayoutActor*, const nw4r::lyt::DrawInfo&);
    bool isStep(const LayoutActor*, s32) NO_INLINE;
    bool isFirstStep(const LayoutActor*);
    bool isLessStep(const LayoutActor*, s32);
    bool isGreaterStep(const LayoutActor*, s32);
    bool isGreaterEqualStep(const LayoutActor*, s32);
    bool isIntervalStep(const LayoutActor*, s32);
    bool isNewNerve(const LayoutActor*) NO_INLINE;
    f32 calcNerveRate(const LayoutActor*, s32);
    f32 calcNerveRate(const LayoutActor*, s32, s32);
    f32 calcNerveEaseInRate(const LayoutActor*, s32);
    f32 calcNerveEaseInValue(const LayoutActor*, s32, s32, f32, f32);
    void setNerveAtStep(LayoutActor*, const Nerve*, s32);
    void setNerveAtAnimStopped(LayoutActor*, const Nerve*, u32);
    void setNerveAtPaneAnimStopped(LayoutActor*, const char*, const Nerve*, u32);
    void killAtAnimStopped(LayoutActor*, u32);
    bool isDead(const LayoutActor*);
    bool isHiddenLayout(const LayoutActor*);
    void showLayout(LayoutActor*);
    void hideLayout(LayoutActor*);
    bool isStopAnimFrame(const LayoutActor*);
    void stopAnimFrame(LayoutActor*);
    void releaseAnimFrame(LayoutActor*);
    void onCalcAnim(LayoutActor*);
    void offCalcAnim(LayoutActor*);
    bool isExecuteCalcAnimLayout(const LayoutActor*);
    bool isExecuteDrawLayout(const LayoutActor*);
    SimpleLayout* createSimpleLayout(const char*, const char*, u32);
    SimpleLayout* createSimpleLayoutTalkParts(const char*, const char*, u32);
    nw4r::lyt::Pane* getPane(const LayoutActor*, const char*);
    nw4r::lyt::Pane* getRootPane(const LayoutActor*);
    void calcTextBoxRectRecursive(TBox2f*, const LayoutActor*, const char*);
    u32 getTextLineNumMaxRecursive(const LayoutActor*, const char*);
    void invalidateParentAnim(LayoutActor*);
    void setCometPaneAnimFromId(LayoutActor*, const char*, int, u32);
    void setTextBoxNumberRecursive(LayoutActor*, const char*, s32);
    void clearTextBoxMessageRecursive(LayoutActor*, const char*);
    IconAButton* createAndSetupIconAButton(LayoutActor*, bool, bool);
    void setCometAnimFromId(LayoutActor*, int, u32);
};
