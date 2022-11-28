#include "Game/NameObj/NameObjCategoryList.h"

NameObjCategoryList::NameObjCategoryList(u32 count, const CategoryListInitialTable *pTable, func func_ptr, bool a4, const char * /* unused */) {
    func new_ptr;
    new_ptr = func_ptr;
    mDelegator = new NameObjRealDelegator<func>(new_ptr);
    _D = a4;
    _C = 0;
    initTable(count, pTable);
}

NameObjCategoryList::NameObjCategoryList(u32 count, const CategoryListInitialTable *pTable, func_const func_ptr, bool a4, const char * /* unused */) {
    func_const new_ptr;
    new_ptr = func_ptr;
    mDelegatorConst = new NameObjRealDelegator<func_const>(new_ptr);
    _D = a4;
    _C = 0;
    initTable(count, pTable);
}

void NameObjCategoryList::execute(int idx) {
    NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo.mArr[idx];

    if (inf->_8 && inf->_C) {
        (*inf->_C)();
    }

    NameObj** arr = inf->mNameObjArr.mArr;

    while (arr != &inf->mNameObjArr.mArr[inf->_8]) {
        (*mDelegator)(*arr);
        arr++;
    }
}

void NameObjCategoryList::incrementCheck(NameObj */*unused*/, int index) {
    mCategoryInfo.mArr[index].mCheck++;
}

void NameObjCategoryList::allocateBuffer() {
    if (_D) {
        for (s32 i = 0; i < mCategoryInfo.mMaxSize; i++) {
            NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo.mArr[i];
            u32 size = inf->mCheck;
            NameObj** nameObjArr = new NameObj*[size];
            MR::AssignableArray<NameObj*>* arr = &mCategoryInfo.mArr[i].mNameObjArr;
            arr->mArr = nameObjArr;
            arr->mMaxSize = size;
        }

        _C = 1;
    }
}

void NameObjCategoryList::add(NameObj *pObj, int idx) {
    NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo.mArr[idx];
    s32 cnt = inf->_8++;
    inf->mNameObjArr.mArr[cnt] = pObj;
}

// NameObjCategoryList::remove

void NameObjCategoryList::registerExecuteBeforeFunction(const MR::FunctorBase &rFunc, int idx) {
    NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo.mArr[idx];
    inf->_C = rFunc.clone(0);
}

void NameObjCategoryList::initTable(u32 count, const CategoryListInitialTable *pTable) {
    mCategoryInfo.mArr = new CategoryInfo[count];
    mCategoryInfo.mMaxSize = count;

    NameObjCategoryList::CategoryInfo* curInf = &mCategoryInfo.mArr[0];

    while ((curInf != &mCategoryInfo.mArr[mCategoryInfo.mMaxSize])) {
        curInf->_C = 0;
        curInf++;
    }

    const CategoryListInitialTable* ent = &pTable[0];

    while(ent->mIndex != -1) {
        if (!_D) {
            u32 size = ent->mCount;
            NameObj** arr = new NameObj*[size];
            NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo.mArr[ent->mIndex];
            inf->mNameObjArr.mArr = arr;
            inf->mNameObjArr.mMaxSize = size;
            _C = 1;
        }

        mCategoryInfo.mArr[ent->mIndex].mCheck = 0;

        ent++;
    }
}

NameObjCategoryList::CategoryInfo::CategoryInfo() {
    mNameObjArr.mArr = 0;
    mNameObjArr.mMaxSize = 0;
    _8 = 0;
}

NameObjCategoryList::CategoryInfo::~CategoryInfo() {
    
}

NameObjCategoryList::~NameObjCategoryList() {
    delete mDelegator;
}
