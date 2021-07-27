#pragma once

#include "JSystem/JSupport/JSUList.h"

template<typename T>
class JSUTree : public JSUPtrList
{
public:
    inline JSUTree(void* ptr) : JSUPtrList(), mLink(ptr) { }

    ~JSUTree();

    JSULink<T> mLink; // _C
};