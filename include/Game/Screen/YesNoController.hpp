#pragma once

#include "Game/System/NerveExecutor.hpp"

class ButtonPaneController;
class LayoutActor;

class YesNoController : public NerveExecutor {
public:
    /// @brief Creates a new `YesNoController`.
    /// @param pHost The pointer to the owning actor instance.
    YesNoController(LayoutActor* pHost);

    void appear();
    void kill();
    void update();
    bool isSelected() const;
    bool isSelectedYes() const;
    bool isDisappearStart() const;
    void setSE(const char*, const char*, const char*);
    bool trySelect();
    void emitEffectIfExist(const char*);
    void deleteEffectIfExist(const char*);
    void forceDeleteEffectAllIfExist();
    void exeSelecting();
    void exeDecided();
    void exeDisappear();
    void exeSelected();
    void exeNotSelected();

    /* 0x08 */ LayoutActor* mHost;
    /* 0x0C */ bool _C;
    /* 0x10 */ ButtonPaneController* mButtonYesPaneCtrl;
    /* 0x14 */ ButtonPaneController* mButtonNoPaneCtrl;
    /* 0x18 */ const char* mCursorSE;
    /* 0x1C */ const char* mYesSE;
    /* 0x20 */ const char* mNoSE;
};
