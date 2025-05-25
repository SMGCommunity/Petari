#pragma once

#include "Game/Screen/LayoutActor.hpp"

class ButtonPaneController;
class GalaxyMapGalaxyPlain;

namespace MR {
    class FunctorBase;
};

class FileSelectButton : public LayoutActor {
public:
    /// @brief Creates a new `FileSelectButton`.
    /// @param pName The pointer to the null-terminated name of the object.
    FileSelectButton(const char* pName);

    /// @brief Destroys the `FileSelectButton`.
    virtual ~FileSelectButton() {}

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void disappear();
    void setCallbackFunctor(
        const MR::FunctorBase&,
        const MR::FunctorBase&,
        const MR::FunctorBase&,
        const MR::FunctorBase&,
        const MR::FunctorBase&);
    void shiftSelect();
    void exeSelect();
    void exeWait();
    void exeDisappear();
    void createPaneControl();
    void createButtonController();
    void createOthers();

private:
    /* 0x20 */ ButtonPaneController* mButtonCtrl[5];
    /* 0x34 */ MR::FunctorBase* mCallbackFunctor[5];
    /* 0x48 */ GalaxyMapGalaxyPlain* _48;
};
