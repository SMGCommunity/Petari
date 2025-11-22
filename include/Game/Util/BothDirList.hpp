#pragma once

#include <revolution.h>

namespace MR {

    class BothDirPtrList;

    class BothDirPtrLink {
    public:
        BothDirPtrLink(void*);
        ~BothDirPtrLink();

        /* 0x0 */ void* mValue;
        /* 0x4 */ BothDirPtrList* mList;
        /* 0x8 */ BothDirPtrLink* mPrevLink;
        /* 0xC */ BothDirPtrLink* mNextLink;
    };

    class BothDirPtrList {
    public:
        BothDirPtrList(bool);
        ~BothDirPtrList();

        void initiate();
        void setFirst(BothDirPtrLink*);
        bool append(BothDirPtrLink*);
        bool remove(BothDirPtrLink*);

        /* 0x0 */ BothDirPtrLink* mHead;
        /* 0x4 */ BothDirPtrLink* mTail;
        /* 0x8 */ u32 mCount;
    };

    template < class T >
    class BothDirLink : public BothDirPtrLink {
    public:
        BothDirLink(T* pValue) : BothDirPtrLink(pValue) {}

        ~BothDirLink() NO_INLINE {}
    };

    template < class T >
    class BothDirList : public BothDirPtrList {
    public:
        BothDirList(bool param1) : BothDirPtrList(param1) {}

        ~BothDirList() {}
    };
};  // namespace MR
