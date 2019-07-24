#include "Actor/NameObj/NameObjCategoryList.h"
#include "arch/NMWException.h"

NameObjCategoryList::CategoryInfo::CategoryInfo()
{
    this->mArr.mContent = 0;
    this->mArr.mCount = 0;
    this->_8 = 0;
}

// todo -- finish
void NameObjCategoryList::initTable(u32 numEntries, const CategoryListInitialTable *table)
{
    CategoryInfo* cat = new CategoryInfo[numEntries];
    this->mCategoryInfo = cat;
    this->_4 = numEntries;

    while(cat != this->mCategoryInfo + this->_4)
    {
        cat->_C = 0;
        cat++;
    }

    while(table->mIndex != -1)
    {
        if (this->_D == 0)
        {

            this->_C = 1;
        }
    }

    return;
}