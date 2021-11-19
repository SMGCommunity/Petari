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

void NameObjCategoryList::incrementCheck(NameObj */*unused*/, int index) {
    mCategoryInfo.mArr[index].mCheck++;
}

void NameObjCategoryList::allocateBuffer() {
    if (_D) {
        
    }
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

    while(ent->_0 != -1) {
        if (!_D) {
            u32 size = ent->_4;
            NameObj** arr = new NameObj*[size];
            NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo.mArr[ent->_0];
            inf->mNameObjArr.mArr = arr;
            inf->mNameObjArr.mMaxSize = size;
            _C = 1;
        }

        mCategoryInfo.mArr[ent->_0].mCheck = 0;

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