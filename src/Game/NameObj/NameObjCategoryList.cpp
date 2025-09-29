#include "Game/NameObj/NameObjCategoryList.hpp"

NameObjCategoryList::NameObjCategoryList(u32 count, const CategoryListInitialTable *pTable, NameObjMethod pMethod, bool a4, const char * /* unused */) {
    NameObjMethod method;
    method = pMethod;
    mDelegator = new NameObjRealDelegator<NameObjMethod>(method);
    _D = a4;
    _C = 0;
    initTable(count, pTable);
}

NameObjCategoryList::NameObjCategoryList(u32 count, const CategoryListInitialTable *pTable, NameObjMethodConst pMethod, bool a4, const char * /* unused */) {
    NameObjMethodConst method;
    method = pMethod;
    mDelegatorConst = new NameObjRealDelegator<NameObjMethodConst>(method);
    _D = a4;
    _C = 0;
    initTable(count, pTable);
}

void NameObjCategoryList::execute(int idx) {
    CategoryInfo* pCategoryInfo = &mCategoryInfo[idx];

    if (pCategoryInfo->mNameObjArr.size() == 0) {
        return;
    }

    if (pCategoryInfo->_C != NULL) {
        (*pCategoryInfo->_C)();
    }

    for (NameObj** pNameObj = pCategoryInfo->mNameObjArr.begin(); pNameObj != pCategoryInfo->mNameObjArr.end(); pNameObj++) {
        (*mDelegator)(*pNameObj);
    }
}

void NameObjCategoryList::incrementCheck(NameObj */*unused*/, int index) {
    mCategoryInfo[index].mCheck++;
}

void NameObjCategoryList::allocateBuffer() {
    if (_D) {
        for (int i = 0; i < mCategoryInfo.size(); i++) {
            NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo[i];
            u32 size = inf->mCheck;
            NameObj** nameObjArr = new NameObj*[size];
            MR::Vector<MR::AssignableArray<NameObj*> >* arr = &mCategoryInfo[i].mNameObjArr;
            arr->mArray.mArr = nameObjArr;
            arr->mArray.mMaxSize = size;
        }

        _C = 1;
    }
}

void NameObjCategoryList::add(NameObj *pObj, int idx) {
    mCategoryInfo[idx].mNameObjArr.push_back(pObj);
}

// NameObjCategoryList::remove

void NameObjCategoryList::registerExecuteBeforeFunction(const MR::FunctorBase &rFunc, int idx) {
    NameObjCategoryList::CategoryInfo* pCategoryInfo = &mCategoryInfo[idx];

    pCategoryInfo->_C = rFunc.clone(nullptr);
}

void NameObjCategoryList::initTable(u32 count, const CategoryListInitialTable *pTable) {
    mCategoryInfo.init(count);

    for (CategoryInfo* pCategoryInfo = mCategoryInfo.begin(); pCategoryInfo != mCategoryInfo.end(); pCategoryInfo++) {
        pCategoryInfo->_C = NULL;
    }

    for (const CategoryListInitialTable* pEntry = &pTable[0]; pEntry->mIndex != -1; pEntry++) {
        if (!_D) {
            u32 size = pEntry->mCount;
            NameObj** arr = new NameObj*[size];
            NameObjCategoryList::CategoryInfo* inf = &mCategoryInfo[pEntry->mIndex];
            inf->mNameObjArr.mArray.mArr = arr;
            inf->mNameObjArr.mArray.mMaxSize = size;
            _C = 1;
        }

        mCategoryInfo[pEntry->mIndex].mCheck = 0;
    }
}

NameObjCategoryList::CategoryInfo::CategoryInfo() :
    mNameObjArr()
{}

NameObjCategoryList::CategoryInfo::~CategoryInfo() {
    
}

NameObjCategoryList::~NameObjCategoryList() {
    delete mDelegator;
}
