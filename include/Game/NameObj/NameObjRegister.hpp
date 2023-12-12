#pragma once

#include "Game/NameObj/NameObjHolder.h"

/// @brief Class that stores every NameObj instance in a scene.
class NameObjRegister {
public:
    NameObjRegister();

    void setCurrentHolder(NameObjHolder *);
    void add(NameObj *);

    NameObjHolder* mHolder; // _0
};