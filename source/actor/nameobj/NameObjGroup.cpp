#include "actor/nameobj/NameObjGroup.h"

NameObjGroup::NameObjGroup(const char *name, s32 len) : NameObj(name)
{
    this->mNumObjs = 0;
    this->_10 = 0;
    this->mObjs = 0;

    this->initObjArray(len);
}

void NameObjGroup::registerObj(NameObj *obj)
{
    // TODO
}