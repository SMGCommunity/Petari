#pragma once

#include "nw4r/db/assert.h"
#include "nw4r/misc.h"
#include "nw4r/ut/inlines.h"

namespace nw4r {
    namespace ut {
        class LinkListNode;

        namespace detail {
            class LinkListImpl;

            template < typename T, typename Difference = PtrDiff, typename Pointer = T*, typename Reference = T& >
            class Iterator {
            public:
                typedef T value_type;
                typedef Difference difference_type;
                typedef Pointer pointer;
                typedef Reference reference;
            };
        };  // namespace detail

        class LinkListNode : private NonCopyable {
        public:
            typedef LinkListNode Self;

            explicit LinkListNode() : mNext(0), mPrev(0) {
            }

            LinkListNode* GetNext() const {
                return mNext;
            }

            LinkListNode* GetPrev() const {
                return mPrev;
            }

            friend bool operator==(const Self& r1, const Self& r2) {
                return &r1 == &r2;
            }
            friend bool operator!=(const Self& r1, const Self& r2) {
                return !(r1 == r2);
            }

            LinkListNode* mNext;
            LinkListNode* mPrev;

            friend class detail::LinkListImpl;
        };

        namespace detail {
            class LinkListImpl : private NonCopyable {
            public:
                typedef LinkListImpl Self;
                typedef unsigned long size_type;
                typedef PtrDiff difference_type;
                typedef LinkListNode Node;
                typedef Node value_type;
                typedef value_type* pointer;
                typedef const value_type* const_pointer;
                typedef value_type& reference;
                typedef const value_type& const_reference;

                class Iterator;
                class ConstIterator;

                class Iterator : public detail::Iterator< value_type > {
                public:
                    typedef Iterator TIt;
                    typedef detail::Iterator< value_type > TBaseIt;

                    explicit Iterator() : mPointer(0) {
                    }

                    friend bool operator==(TIt it1, TIt it2) {
                        return it1.mPointer == it2.mPointer;
                    }
                    friend bool operator!=(TIt it1, TIt it2) {
                        return !(it1 == it2);
                    }

                    reference operator*() const {
                        return *mPointer;
                    }

                    pointer operator->() const {
                        return mPointer;
                    }

                    TIt& operator++() {
                        mPointer = mPointer->GetNext();
                        return *this;
                    }

                    TIt operator++(int) {
                        const TIt it(*this);
                        (void)++*this;
                        return it;
                    }

                    TIt& operator--() {
                        mPointer = mPointer->GetPrev();
                        return *this;
                    }

                    TIt operator--(int) {
                        const TIt it(*this);
                        (void)--*this;
                        return it;
                    }

                    explicit Iterator(pointer p) : mPointer(p) {
                    }

                    pointer mPointer;
                };

                typedef Iterator Iterator_alias_;
                class ConstIterator : public detail::Iterator< value_type > {
                public:
                    typedef ConstIterator TIt;
                    typedef detail::Iterator< value_type > TBaseIt;
                    typedef const_pointer pointer;
                    typedef const_reference reference;

                    explicit ConstIterator() : mPointer(NULL) {
                    }
                    ConstIterator(Iterator_alias_ it) : mPointer(it.mPointer) {
                    }

                    reference operator*() const {
                        NW4R_NULL_ASSERT_AT(230, mPointer);
                        return *mPointer;
                    }
                    pointer operator->() const {
                        return mPointer;
                    }

                    TIt& operator++() {
                        mPointer = mPointer->GetNext();
                        return *this;
                    }
                    TIt operator++(int) {
                        const TIt it(*this);
                        (void)++*this;
                        return it;
                    }
                    TIt& operator--() {
                        mPointer = mPointer->GetPrev();
                        return *this;
                    }
                    TIt operator--(int) {
                        const TIt it(*this);
                        (void)--*this;
                        return it;
                    }

                    friend bool operator==(TIt it1, TIt it2) {
                        return it1.mPointer == it2.mPointer;
                    }
                    friend bool operator!=(TIt it1, TIt it2) {
                        return !(it1 == it2);
                    }

                private:
                    explicit ConstIterator(pointer p) : mPointer(p) {
                    }

                    pointer mPointer;

                    friend class LinkListImpl;
                };
                explicit LinkListImpl() {
                    Initialize_();
                }

                ~LinkListImpl();

                Iterator GetBeginIter() {
                    return Iterator(mNode.GetNext());
                }

                Iterator GetEndIter() {
                    return Iterator(&mNode);
                }

                void PushFront(pointer p) {
                    Insert(GetBeginIter(), p);
                }

                void PushBack(pointer p) {
                    Insert(GetEndIter(), p);
                }

                Iterator Erase(pointer p);
                Iterator Erase(Iterator it);
                Iterator Erase(Iterator, Iterator);
                Iterator Insert(Iterator, pointer);

                void Initialize_() {
                    mSize = 0;
                    mNode.mNext = &mNode;
                    mNode.mPrev = &mNode;
                }

