#pragma once

#include <revolution/types.h>

template < class T >
struct JPANode {
    JPANode();
    ~JPANode();
    JPANode< T >* getPrev() {
        return mpPrev;
    }
    JPANode< T >* getNext() {
        return mpNext;
    }
    T* getObject() {
        return &mData;
    }

    /* 0x00 */ JPANode< T >* mpPrev;
    /* 0x04 */ JPANode< T >* mpNext;
    /* 0x08 */ T mData;
};

template < class T >
struct JPAList {
    /* 0x00 */ JPANode< T >* mpFirst;
    /* 0x04 */ JPANode< T >* mpLast;
    /* 0x08 */ u32 mNum;

    JPAList();
    ~JPAList() {
    }

    JPANode< T >* getEnd() {
        return NULL;
    }
    JPANode< T >* getFirst() const {
        return mpFirst;
    }
    JPANode< T >* getLast() const {
        return mpLast;
    }
    u32 getNum() const {
        return mNum;
    }

    void push_front(JPANode< T >* node);

    void push_back(JPANode< T >* node) {
        if (mpLast != NULL) {
            node->mpPrev = mpLast;
            node->mpNext = NULL;
            mpLast->mpNext = node;
            mpLast = node;
        } else {
            mpFirst = node;
            mpLast = node;
            node->mpNext = node->mpPrev = NULL;
        }

        mNum++;
    }

    JPANode< T >* pop_front();

    JPANode< T >* pop_back();

    JPANode< T >* erase(JPANode< T >* node);
};

#ifndef JPA_LIST_DEFER_INLINE
#include "JSystem/JParticle/JPAListInline.hpp"
#endif
