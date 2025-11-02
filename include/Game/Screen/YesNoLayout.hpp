#pragma once

#include "Game/Screen/SimpleLayout.hpp"

class YesNoController;

class YesNoLayout : public SimpleLayout {
public:
    /// @brief Creates a new `YesNoLayout`.
    YesNoLayout();

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void setInformationMessage(const char*);
    void setSelectMessage(const char*, const char*);
    void exeSelecting();

    /* 0x20 */ YesNoController* mController;
    /* 0x24 */ bool _24;
};
