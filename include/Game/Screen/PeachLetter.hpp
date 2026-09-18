#pragma once

#include "Game/Screen/LayoutActor.hpp"

class IconAButton;
class NameObjArchiveListCollector;

class PeachLetter : public LayoutActor {
public:
    /// @brief Creates a new `PeachLetter`.
    /// @param pName The pointer to the null-terminated name of the object.
    PeachLetter(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void movement();
    virtual void appear();

    void exeAppear();
    void exeWait();
    void exeEnd();
    static void makeArchiveList(NameObjArchiveListCollector*, const JMapInfoIter&);

public:
    /// @brief The pointer to the actor instance for the A Button.
    /* 0x20 */ IconAButton* mAButtonIcon;
};
