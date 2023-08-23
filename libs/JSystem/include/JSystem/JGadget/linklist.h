#pragma once

#include <cstddef>
#include <revolution/types.h>

#include "Inline.h"

#define JGADGET_LINK_LIST(type, node) JGadget::TLinkList<type, -offsetof(type, node)>

namespace std {
    struct input_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    template<
        typename Category,
        typename T,
        typename Distance = s32,
        typename Pointer = T*,
        typename Reference = T&
    >
    class iterator {
    public:
        typedef Category iterator_category;
        typedef T value_type;
        typedef Distance difference_type;
        typedef Pointer pointer;
        typedef Reference reference;
        iterator& operator=(const iterator &rOther) NO_INLINE {return *this;}
    };
}

namespace JGadget {
    template<
        typename Category,
        typename T,
        typename Distance = s32,
        typename Pointer = T*,
        typename Reference = T&
    >
    class TIterator : std::iterator<Category, T, Distance, Pointer, Reference> {
    public:
        const TIterator& operator=(const TIterator &rOther) NO_INLINE {
            std::iterator<Category, T, Distance, Pointer, Reference>::operator=(rOther);
            return *this;
        }
    };
    class TLinkListNode {
    public:
        INLINE_FUNC_DECL_NO_ARG(TLinkListNode) : mPrev(nullptr), mNext(nullptr) {}
        TLinkListNode() NO_INLINE;
        TLinkListNode* getNext() const NO_INLINE;
        TLinkListNode *mNext;
        TLinkListNode *mPrev;
    };
    class TNodeLinkList {
    public:
        struct iterator {
            struct iteratorData {
                TLinkListNode *curr;
                inline iteratorData(TLinkListNode *curr) : curr(curr) {}
                inline bool operator!=(const iteratorData &rOther) {return curr == rOther.curr;}
            };
            iterator() NO_INLINE;
            inline iterator(iteratorData data) : curr(data.curr) {}
            iterator(TLinkListNode *) NO_INLINE;
            inline iterator(TLinkListNode *node, bool pretend) : curr(node) {}
            iterator(const iterator &) NO_INLINE;
            iterator& operator++() NO_INLINE;
            TLinkListNode* operator->() const NO_INLINE;
            inline iteratorData getData() const {return iteratorData(curr);}
            TLinkListNode *curr;
        };
        TNodeLinkList() NO_INLINE;
        ~TNodeLinkList();
        void Initialize_() NO_INLINE;
        iterator::iteratorData begin() NO_INLINE;
        iterator::iteratorData end() NO_INLINE;
        iterator::iteratorData Insert(iterator, TLinkListNode *);
        TLinkListNode* Erase(TLinkListNode *);
        void Remove(TLinkListNode *);

        INLINE_FUNC_DECL_NO_ARG(TNodeLinkList) : CALL_INLINE_FUNC_NO_ARG(mEnd) {
            Initialize_();
        }

        void splice(iterator, TNodeLinkList &, iterator);
        u32 _0;
        TLinkListNode mEnd;
    };
    template<typename T, int NODE_OFFSET>
    class TLinkList : public TNodeLinkList {
    public:
        struct iterator :
            public TIterator<std::bidirectional_iterator_tag, T>,
            public TNodeLinkList::iterator
        {
            struct iteratorData : TNodeLinkList::iterator::iteratorData {
                inline iteratorData(TLinkListNode *curr) : TNodeLinkList::iterator::iteratorData(curr) {}
            };
            inline iterator(iteratorData data) : TNodeLinkList::iterator(data) {}
            iterator() NO_INLINE : TNodeLinkList::iterator() {}
            inline iterator(TLinkListNode *iter) : curr(iter) {}
            iterator(TNodeLinkList::iterator iter) NO_INLINE : TNodeLinkList::iterator(iter) {}
            iterator(const iterator &rOther) : TNodeLinkList::iterator(rOther.curr, false) {
                //curr = rOther.curr;
            }
            const iterator& operator=(const iterator &rOther) NO_INLINE {
                TIterator<std::bidirectional_iterator_tag, T>::operator=(rOther);
                curr = rOther.curr;
                return *this;
            }
            iterator& operator++() NO_INLINE {
                TNodeLinkList::iterator::operator++();
                return *this;
            }
            T* operator->() const NO_INLINE {
                return TLinkList<T, NODE_OFFSET>::Element_toValue (
                    TNodeLinkList::iterator::operator->()
                );
            }
            T* operator*() const NO_INLINE {
                return operator->();
            }
        };
        
        TLinkList() : TNodeLinkList() {}

        ~TLinkList() NO_INLINE {};
        
        iterator::iteratorData begin() NO_INLINE {
            return iterator::iteratorData(
                iterator(
                    TNodeLinkList::iterator(
                        TNodeLinkList::begin()
                    )
                ).curr
            );
        }
        
        iterator::iteratorData end() NO_INLINE {
            return iterator::iteratorData(iterator(TNodeLinkList::end()).curr);
        }
        void Push_back(T *element) NO_INLINE {
            Insert(iterator(end()), element);
        }
        iterator::iteratorData Insert(iterator pos, T *element) NO_INLINE {
            return iterator::iteratorData(
                iterator(
                    TNodeLinkList::iterator(
                        TNodeLinkList::Insert(
                            TNodeLinkList::iterator(pos.curr, false),
                            Element_toNode(element)
                        )
                    )
                ).curr
            );
        }
        static TLinkListNode* Element_toNode(T *element) NO_INLINE {
            return (TLinkListNode *)((u8 *)element - NODE_OFFSET);
        }
        static T* Element_toValue(TLinkListNode *element) NO_INLINE {
            return (T *)((u8 *)element + NODE_OFFSET);
        }
        void Remove(T* element) NO_INLINE {
            TNodeLinkList::Remove(Element_toNode(element));
        }
    };
}
