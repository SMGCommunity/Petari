#include "NameObj/NameObjHolder.h"
#include "MR/ObjUtil.h"

NameObjHolder::NameObjHolder(s32 a1)
{
    s32 temp = 0;
    mObjs = 0;
    _4 = 0;
    _8 = 0;
    _4C = 0;

    s16 size = a1 << 2;
    NameObj** pArr = new NameObj*[size];
    mObjs = pArr;

    _4 = a1;
}

void NameObjHolder::add(NameObj *pObj)
{
    mObjs[_8++] = pObj;
}

void NameObjHolder::suspendAllObj()
{
    u32 curObjIdx = 0;

    while (curObjIdx < _8)
    {
        MR::requestMovementOff(mObjs[curObjIdx]);
        curObjIdx++;
    }
}

void NameObjHolder::resumeAllObj()
{
    u32 curObjIdx = 0;

    while (curObjIdx < _8)
    {
        MR::requestMovementOn(mObjs[curObjIdx]);
        curObjIdx++;
    }
}

/* TODO -- callMethodAllObj() / syncWithFlags() / find() */

void NameObjHolder::clearArray()
{
    u32 temp = 0;
    _8 = 0;
    _4C = 0;
}