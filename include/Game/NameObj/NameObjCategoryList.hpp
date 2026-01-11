#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Util.hpp"

namespace {
    template < typename T >
    class NameObjRealDelegator {
    public:
        inline NameObjRealDelegator(T in_func) { mNameObjFunc = in_func; }

        virtual void operator()(NameObj* pNameObj) { (pNameObj->*mNameObjFunc)(); }

        T mNameObjFunc;  // 0x4
    };
};  // namespace

struct CategoryListInitialTable {
    u32 mIndex;  // 0x0
    u32 mCount;  // 0x4
};

/// @brief Organizes NameObjs by execution category.
class NameObjCategoryList {
public:
    class CategoryInfo {
    public:
        CategoryInfo();
        ~CategoryInfo();

        MR::Vector< MR::AssignableArray< NameObj* > > mNameObjArr;  // 0x0
        MR::FunctorBase* _C;
        u32 mCheck;  // 0x10
    };

    NameObjCategoryList(u32, const CategoryListInitialTable*, NameObjMethod, bool, const char*);
    NameObjCategoryList(u32, const CategoryListInitialTable*, NameObjMethodConst, bool, const char*);
    ~NameObjCategoryList();

    void execute(int);
    void incrementCheck(NameObj*, int);
    void allocateBuffer();
    void add(NameObj*, int);
    void remove(NameObj*, int);
    void registerExecuteBeforeFunction(const MR::FunctorBase&, int);
    void initTable(u32, const CategoryListInitialTable*);

    MR::AssignableArray< NameObjCategoryList::CategoryInfo > mCategoryInfo;  // 0x0

    union {
        NameObjRealDelegator< NameObjMethod >* mDelegator;
        NameObjRealDelegator< NameObjMethodConst >* mDelegatorConst;
    };

    u8 _C;
    u8 _D;
};