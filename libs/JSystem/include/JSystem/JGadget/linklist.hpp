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
        iterator& operator=(const iterator& rOther) NO_INLINE { return *this; }
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
        INLINE_FUNC_DECL_NO_ARG(TLinkListNode) : mPrev(nullptr), mNext(nullptr) {}
        TLinkListNode() NO_INLINE;
        TLinkListNode* getNext() const NO_INLINE;
        TLinkListNode* mNext;
        TLinkListNode* mPrev;
    };

    class TNodeLinkList {
    public:
        struct iterator {
            iterator() NO_INLINE;
            iterator(TLinkListNode*) NO_INLINE;
            iterator(const iterator&) NO_INLINE;
            iterator& operator++() NO_INLINE;
            TLinkListNode* operator->() const NO_INLINE;

            INLINE_FUNC_DECL(iterator, TLinkListNode* node) : curr(node) {}

            TLinkListNode* curr;
        };

        TNodeLinkList() NO_INLINE;

        ~TNodeLinkList();

        void Initialize_() NO_INLINE;
        TLinkListNode* begin() NO_INLINE;
        TLinkListNode* end() NO_INLINE;
        TLinkListNode* Insert(iterator, TLinkListNode*);
        TLinkListNode* Erase(TLinkListNode*);
        void Remove(TLinkListNode*);
        void splice(iterator, TNodeLinkList&, iterator);

        INLINE_FUNC_DECL_NO_ARG(TNodeLinkList) : CALL_INLINE_FUNC_NO_ARG(mEnd) { Initialize_(); }

        template < typename T >
        inline void Remove_if(T p, TNodeLinkList& removed) {
            TLinkListNode *curr, *end, *removedEnd;
            removedEnd = &removed.mEnd;
            curr = mEnd.mNext;
            end = &mEnd;
            while (curr != end) {
                TLinkListNode* prev = curr;
                if (p(curr)) {
                    curr = curr->mNext;
                    removed.splice(CALL_INLINE_FUNC(iterator, removedEnd), *this, CALL_INLINE_FUNC(iterator, prev));
                } else
                    curr = curr->mNext;
            }
        }

        template < typename T >
        inline void remove_if(T p) {
            TNodeLinkList CALL_INLINE_FUNC_NO_ARG(removed);
            Remove_if(p, removed);
        }

        u32 mLen;
        TLinkListNode mEnd;
    };

    template < typename T, int NODE_OFFSET >
    class TLinkList : public TNodeLinkList {
    public:
        struct iterator : public TIterator< std::bidirectional_iterator_tag, T >, public TNodeLinkList::iterator {
            iterator() NO_INLINE : TNodeLinkList::iterator() {}

            iterator(TLinkListNode* iter) : CALL_INLINE_FUNC(TNodeLinkList::iterator, iter) {}

            iterator(TNodeLinkList::iterator iter) NO_INLINE : TNodeLinkList::iterator(iter) {}

            iterator(const iterator& rOther) : CALL_INLINE_FUNC(TNodeLinkList::iterator, rOther.curr) {}

            const iterator& operator=(const iterator& rOther) NO_INLINE {
                TIterator< std::bidirectional_iterator_tag, T >::operator=(rOther);
                curr = rOther.curr;
                return *this;
            }

            iterator& operator++() NO_INLINE {
                TNodeLinkList::iterator::operator++();
                return *this;
            }

            T* operator->() const NO_INLINE { return TLinkList< T, NODE_OFFSET >::Element_toValue(TNodeLinkList::iterator::operator->()); }

            T* operator*() const NO_INLINE { return operator->(); }
        };

        TLinkList() NO_INLINE : TNodeLinkList() {}

        ~TLinkList() NO_INLINE {};

        TLinkListNode* begin() NO_INLINE { return iterator(CALL_INLINE_FUNC(TNodeLinkList::iterator, TNodeLinkList::begin())).curr; }

        TLinkListNode* end() NO_INLINE { return iterator(CALL_INLINE_FUNC(TNodeLinkList::iterator, TNodeLinkList::end())).curr; }

        void Push_back(T* element) NO_INLINE { Insert(end(), element); }

        TLinkListNode* Insert(iterator pos, T* element) NO_INLINE {
            return iterator(CALL_INLINE_FUNC(TNodeLinkList::iterator,
                                             TNodeLinkList::Insert(CALL_INLINE_FUNC(TNodeLinkList::iterator, pos.curr), Element_toNode(element))))
                .curr;
        }

        static TLinkListNode* Element_toNode(T* element) NO_INLINE { return (TLinkListNode*)((u8*)element - NODE_OFFSET); }

        static T* Element_toValue(TLinkListNode* element) NO_INLINE { return (T*)((u8*)element + NODE_OFFSET); }

        void Remove(T* element) NO_INLINE { TNodeLinkList::Remove(Element_toNode(element)); }
    };
}  // namespace JGadget
