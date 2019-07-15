#ifndef NAMEOBJREGISTER_H
#define NAMEOBJREGISTER_H

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

#endif // NAMEOBJREGISTER_H