                void Clear();

                unsigned long mSize;
                LinkListNode mNode;
            };
        };  // namespace detail

        template < typename T, PtrDiff TNOffset >
        class LinkList : private detail::LinkListImpl {
        public:
            typedef detail::LinkListImpl Base;
            typedef LinkList Self;
            using Base::Node;

            using Base::difference_type;
            using Base::size_type;

            typedef T value_type;
            typedef value_type* pointer;
            typedef const value_type* const_pointer;
            typedef value_type& reference;
            typedef const value_type& const_reference;

            typedef Base::Iterator TIt_base_;
            typedef Base::ConstIterator TItC_base_;

            class Iterator;

            class Iterator : public detail::Iterator< value_type > {
            public:
                typedef Iterator TIt;
                typedef detail::Iterator< value_type > TBaseIt;

                explicit Iterator() {
                }
                explicit Iterator(TIt_base_ it) : it_(it) {
                }

                reference operator*() const {
                    pointer p = operator->();
                    NW4R_NULL_ASSERT_AT(403, p);
                    return *p;
                }
                pointer operator->() const {
                    return GetPointerFromNode(it_.operator->());
                }

                friend bool operator==(TIt it1, TIt it2) {
                    return it1.it_ == it2.it_;
                }
                friend bool operator!=(TIt it1, TIt it2) {
                    return !(it1 == it2);
                }

                TIt& operator++() {
                    (void)++it_;
                    return *this;
                }

                TIt operator++(int) {
                    const TIt it(*this);
                    (void)++*this;
                    return it;
                }

                TIt& operator--() {
                    (void)--it_;
                    return *this;
                }

                TIt operator--(int) {
                    const TIt it(*this);
                    (void)--*this;
                    return it;
                }

                TIt_base_ it_;
            };

            typedef Iterator Iterator_alias_;
            class ConstIterator : public detail::Iterator< value_type > {
            public:
                typedef ConstIterator TIt;
                typedef detail::Iterator< value_type > TBaseIt;
                typedef const_pointer pointer;
                typedef const_reference reference;

                explicit ConstIterator() {
                }
                ConstIterator(Iterator_alias_ it) : it_(it.it_) {
                }

                reference operator*() const {
                    pointer p = operator->();
                    NW4R_NULL_ASSERT_AT(447, p);
                    return *p;
                }
                pointer operator->() const {
                    return GetPointerFromNode(it_.operator->());
                }

                TIt& operator++() {
                    (void)++it_;
                    return *this;
                }
                TIt operator++(int) {
                    const TIt it(*this);
                    (void)++*this;
                    return it;
                }
                TIt& operator--() {
                    (void)--it_;
                    return *this;
                }
                TIt operator--(int) {
                    const TIt it(*this);
                    (void)--*this;
                    return it;
                }

                friend bool operator==(TIt it1, TIt it2) {
                    return it1.it_ == it2.it_;
                }
                friend bool operator!=(TIt it1, TIt it2) {
                    return !(it1 == it2);
                }

            protected:
                explicit ConstIterator(TItC_base_ it) : it_(it) {
                }

                TItC_base_ it_;

                friend class Self;
            };
            explicit LinkList() {
            }

            Iterator Insert(Iterator it, pointer p) {
                return Iterator(Base::Insert(it.it_, GetNodeFromPointer(p)));
            }

            void PushFront(pointer p) {
                (void)Insert(GetBeginIter(), p);
            }

            void PushBack(pointer p) {
                (void)Insert(GetEndIter(), p);
            }

            Iterator GetBeginIter() {
                return Iterator(Base::GetBeginIter());
            }

            ConstIterator GetBeginIter() const {
                return ConstIterator(const_cast< Self* >(this)->GetBeginIter());
            }

            Iterator GetEndIter() {
                return Iterator(Base::GetEndIter());
            }

            ConstIterator GetEndIter() const {
                return ConstIterator(const_cast< Self* >(this)->GetEndIter());
            }

            Iterator Erase(Iterator it) {
                return Iterator(Base::Erase(it.it_));
            }

            Iterator Erase(pointer p) {
                return Iterator(Base::Erase(GetNodeFromPointer(p)));
            }

            unsigned long GetSize() const {
                return mSize;
            }

            static const_pointer GetPointerFromNode(const Node* p) {
                NW4R_NULL_ASSERT_AT(578, p);
                return reinterpret_cast< const_pointer >(reinterpret_cast< IntPtr >(p) - TNOffset);
            }

            static pointer GetPointerFromNode(Node* p) {
                NW4R_NULL_ASSERT_AT(573, p);
                return reinterpret_cast< pointer >(reinterpret_cast< IntPtr >(p) - TNOffset);
            }

            static Node* GetNodeFromPointer(pointer p) {
                return reinterpret_cast< Node* >(reinterpret_cast< IntPtr >(p) + TNOffset);
            }
        };
    };  // namespace ut
};  // namespace nw4r
