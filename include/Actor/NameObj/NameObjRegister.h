#ifndef NAMEOBJREGISTER_H
#define NAMEOBJREGISTER_H

#include <revolution.h>
#include "Actor/NameObj/NameObj.h"
#include "Actor/NameObj/NameObjHolder.h"

class NameObjRegister
{
public:
    NameObjRegister();

    void add(NameObj *);
    void setCurrentHolder(NameObjHolder *);

    NameObjHolder* mHolder;
};

#endif // NAMEOBJREGISTER_H