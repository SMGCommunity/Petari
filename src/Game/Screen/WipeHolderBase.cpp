#include "Game/Screen/WipeHolderBase.hpp"
#include "Game/Screen/WipeLayoutBase.hpp"
#include <cstdio>

WipeHolderBase::WipeHolderBase(u32 num, const char *pName) :
    NameObj(pName),
    mCurrentWipeLayout(),
    mWipeLayoutArray()
{
    mWipeLayoutArray.init(num);
}

void WipeHolderBase::setCurrent(const char* pWipeName) {
    if (mCurrentWipeLayout != nullptr) {
        mCurrentWipeLayout->forceOpen();
        mCurrentWipeLayout->kill();
    }

    mCurrentWipeLayout = findWipe(pWipeName);
    mCurrentWipeLayout->appear();
    mCurrentWipeLayout->forceOpen();
}

void WipeHolderBase::wipe(const char* pWipeName, s32 step) {
    updateWipe(pWipeName);
    mCurrentWipeLayout->wipe(step);
}

void WipeHolderBase::forceClose(const char* pWipeName) {
    updateWipe(pWipeName);
    mCurrentWipeLayout->forceClose();
}

void WipeHolderBase::forceOpen(const char* pWipeName) {
    updateWipe(pWipeName);
    mCurrentWipeLayout->forceOpen();
}

bool WipeHolderBase::isOpen() const {
    return mCurrentWipeLayout->isOpen();
}

bool WipeHolderBase::isClose() const {
    return mCurrentWipeLayout->isClose();
}

bool WipeHolderBase::isWipeIn() const {
    return mCurrentWipeLayout->isWipeIn();
}

bool WipeHolderBase::isWipeOut() const {
    return mCurrentWipeLayout->isWipeOut();
}

bool WipeHolderBase::isCurrent(const char* pWipeName) const {
    return strcmp(mCurrentWipeLayout->getWipeName(), pWipeName) == 0;
}

void WipeHolderBase::addWipeLayout(WipeLayoutBase* pWipeLayout) {
    pWipeLayout->initWithoutIter();

    if (mCurrentWipeLayout == nullptr) {
        mCurrentWipeLayout = pWipeLayout;
    }

    mWipeLayoutArray.push_back(pWipeLayout);
}

void WipeHolderBase::updateWipe(const char* pWipeName) {
    if (pWipeName == nullptr) {
        return;
    }

    if (isCurrent(pWipeName)) {
        return;
    }

    setCurrent(pWipeName);
}

WipeLayoutBase* WipeHolderBase::getCurrent() const {
    return mCurrentWipeLayout;
}

WipeLayoutBase* WipeHolderBase::findWipe(const char* pWipeName) const {
    if (isCurrent(pWipeName)) {
        return mCurrentWipeLayout;
    }

    for (u32 i = 0; i < mWipeLayoutArray.size(); i++) {
        if (strcmp(pWipeName, mWipeLayoutArray[i]->getWipeName()) == 0) {
            return mWipeLayoutArray[i];
        }
    }

    return nullptr;
}
