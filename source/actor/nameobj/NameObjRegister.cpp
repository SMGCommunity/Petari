#include "actor/nameobj/NameObjRegister.h"

NameObjRegister::NameObjRegister()
{
    this->mHolder = 0;
}

void NameObjRegister::add(NameObj* obj)
{
    this->mHolder->add(obj);
}

void NameObjRegister::setCurrentHolder(NameObjHolder* holder)
{
    this->mHolder = holder;
}