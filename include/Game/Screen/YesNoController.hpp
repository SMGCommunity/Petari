#pragma once

#include "Game/Screen/LayoutActor.hpp"
#include "Game/Screen/ButtonPaneController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/System/NerveExecutor.hpp"

class YesNoController : public NerveExecutor {
public: 
    YesNoController(LayoutActor *);
    virtual ~YesNoController();
    void appear();
    void kill();
    void update();
    bool isSelected() const;
    bool isSelectedYes() const;
    bool isDisappearStart() const;
    void setSE(const char*,const char*,const char*);
    bool trySelect();
    void emitEffectIfExist(const char*);
    void deleteEffectIfExist(const char*);
    void forceDeleteEffectAllIfExist();
    void exeSelecting();
    void exeDecided();
    void exeDisappear();

    LayoutActor* mHost; // 0x8
    bool _C;
    ButtonPaneController* mButtonRight; // 0x10
    ButtonPaneController* mButtonLeftt; // 0x14
    s32 _18;
    s32 _1C;
    s32 _20;
};

namespace NrvYesNoController {
    NERVE(YesNoControllerNrvNotSelected);
    NERVE(YesNoControllerNrvSelected);
    NERVE(YesNoControllerNrvDisappear);
    NERVE(YesNoControllerNrvDecided);
    NERVE(YesNoControllerNrvSelecting);
};