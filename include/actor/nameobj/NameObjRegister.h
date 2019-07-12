#pragma once

#include "types.h"
#include "actor/nameobj/NameObj.h"
#include "actor/nameobj/NameObjHolder.h"

class NameObjRegister
{
public:
    NameObjRegister();

    void add(NameObj *);
    void setCurrentHolder(NameObjHolder *);

    NameObjHolder* mHolder;
};