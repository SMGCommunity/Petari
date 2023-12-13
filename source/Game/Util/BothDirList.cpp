#include "Game/Util.hpp"

namespace MR {
    BothDirPtrLink::BothDirPtrLink(void *pLink) :
        mPtr(pLink), mPtrList(0), _8(0), _C(0) {

    }

    BothDirPtrLink::~BothDirPtrLink() {
        if (mPtrList) {
            mPtrList->remove(this);
        }
    }

    BothDirPtrList::BothDirPtrList(bool doInitialize) {
        if (doInitialize) {
            initiate();
        }
    }

    void BothDirPtrList::initiate() {
        _0 = 0;
        _4 = 0;
        _8 = 0;
    }

    void BothDirPtrList::setFirst(MR::BothDirPtrLink *pFirst) {
        pFirst->mPtrList = this;
        pFirst->_8 = 0;
        pFirst->_C = 0;
        _4 = pFirst;
        _0 = pFirst;
        _8 = 1;
    }
};
