#pragma once

#include "types.h"
#include "actor/nameobj/NameObj.h"

class NameObjHolder
{
public:
    NameObjHolder();

    void add(NameObj *);
};