#pragma once

#include <revolution/types.h>

class FileSelectItem;

class FileSelectItemDelegatorBase {
public:
    virtual void notify(FileSelectItem*, s32) = 0;
};

template < typename T >
class FileSelectItemDelegator : public FileSelectItemDelegatorBase {
public:
    typedef void (T::*Func)(FileSelectItem*, s32);

    inline FileSelectItemDelegator(T* object, Func func) : mObject(object), mFunc(func) {
    }

    void notify(FileSelectItem* pItem, s32 arg1) {
        (mObject->*mFunc)(pItem, arg1);
    }

    T* mObject;
    Func mFunc;
};
