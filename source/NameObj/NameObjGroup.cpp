#include "NameObj/NameObjGroup.h"
#include "Util/ObjUtil.h"

NameObjGroup::NameObjGroup(const char *pName, s32 len) : NameObj(pName)
{
    _C = 0;
    mNumObjs = 0;
    mObjs = 0;

    initObjArray(len);
}

void NameObjGroup::registerObj(NameObj *pObj)
{
    mObjs[mNumObjs] = pObj;
    mNumObjs++;
}

void NameObjGroup::pauseOffAll() const
{
    u32 curObjIdx = 0;

    while (curObjIdx < mNumObjs)
    {
        MR::requestMovementOn(mObjs[curObjIdx]);
        curObjIdx++;
    }
}

void NameObjGroup::initObjArray(s32 numObjs)
{
    _C = numObjs;
    s16 size = numObjs << 2;
    NameObj** pArr = new NameObj*[size];
    mObjs = pArr;

    u32 curObjIdx = 0;

    while (curObjIdx < _C)
    {
        curObjIdx++;
        mObjs[curObjIdx] = 0;
    }
}