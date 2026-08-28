#pragma once

#include "Game/Screen/SimpleLayout.hpp"

class YesNoController;

class YesNoLayout : public SimpleLayout {
public:
    /// @brief Creates a new `YesNoLayout`.
    YesNoLayout();

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x24 */ virtual void appear();
    /* 0x28 */ virtual void kill();
    /* 0x2C */ virtual void control();

    void setInformationMessage(const char*);
    void setSelectMessage(const char*, const char*);
    void exeSelecting();

    /* 0x20 */ YesNoController* mController;
    /* 0x24 */ bool _24;
};
