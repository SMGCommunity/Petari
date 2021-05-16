#include "NameObj/NameObjCategoryList.h"

NameObjCategoryList::NameObjCategoryList(u32 numEntries, const CategoryListInitialTable *pTable, TNameObjFunc func, bool unkBool, const char *pCharPtr)
    : mDelegator(new NameObjRealDelegator<TNameObjFunc>(func))
{
    _D = unkBool;
    _C = 0;
}


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

void NameObjCategoryList::remove(NameObj *pObj, s32 idx)
{
    CategoryInfo* info = mCategoryInfo.mContent[idx];
    u32 _8 = info->_8;
    // todo
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