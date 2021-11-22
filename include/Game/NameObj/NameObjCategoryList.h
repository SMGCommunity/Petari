#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/NameObj/NameObjHolder.h"
#include "Game/Util.h"

namespace {
    template<typename T>
    class NameObjRealDelegator {
    public:
        inline NameObjRealDelegator(T in_func) {
            mNameObjFunc = in_func;
        }

        virtual void operator()(NameObj *pNameObj) {
            (pNameObj->*mNameObjFunc)();
        }

        T mNameObjFunc; // _4
    };
};

struct CategoryListInitialTable {
    u32 _0;
    u32 _4;
};

class NameObjCategoryList {
public:

    class CategoryInfo {
    public:
        CategoryInfo();
        ~CategoryInfo();

        MR::AssignableArray<NameObj *> mNameObjArr; // _0
        u32 _8;
        MR::FunctorBase* _C;
        u32 mCheck;                                 // _10
    };

    NameObjCategoryList(u32, const CategoryListInitialTable *, func, bool, const char *);
    NameObjCategoryList(u32, const CategoryListInitialTable *, func_const, bool, const char *);
    ~NameObjCategoryList();

    void execute(int);
    void incrementCheck(NameObj *, int);
    void allocateBuffer();
    void add(NameObj *, int);
    void remove(NameObj *, int);
    void registerExecuteBeforeFunction(const MR::FunctorBase &, int);
    void initTable(u32, const CategoryListInitialTable *);

    MR::AssignableArray<NameObjCategoryList::CategoryInfo> mCategoryInfo;   // _0

    union {
        NameObjRealDelegator<func>* mDelegator;
        NameObjRealDelegator<func_const>* mDelegatorConst;
    };

    u8 _C;
    u8 _D;
};