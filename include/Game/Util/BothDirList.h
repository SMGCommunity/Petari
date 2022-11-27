#pragma once

#include <revolution.h>

namespace MR {

    class BothDirPtrList;

    class BothDirPtrLink {
    public:
        BothDirPtrLink(void *);
        ~BothDirPtrLink();

        void* mPtr; // _0
        BothDirPtrList* mPtrList; // _4
        u32 _8;
        u32 _C;
    };

    class BothDirPtrList {
    public:
        BothDirPtrList(bool);

        void initiate();
        void setFirst(MR::BothDirPtrLink *);
        bool append(MR::BothDirPtrLink *);
        bool remove(MR::BothDirPtrLink *);

        MR::BothDirPtrLink* _0;
        MR::BothDirPtrLink* _4;
        u32 _8;
    };

    template<class T>
    class BothDirList : public BothDirPtrList {
    public:
        inline BothDirList() : BothDirPtrList() {

        }

        ~BothDirList() {
            
        }
    };
};