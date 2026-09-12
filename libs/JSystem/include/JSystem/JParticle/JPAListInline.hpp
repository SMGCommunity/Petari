#ifndef JPA_LIST_INLINE_HPP
#define JPA_LIST_INLINE_HPP

template < class T >
inline JPANode< T >::JPANode() : mpPrev(NULL), mpNext(NULL) {
}

template < class T >
inline JPANode< T >::~JPANode() {
}

template < class T >
inline JPAList< T >::JPAList() : mpFirst(NULL), mpLast(NULL), mNum() {
}

template < class T >
inline JPANode< T >* JPAList< T >::pop_front() {
    JPANode< T >* ret = NULL;

    if (mNum == 1) {
        ret = mpFirst;
        mpLast = NULL;
        mpFirst = NULL;
        mNum--;
    } else if (mNum) {
        ret = mpFirst;
        ret->mpNext->mpPrev = NULL;
        mpFirst = ret->mpNext;
        mNum--;
    }

    return ret;
}

template < class T >
inline JPANode< T >* JPAList< T >::pop_back() {
    JPANode< T >* ret = NULL;

    if (mNum == 1) {
        ret = mpLast;
        mpLast = NULL;
        mpFirst = NULL;
        mNum--;
    } else if (mNum) {
        ret = mpLast;
        ret->mpPrev->mpNext = NULL;
        mpLast = ret->mpPrev;
        mNum--;
    }

    return ret;
}

template < class T >
inline JPANode< T >* JPAList< T >::erase(JPANode< T >* node) {
    if (node->mpNext != NULL && node->mpPrev != NULL) {
        node->mpPrev->mpNext = node->mpNext;
        node->mpNext->mpPrev = node->mpPrev;
        mNum--;
    } else if (node->mpNext != NULL) {
        node->mpNext->mpPrev = NULL;
        mpFirst = node->mpNext;
        mNum--;
    } else if (node->mpPrev != NULL) {
        node->mpPrev->mpNext = NULL;
        mpLast = node->mpPrev;
        mNum--;
    } else {
        mpLast = NULL;
        mpFirst = NULL;
        mNum--;
    }
    return node;
}

template < class T >
inline void JPAList< T >::push_front(JPANode< T >* node) {
    if (mpFirst != NULL) {
        node->mpPrev = NULL;
        node->mpNext = mpFirst;
        mpFirst->mpPrev = node;
        mpFirst = node;
    } else {
        mpLast = node;
        mpFirst = node;
        node->mpPrev = NULL;
        node->mpNext = NULL;
    }

    mNum++;
}

#endif
