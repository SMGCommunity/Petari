#include "actor/NameObj/NameObjHolder.h"
#include "MR/ObjUtil.h"
#include "os/gpr.h"

NameObjHolder::NameObjHolder(s32 a1)
{
    s32 temp = 0;
    this->mObjs = 0;
    this->_4 = 0;
    this->_8 = 0;
    this->_4C = 0;

    s16 size = a1 << 2;
    NameObj** arr = new NameObj*[size];
    this->mObjs = arr;

    this->_4 = a1;
}

void NameObjHolder::add(NameObj *obj)
{
    this->mObjs[this->_8++] = obj;
}

void NameObjHolder::suspendAllObj()
{
    _savegpr_29();

    u32 curObjIdx = 0;

    while (curObjIdx < this->_8)
    {
        MR::requestMovementOff(this->mObjs[curObjIdx]);
        curObjIdx++;
    }

    _restgpr_29();
}

void NameObjHolder::resumeAllObj()
{
    _savegpr_29();

    u32 curObjIdx = 0;

    while (curObjIdx < this->_8)
    {
        MR::requestMovementOn(this->mObjs[curObjIdx]);
        curObjIdx++;
    }

    _restgpr_29();
}

/* TODO -- callMethodAllObj() / syncWithFlags() / find() */

void NameObjHolder::clearArray()
{
    u32 temp = 0;
    this->_8 = 0;
    this->_4C = 0;
}