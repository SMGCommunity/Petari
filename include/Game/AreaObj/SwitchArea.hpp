#pragma once

#include "Game/AreaObj/AreaObj.hpp"

class SwitchArea : public AreaObj {
public:
    SwitchArea(int, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x14 */ virtual void movement();

    /* 0x28 */ virtual const char* getManagerName() const;

    bool isUpdate() const;
};
