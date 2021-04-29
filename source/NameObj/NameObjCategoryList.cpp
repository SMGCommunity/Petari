#include "NameObj/NameObjCategoryList.h"

NameObjCategoryList::~NameObjCategoryList()
{

}

void NameObjCategoryList::incrementCheck(NameObj *pObj, s32 idx)
{
    mCategoryInfo.mContent[idx]->mChecks++;
}

void NameObjCategoryList::add(NameObj *pObj, s32 idx)
{
    CategoryInfo* info = mCategoryInfo.mContent[idx];
    int val = info->_8++;
    info->mArr.mContent[val] = pObj;
}

NameObjCategoryList::CategoryInfo::CategoryInfo()
{
    mArr.mContent = 0;
    mArr.mCount = 0;
    _8 = 0;
}

NameObjCategoryList::CategoryInfo::~CategoryInfo()
{
    
}