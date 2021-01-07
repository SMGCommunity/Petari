#include "NameObj/NameObjRegister.h"

NameObjRegister::NameObjRegister()
{
    mHolder = 0;
}

void NameObjRegister::add(NameObj *pObj)
{
    mHolder->add(pObj);
}

void NameObjRegister::setCurrentHolder(NameObjHolder *pHolder)
{
   mHolder = pHolder;
}