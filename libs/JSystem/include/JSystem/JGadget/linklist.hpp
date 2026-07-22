#pragma once

#include <cstddef>
#include <revolution/types.h>

#include "JSystem/JGadget/predicate.hpp"

#include "Inline.hpp"

#define JGADGET_LINK_LIST(type, node) JGadget::TLinkList< type, -offsetof(type, node) >

namespace std {

    struct input_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};

    template < typename Category, typename T, typename Distance = s32, typename Pointer = T*, typename Reference = T& >
    class iterator {
    public:
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
        iterator& operator=(const iterator& rOther) NO_INLINE {
            return *this;
        }
    };

}  // namespace std

namespace JGadget {

    template < typename Category, typename T, typename Distance = s32, typename Pointer = T*, typename Reference = T& >
    class TIterator : std::iterator< Category, T, Distance, Pointer, Reference > {
    public:
        const TIterator& operator=(const TIterator& rOther) NO_INLINE {
            std::iterator< Category, T, Distance, Pointer, Reference >::operator=(rOther);
            return *this;
        }
    };

    class TLinkListNode {
    public:
        TLinkListNode() : mPrev(nullptr), mNext(nullptr) {
        }
        TLinkListNode* getNext() const {
            return mNext;
        }
        TLinkListNode* getPrev() const {
            return mPrev;
        }

        TLinkListNode* mNext;
        TLinkListNode* mPrev;
    };

    class TNodeLinkList {
    public:
        struct iterator {
            iterator(){};
            explicit iterator(TLinkListNode* node) : curr(node){};
            iterator& operator=(const iterator& other) {
                curr = other.curr;
                return *this;
            }
            friend bool operator==(iterator a, iterator b) {
                return a.curr == b.curr;
            }
            friend bool operator!=(iterator a, iterator b) {
                return !(a == b);
            }
            iterator& operator++() {
                curr = curr->getNext();
                return *this;
            }
            TLinkListNode* operator->() const {
                return curr;
            }
            TLinkListNode& operator*() const {
                return *curr;
            }

            iterator operator--(int) {
                const iterator old(*this);
                (void)--*this;
                return old;
            }

            iterator& operator--() {
                curr = curr->getPrev();
                return *this;
            }

            TLinkListNode* curr;
        };

        TNodeLinkList() : mEnd() {
            Initialize_();
        }

        ~TNodeLinkList();

        void Initialize_() {
            mLen = 0;
            mEnd.mNext = &mEnd;
            mEnd.mPrev = &mEnd;
        }

        iterator begin() {
            return iterator(mEnd.getNext());
        }
        iterator end() {
            return iterator(&mEnd);
        }
        iterator Insert(iterator, TLinkListNode*);
        iterator Erase(TLinkListNode*);
        void Remove(TLinkListNode*);
        void splice(iterator, TNodeLinkList&, iterator);

        template < typename T >
        inline void Remove_if(T p, TNodeLinkList& removed) {
            iterator it = begin();

            while (it.curr != &mEnd) {
                if (p(*it)) {
                    iterator prev = it;
                    ++it;
                    removed.splice(removed.end(), *this, prev);
                } else {
                    ++it;
                }
            }
        }

        u32 size() const {
            return mLen;
        }

        template < typename T >
        inline void remove_if(T p) {
            TNodeLinkList removed;
            Remove_if(p, removed);
        }

        u32 mLen;
        TLinkListNode mEnd;
    };

    template < typename T, int NODE_OFFSET >
    class TLinkList : public TNodeLinkList {
    public:
        struct iterator : public TIterator< std::bidirectional_iterator_tag, T >, public TNodeLinkList::iterator {
            iterator(){};

            iterator(TLinkListNode* iter) : TNodeLinkList::iterator(iter){};

            explicit iterator(TNodeLinkList::iterator iter) : TNodeLinkList::iterator(iter){};

            const iterator& operator=(const iterator& rOther) {
                TIterator< std::bidirectional_iterator_tag, T >::operator=(rOther);
                curr = rOther.curr;
                return *this;
            }

            iterator& operator++() {
                TNodeLinkList::iterator::operator++();
                return *this;
            }

            friend bool operator==(iterator a, iterator b) {
                return (TNodeLinkList::iterator&)a == (TNodeLinkList::iterator&)b;
            }
            friend bool operator!=(iterator a, iterator b) {
                return !(a == b);
            }

            T* operator->() const {
                return TLinkList< T, NODE_OFFSET >::Element_toValue(TNodeLinkList::iterator::operator->());
            }

            T* operator*() const {
                return operator->();
            }

            iterator& operator--() {
                TNodeLinkList::iterator::operator--();
                return *this;
            }

            iterator operator--(int) {
                const iterator old(*this);
                --*this;
                return old;
            }
        };

        TLinkList() : TNodeLinkList() {
        }

        iterator begin() {
            return iterator(TNodeLinkList::begin());
        }

        iterator end() NO_INLINE {
            return iterator(TNodeLinkList::end());
        }

        void Push_back(T* element) NO_INLINE {
            Insert(end(), element);
        }

        iterator Insert(iterator pos, T* element) NO_INLINE {
            return iterator(TNodeLinkList::Insert((TNodeLinkList::iterator&)pos, Element_toNode(element)));
        }

        static TLinkListNode* Element_toNode(T* element) NO_INLINE {
            return (TLinkListNode*)((u8*)element - NODE_OFFSET);
        }

        static T* Element_toValue(TLinkListNode* element) NO_INLINE {
            return (T*)((u8*)element + NODE_OFFSET);
        }

        T& front() {
            return *begin();
        }

        T& back() {
            return *--end();
        }

        void Remove(T* element) NO_INLINE {
            TNodeLinkList::Remove(Element_toNode(element));
        }
    };
}  // namespace JGadget
