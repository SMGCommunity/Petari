#ifndef NAMEOBJREGISTER_H
#define NAMEOBJREGISTER_H

#include <revolution.h>
#include "NameObj/NameObj.h"
#include "NameObj/NameObjHolder.h"

class NameObjRegister
{
public:
    NameObjRegister();

    void add(NameObj *);
    void setCurrentHolder(NameObjHolder *);

    NameObjHolder* mHolder;
};

#endif // NAMEOBJREGISTER_H