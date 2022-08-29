#include "JSystem/JKernel/JKRHeap.h"

JKRHeap* JKRHeap::sCurrentHeap;
JKRHeap* JKRHeap::sRootHeap;
JKRHeap* JKRHeap::sSystemHeap;

static u32 ARALT_AramStartAddr = 0x90000000;

JKRHeap::~JKRHeap() {
    JSUTree<JKRHeap>* tree = mChildTree.getParent();
    tree->removeChild(&mChildTree);

    JSUTree<JKRHeap>* next = sRootHeap->mChildTree.getFirstChild();
    JKRHeap* root = sRootHeap;
    JKRHeap* current = sCurrentHeap;

    if (current == this) {
        sCurrentHeap = next == NULL ? root : next->getObject();
    }

    JKRHeap* system = sSystemHeap;
    if (system == this) {
        sSystemHeap = next == NULL ? root : next->getObject();
    }
} 

JKRHeap* JKRHeap::becomeSystemHeap() {
    JKRHeap* sys = sSystemHeap;
    sSystemHeap = this; 
    return sys;
}

JKRHeap* JKRHeap::becomeCurrentHeap() {
    JKRHeap* cur = sCurrentHeap;
    sCurrentHeap = this; 
    return cur;
}

void JKRHeap::destroy(JKRHeap *pHeap) {
    pHeap->do_destroy();
}

void* JKRHeap::alloc(u32 size, int align, JKRHeap *pHeap) {
    if (pHeap != NULL) {
        return pHeap->alloc(size, align);
    }

    if (JKRHeap::sCurrentHeap != NULL) {
        return JKRHeap::sCurrentHeap->alloc(size, align);
    }

    return NULL;
}

void* JKRHeap::alloc(u32 size, int align) {
    return do_alloc(size, align);
}

void JKRHeap::free(void *pData, JKRHeap *pHeap) {
    if (!pHeap) {
        pHeap = findFromRoot(pData);

        if (!pHeap) {
            return;
        }
    }

    pHeap->do_free(pData);
}

void JKRHeap::free(void *pData) {
    do_free(pData); 
}

void JKRHeap::callAllDisposer() {
    while (mDisposerList.mHead != NULL) {
        reinterpret_cast<JKRDisposer*>(mDisposerList.mHead->mData)->~JKRDisposer();
    }
}

void JKRHeap::freeAll() {
    do_freeAll();
}

void JKRHeap::freeTail() {
    do_freeTail();
}

s32 JKRHeap::resize(void *pData, u32 size) {
    return do_resize(pData, size);
}
 
JKRHeap* JKRHeap::findFromRoot(void *pData) {
    JKRHeap* root = sRootHeap;

    if (root == NULL) { 
        return NULL; 
    }

    if ((void*)root->mStart <= pData && pData < (void*)root->mEnd) {
        return root->find(pData);
    }

    return root->findAllHeap(pData);
}

#ifdef NON_MATCHING
// some loading issues with the first line
JKRHeap* JKRHeap::find(void *pData) const {
    if (getStartAddr() <= pData && pData < getEndAddr()) {
        const JSUTree<JKRHeap>& tree = mChildTree;

        if (tree.getNumChildren() != 0) {
            JSUTreeIterator<JKRHeap> it;

            for (it = tree.getFirstChild(); it != tree.getEndChild(); ++it) {
                JKRHeap* childHeap = it.getObject();
                JKRHeap* resHeap = childHeap->find(pData);

                if (resHeap) {
                    return resHeap;
                }
            }
        }

        // this is to avoid returning a const JKRHeap ptr
        return (JKRHeap*)this;
    } 

    return NULL;
}
#endif


#ifdef NON_MATCHING
// shrug
void JKRHeap::dispose_subroutine(u32 start, u32 end) {
    JSUListIterator<JKRDisposer> last_it;
    JSUListIterator<JKRDisposer> next_it;
    JSUListIterator<JKRDisposer> it = mDisposerList.getFirst();

    for (; it != NULL; it = next_it) {
        JKRDisposer* disp = it.getObject();

        if ((void*)start <= disp && disp < (void*)end) {
            disp->~JKRDisposer();

            if (last_it == NULL) {
                next_it = mDisposerList.getFirst();
            }
            else {
                next_it = last_it;
                next_it++;
            }
        }
        else {
            last_it = it;
            next_it = it;
            next_it++;
        }
    }
}
#endif

void* operator new(u32 size) {
    return JKRHeap::alloc(size, 4, NULL);
}

void* operator new(u32 size, int align) {
    return JKRHeap::alloc(size, align, NULL);
}

void* operator new(u32 size, JKRHeap *pHeap, int align) {
    return JKRHeap::alloc(size, align, pHeap);
}

void* operator new[](u32 size) {
    return JKRHeap::alloc(size, 4, NULL);
}

void* operator new[](u32 size, int align) {
    return JKRHeap::alloc(size, align, NULL);
}

void* operator new[](u32 size, JKRHeap *pHeap, int align) {
    return JKRHeap::alloc(size, align, pHeap);
}

void operator delete(void *pData) {
    JKRHeap::free(pData, NULL);
}