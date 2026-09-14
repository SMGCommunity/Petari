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
        BothDirPtrList() {
            initiate();
        }

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
        BothDirLink(T* pValue) : BothDirPtrLink(pValue) {
        }

        ~BothDirLink() {
        }
    };

    template < class T >
    class BothDirList : public BothDirPtrList {
    public:
        class iterator {
        public:
            iterator(BothDirPtrLink* pLink) : mLink(pLink) {
            }

            T* operator->() const {
                return static_cast< T* >(mLink->mValue);
            }

            void operator++(int) {
                mLink = mLink->mNextLink;
            }

            bool isEnd() const {
                return mLink == nullptr;
            }

            /* 0x00 */ BothDirPtrLink* mLink;
        };

        iterator begin() const {
            return iterator(mHead);
        }

        BothDirList() : BothDirPtrList() {
        }

        BothDirList(bool param1) : BothDirPtrList(param1) {
        }

        ~BothDirList() {
        }
    };
};  // namespace MR
