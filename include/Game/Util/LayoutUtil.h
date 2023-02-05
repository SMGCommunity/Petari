#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class LayoutActor;

namespace MR {
    void createAndAddPaneCtrl(LayoutActor *, const char *, u32);

    void startPaneAnim(LayoutActor *, const char *, const char *, u32);

    void startAnim(LayoutActor *, const char *, u32);

    void setAnimFrameAndStop(LayoutActor *, f32, u32);
    void setPaneAnimFrameAndStop(LayoutActor *, const char *, f32, u32);

    void setAnimRate(LayoutActor *, f32, u32);

    f32 getAnimFrame(const LayoutActor *, u32);

    s16 getAnimFrameMax(const LayoutActor *, u32);
    
    bool isAnimStopped(const LayoutActor *, u32);

    void setFollowPos(const TVec2f *, const LayoutActor *, const char *);
    void setFollowPos(const TVec2f &, const LayoutActor *, const char *);

    void setFollowTypeAdd(const LayoutActor *, const char *);

    void copyPaneTrans(TVec2f *, const LayoutActor *, const char *);

    void convertLayoutPosToScreenPos(TVec2f *, const TVec2f &);

    void setTextBoxNumberRecursive(LayoutActor *, const char *, s32);
    void clearTextBoxMessageRecursive(LayoutActor *, const char *);

    void setTextBoxGameMessageRecursive(LayoutActor *, const char *, const char *);
    void setTextBoxLayoutMessageRecursive(LayoutActor *, const char *, const char *);
    void setTextBoxSystemMessageRecursive(LayoutActor *, const char *, const char *);
    void setTextBoxMessageRecursive(LayoutActor *, const char *, const wchar_t *);
    void setTextBoxFormatRecursive(LayoutActor *, const char *, const wchar_t *, ...);

    bool isFirstStep(const LayoutActor *);

    bool isDead(const LayoutActor *);

    bool isStoppedAnimFrame(const LayoutActor *);

    bool isExecuteCalcAnimLayout(const LayoutActor *);
    bool isExecuteDrawLayout(const LayoutActor *);
    
    void showLayout(LayoutActor *);
    void hideLayout(LayoutActor *);

    void showPane(LayoutActor *, const char *);
    void hidePane(LayoutActor *, const char *);

    bool isHiddenLayout(const LayoutActor *);

    void startAnimAndSetFrameAndStop(LayoutActor *, const char *, f32, u32);

    void emitEffect(LayoutActor *, const char *);

    void setCometAnimFromId(LayoutActor *, int, u32);
};
