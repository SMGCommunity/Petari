#include "NameObj/NameObjCategoryList.h"

NameObjCategoryList::CategoryInfo::CategoryInfo()
{
    mContent = 0;
    mCount = 0;
    _8 = 0;
}

void NameObjCategoryList::incrementCheck(NameObj *pObj, s32 idx)
{
    mCategoryInfo[idx].mChecks++;
}

void NameObjCategoryList::add(NameObj *pObj, s32 idx)
{
    CategoryInfo* info = &mCategoryInfo[idx];
    int val = info->_8++;
    info->mContent[val] = pObj;
}