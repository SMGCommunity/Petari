#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;
class NameObjArchiveListCollector;

class PeachLetter : public LayoutActor {
public:
    /// @brief Creates a new `PeachLetter`.
    /// @param pName The pointer to the null-terminated name of the object.
    PeachLetter(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();
    /* 0x24 */ virtual void appear();

    void exeAppear();
    void exeWait();
    void exeEnd();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

private:
    /// @brief The pointer to the actor instance for the A Button.
    /* 0x20 */ IconAButton* mAButtonIcon;
};
