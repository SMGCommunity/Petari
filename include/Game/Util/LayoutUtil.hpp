#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.hpp"

class Layout;
class Nerve;
class LayoutActor;
class IconAButton;

namespace MR {
    void setEffectRate(LayoutActor*, const char*, f32);
    void setEffectDirectionalSpeed(LayoutActor*, const char*, f32);

    void createAndAddPaneCtrl(LayoutActor*, const char*, u32);
    bool isExistPaneCtrl(LayoutActor*, const char*);

    void startPaneAnim(LayoutActor*, const char*, const char*, u32);
    void startPaneAnimAtStep(LayoutActor*, const char*, const char*, s32, u32);

    void showPaneRecursive(LayoutActor*, const char*);

    void startAnim(LayoutActor*, const char*, u32);

    void setAnimFrameAndStop(LayoutActor*, f32, u32);
    void setPaneAnimFrameAndStop(LayoutActor*, const char*, f32, u32);

    void setAnimRate(LayoutActor*, f32, u32);
    void setPaneAnimRate(LayoutActor*, const char*, f32, u32);

    f32 getAnimFrame(const LayoutActor*, u32);

    s16 getAnimFrameMax(const LayoutActor*, u32);
    s16 getAnimFrameMax(const LayoutActor*, const char*);

    bool isAnimStopped(const LayoutActor*, u32);
    bool isPaneAnimStopped(const LayoutActor*, const char*, u32);

    f32 getPaneAnimFrame(const LayoutActor*, const char*, u32);

    void setFollowPos(const TVec2f*, const LayoutActor*, const char*);

    void setFollowTypeReplace(const LayoutActor*, const char*);
    void setFollowTypeAdd(const LayoutActor*, const char*);

    void copyPaneTrans(TVec2f*, const LayoutActor*, const char*);

    void convertLayoutPosToScreenPos(TVec2f*, const TVec2f&);

    void setTextBoxNumberRecursive(LayoutActor*, const char*, s32);
    void clearTextBoxMessageRecursive(LayoutActor*, const char*);

    void setTextBoxGameMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxLayoutMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxSystemMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxMessageRecursive(LayoutActor*, const char*, const wchar_t*);
    void setTextBoxFormatRecursive(LayoutActor*, const char*, const wchar_t*, ...);

    void setTextBoxArgNumberRecursive(LayoutActor*, const char*, s32, s32);
    void setTextBoxArgStringRecursive(LayoutActor*, const char*, const wchar_t*, s32);
    void setTextBoxVerticalPositionTopRecursive(LayoutActor*, const char*);
    void setTextBoxVerticalPositionCenterRecursive(LayoutActor*, const char*);
    void setTextBoxVerticalPositionBottomRecursive(LayoutActor*, const char*);

    bool isFirstStep(const LayoutActor*);
    bool isGreaterStep(const LayoutActor*, s32);

    bool isDead(const LayoutActor*);

    bool isStoppedAnimFrame(const LayoutActor*);

    bool isExecuteCalcAnimLayout(const LayoutActor*);
    bool isExecuteDrawLayout(const LayoutActor*);

    void showLayout(LayoutActor*);
    void hideLayout(LayoutActor*);

    void showPane(LayoutActor*, const char*);
    void hidePane(LayoutActor*, const char*);
    void hidePaneRecursive(LayoutActor*, const char*);
    void showPaneRecursive(LayoutActor*, const char*);

    bool isHiddenLayout(const LayoutActor*);

    void startAnimAndSetFrameAndStop(LayoutActor*, const char*, f32, u32);

    void emitEffect(LayoutActor*, const char*);

    IconAButton* createAndSetupIconAButton(LayoutActor*, bool, bool);

    void setCometAnimFromId(LayoutActor*, int, u32);

    void setCometPaneAnimFromId(LayoutActor*, const char*, int, u32);

    void createAndAddLayoutHolderRawData(const char*);

    void setEffectHostMtx(LayoutActor*, const char*, MtxPtr);

    void addStarPointerTargetCircle(LayoutActor*, const char*, f32, const TVec2f&, const char*);

    void startPaneAnimAtFirstStep(LayoutActor*, const char*, const char*, u32);

    void setNerveAtPaneAnimStopped(LayoutActor*, const char*, const Nerve*, u32);

    void setNerveAtStep(LayoutActor*, const Nerve*, s32);

    void setTextBoxGameMessageRecursive(LayoutActor*, const char*, const char*);
    void setTextBoxArgGameMessageRecursive(LayoutActor*, const char*, const char*, s32);

    bool isStep(const LayoutActor*, s32);

    void hideLayout(LayoutActor*);

    f32 calcNerveEaseInValue(const LayoutActor*, s32, s32, f32, f32);

    void setNerveAtAnimStopped(LayoutActor*, const Nerve*, u32);
};  // namespace MR
