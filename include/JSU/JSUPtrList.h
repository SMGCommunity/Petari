#ifndef JSUPTRLIST_H
#define JSUPTRLIST_H

#include "types.h"

class JSUPtrLink;

class JSUPtrList
{
public:
    JSUPtrList(bool isInitiated);
    ~JSUPtrList();

    void initiate();
    void setFirst(JSUPtrLink* link);
    void append(JSUPtrLink* link);
    void prepend(JSUPtrLink* link);

    u32 _0;
    u32 _4;
    u32 mNumPtrs; // _8
};

#endif // JSUPTRLIST_H