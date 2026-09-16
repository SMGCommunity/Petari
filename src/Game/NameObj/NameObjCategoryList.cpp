#include "Game/NameObj/NameObjCategoryList.hpp"
#include <algorithm>

NameObjCategoryList::NameObjCategoryList(u32 count, const CategoryListInitialTable* pTable, NameObjMethod pMethod, bool allocateByCheck,
                                         const char*) {
    NameObjMethod method;
    method = pMethod;
    mDelegator = new NameObjRealDelegator< NameObjMethod >(method);
    mAllocateByCheck = allocateByCheck;
    mBufferAllocated = false;
    initTable(count, pTable);
}

NameObjCategoryList::NameObjCategoryList(u32 count, const CategoryListInitialTable* pTable, NameObjMethodConst pMethod, bool allocateByCheck,
                                         const char*) {
    NameObjMethodConst method;
    method = pMethod;
    mDelegatorConst = new NameObjRealDelegator< NameObjMethodConst >(method);
    mAllocateByCheck = allocateByCheck;
    mBufferAllocated = false;
    initTable(count, pTable);
}

NameObjCategoryList::~NameObjCategoryList() {
    delete mDelegator;
}

void NameObjCategoryList::execute(int idx) {
    CategoryInfo* pCategoryInfo = &mCategoryInfo[idx];

    if (pCategoryInfo->mNameObjArr.size() == 0) {
        return;
    }

    if (pCategoryInfo->mExecuteBeforeFunction != nullptr) {
        (*pCategoryInfo->mExecuteBeforeFunction)();
    }

    for (NameObj** pNameObj = pCategoryInfo->mNameObjArr.begin(); pNameObj != pCategoryInfo->mNameObjArr.end(); pNameObj++) {
        (*mDelegator)(*pNameObj);
    }
}

void NameObjCategoryList::incrementCheck(NameObj*, int index) {
    mCategoryInfo[index].mCheck++;
}

void NameObjCategoryList::allocateBuffer() {
    if (mAllocateByCheck) {
        for (int i = 0; i < mCategoryInfo.size(); i++) {
            NameObjCategoryList::CategoryInfo* pInfo = &mCategoryInfo[i];
            u32 size = pInfo->mCheck;
            NameObj** pNameObjArr = new NameObj*[size];
            MR::Vector< MR::AssignableArray< NameObj* > >* pArray = &mCategoryInfo[i].mNameObjArr;
            pArray->mArray.mArr = pNameObjArr;
            pArray->mArray.mMaxSize = size;
        }

        mBufferAllocated = true;
    }
}

void NameObjCategoryList::add(NameObj* pObj, int idx) {
    mCategoryInfo[idx].mNameObjArr.push_back(pObj);
}

void NameObjCategoryList::remove(NameObj* pObj, int idx) {
    MR::Vector< MR::AssignableArray< NameObj* > >& rArray = mCategoryInfo[idx].mNameObjArr;
    s32 size = rArray.size();
    NameObj** pBegin = rArray.begin();
    NameObj** pFound = std::find(pBegin, pBegin + size, pObj);
    rArray[pFound - pBegin] = rArray[rArray.size() - 1];
    rArray.pop_back();
}

void NameObjCategoryList::registerExecuteBeforeFunction(const MR::FunctorBase& rFunc, int idx) {
    NameObjCategoryList::CategoryInfo* pCategoryInfo = &mCategoryInfo[idx];

    pCategoryInfo->mExecuteBeforeFunction = rFunc.clone(nullptr);
}

void NameObjCategoryList::initTable(u32 count, const CategoryListInitialTable* pTable) {
    mCategoryInfo.init(count);

    for (CategoryInfo* pCategoryInfo = mCategoryInfo.begin(); pCategoryInfo != mCategoryInfo.end(); pCategoryInfo++) {
        pCategoryInfo->mExecuteBeforeFunction = nullptr;
    }

    for (const CategoryListInitialTable* pEntry = &pTable[0]; pEntry->mIndex != -1; pEntry++) {
        if (!mAllocateByCheck) {
            u32 size = pEntry->mCount;
            NameObj** pArray = new NameObj*[size];
            NameObjCategoryList::CategoryInfo* pInfo = &mCategoryInfo[pEntry->mIndex];
            pInfo->mNameObjArr.mArray.mArr = pArray;
            pInfo->mNameObjArr.mArray.mMaxSize = size;
            mBufferAllocated = true;
        }

        mCategoryInfo[pEntry->mIndex].mCheck = 0;
    }
}

NameObjCategoryList::CategoryInfo::CategoryInfo() : mNameObjArr() {
}

NameObjCategoryList::CategoryInfo::~CategoryInfo() {
}
