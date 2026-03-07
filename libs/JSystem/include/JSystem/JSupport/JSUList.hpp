#pragma once

#include "Inline.hpp"
#include <revolution.h>

class JSUPtrLink;

class JSUPtrList {
public:
    inline JSUPtrList() { initiate(); }

    JSUPtrList(bool);
    ~JSUPtrList();

    void initiate();
    void setFirst(JSUPtrLink*);
    bool append(JSUPtrLink*);
    bool prepend(JSUPtrLink*);
    bool insert(JSUPtrLink*, JSUPtrLink*);
    bool remove(JSUPtrLink*);

    JSUPtrLink* getFirstLink() const { return mHead; }

    u32 getNumLinks() const { return mNodeCount; }

    JSUPtrLink* mHead;  // 0x0
    JSUPtrLink* mTail;  // 0x4
    u32 mNodeCount;     // 0x8
};

class JSUPtrLink {
public:
    JSUPtrLink(void*);
    ~JSUPtrLink();

    void* getObjectPtr() const { return mData; }

    JSUPtrList* getList() const { return mPtrList; }

    JSUPtrLink* getNext() const { return mNext; }

    void* mData;           // 0x0
    JSUPtrList* mPtrList;  // 0x4
    JSUPtrLink* mPrev;     // 0x8
    JSUPtrLink* mNext;     // 0xC
};

template < class T >
class JSULink : public JSUPtrLink {
public:
    JSULink(void* pData) : JSUPtrLink(pData) {}

    ~JSULink() NO_INLINE {}

    T* getObject() const { return (T*)getObjectPtr(); }

    JSULink< T >* getNext() const { return (JSULink< T >*)this->JSUPtrLink::getNext(); }

    JSULink< T >* getPrev() const { return (JSULink< T >*)this->JSUPtrLink::getPrev(); }
};

template < class T >
class JSUList : public JSUPtrList {
public:
    JSUList() : JSUPtrList() {}

    JSUList(bool thing) : JSUPtrList(thing) {}

    ~JSUList(){

    };

    JSULink< T >* getFirst() const { return (JSULink< T >*)getFirstLink(); }

    JSULink< T >* getLast() const { return (JSULink< T >*)getLastLink(); }

    u32 getNumLinks() const { return this->JSUPtrList::getNumLinks(); }

    JSULink< T >* getEnd() const { return NULL; }
};

template < typename T >
class JSUListIterator {
public:
    JSUListIterator() : mLink(NULL) {}

    JSUListIterator(JSULink< T >* link) : mLink(link) {}

    JSUListIterator(JSUList< T >* list) : mLink(list->getFirst()) {}

    JSUListIterator< T >& operator=(JSULink< T >* link) {
        this->mLink = link;
        return *this;
    }

    T* getObject() { return this->mLink->getObject(); }

    bool operator==(JSULink< T > const* other) const { return this->mLink == other; }
    bool operator!=(JSULink< T > const* other) const { return this->mLink != other; }
    bool operator==(JSUListIterator< T > const& other) const { return this->mLink == other.mLink; }
    bool operator!=(JSUListIterator< T > const& other) const { return this->mLink != other.mLink; }

    JSUListIterator< T > operator++(int) {
        JSUListIterator< T > prev = *this;
        this->mLink = this->mLink->getNext();
        return prev;
    }

    JSUListIterator< T >& operator++() {
        this->mLink = this->mLink->getNext();
        return *this;
    }

    JSUListIterator< T > operator--(int) {
        JSUListIterator< T > prev = *this;
        this->mLink = this->mLink->getPrev();
        return prev;
    }

    JSUListIterator< T >& operator--() {
        this->mLink = this->mLink->getPrev();
        return *this;
    }

    T& operator*() { return *this->getObject(); }

    T* operator->() { return this->getObject(); }

    JSULink< T >* mLink;
};

/* from https://github.com/zeldaret/tp/blob/master/include/JSystem/JSupport/JSUList.hpp */
template < typename T >
class JSUTree : public JSUList< T >, public JSULink< T > {
public:
    __inline JSUTree(T* pOwner) : JSUList< T >(), JSULink< T >(pOwner) {}

    ~JSUTree() NO_INLINE {}

    bool appendChild(JSUTree< T >* pChild) { return this->append(pChild); }

    bool removeChild(JSUTree< T >* child) { return this->remove(child); }

    JSUTree< T >* getEndChild() const { return NULL; }

    JSUTree< T >* getFirstChild() const { return (JSUTree< T >*)this->getFirst(); }

    JSUTree< T >* getLastChild() const { return (JSUTree< T >*)this->getLast(); }

    JSUTree< T >* getNextChild() const { return (JSUTree< T >*)this->getNext(); }

    u32 getNumChildren() const { return this->getNumLinks(); }

    T* getObject() const { return (T*)this->mData; }

    JSUTree< T >* getParent() const { return (JSUTree< T >*)this->getList(); }
};

template < typename T >
class JSUTreeIterator {
public:
    JSUTreeIterator() {}

    JSUTreeIterator(JSUTree< T >* tree) : mTree(tree) {}

    T* getObject() { return this->mTree->getObject(); }

    JSUTreeIterator< T >& operator=(JSUTree< T >* tree) {
        this->mTree = tree;
        return *this;
    }

    bool operator==(JSUTree< T >* other) { return this->mTree == other; }

    bool operator!=(JSUTree< T >* other) { return this->mTree != other; }

    JSUTreeIterator< T > operator++(int) {
        JSUTreeIterator< T > prev = *this;
        this->mTree = this->mTree->getNextChild();
        return prev;
    }

    JSUTreeIterator< T >& operator++() NO_INLINE {
        this->mTree = this->mTree->getNextChild();
        return *this;
    }

    T& operator*() { return *this->getObject(); }

    T* operator->() { return this->getObject(); }

    JSUTree< T >* mTree;  // 0x0
};
