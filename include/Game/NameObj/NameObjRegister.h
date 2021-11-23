#pragma once

#include "Game/NameObj/NameObjHolder.h"

class NameObjRegister {
public:
    NameObjRegister();

    void setCurrentHolder(NameObjHolder *);
    void add(NameObj *);

    NameObjHolder* mHolder; // _0
};