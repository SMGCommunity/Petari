#ifndef NAMEOBJCATEGORYLIST_H
#define NAMEOBJCATEGORYLIST_H

#include <revolution.h>
#include "Actor/NameObj/NameObj.h"
#include "MR/AssignableArray.h"

struct CategoryListInitialTable
{
    s32 mIndex; // _0
    s32 mCount; // _4
};

class NameObjCategoryList
{
public:
    class CategoryInfo
    {
    public:
        CategoryInfo();
        ~CategoryInfo();

        MR::AssignableArray<NameObj *> mArr; // _0
        u32 _8;
        u8 _C;
        u8 _D;
        u8 _E;
        u8 _F;
        s32 mChecks; // _10
    };

    ~NameObjCategoryList();

    void execute(s32);
    void incrementCheck(NameObj *, s32);
    void allocateBuffer();
    void add(NameObj *, s32);
    void remove(NameObj *, s32);

    void initTable(u32, const CategoryListInitialTable *);

    NameObjCategoryList::CategoryInfo* mCategoryInfo; // _0
    u32 _4;
    u32* _8; // NameObjRealDelegator*
    u8 _C;
    u8 _D;
};

#endif // NAMEOBJCATEGORYLIST_H