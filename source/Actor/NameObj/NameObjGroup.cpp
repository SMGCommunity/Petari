#include "actor/NameObj/NameObjGroup.h"
#include "os/gpr.h"
#include "MR/util.h"

NameObjGroup::NameObjGroup(const char *name, s32 len) : NameObj(name)
{
    this->_C = 0;
    this->mNumObjs = 0;
    this->mObjs = 0;

    this->initObjArray(len);
}

void NameObjGroup::registerObj(NameObj *obj)
{
    *(NameObj**)(this->mObjs + this->mNumObjs) = obj;
    this->mNumObjs++;
}

void NameObjGroup::pauseOffAll() const
{
    _savegpr_29();

    u32 curObjIdx = 0;

    while (curObjIdx < this->mNumObjs)
    {
        MR::requestMovementOn(this->mObjs[curObjIdx]);
        curObjIdx++;
    }

    _restgpr_29();
}

void NameObjGroup::initObjArray(s32 numObjs)
{
    this->_C = numObjs;
    s16 size = numObjs << 2;
    NameObj** arr = new NameObj*[size];
    this->mObjs = arr;

    u32 curObjIdx = 0;

    while (curObjIdx < this->_C)
    {
        curObjIdx++;
        this->mObjs[curObjIdx] = 0;
    }
}