#pragma once

#include "nw4r/misc.h"
#include "nw4r/ut/inlines.h"
#include <revolution.h>

namespace nw4r {
    namespace ut {

        namespace detail {
            class LinkListImpl;

            template<typename T, typename Difference = PtrDiff, typename Pointer = T*, typename Reference = T&>
            class Iterator {
            public:
                typedef T value_type;
                typedef Difference difference_type;
                typedef Pointer pointer;
                typedef Reference reference;
            };
        };

        class LinkListNode : public NonCopyable {
        public:
            typedef LinkListNode Self;

            explicit LinkListNode() {
                mNext = nullptr;
                mPrev = nullptr;
            }

            Self* GetNext() const {
                return mNext;
            }

            Self* GetPrev() const {
                return mPrev;
            }

        private:
            Self* mNext;
            Self* mPrev;

            friend class detail::LinkListImpl;
        };

        namespace detail {
            class LinkListImpl : public NonCopyable {
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

                class Iterator : public detail::Iterator<value_type> {
                public:
                    typedef Iterator TIt;
                    typedef detail::Iterator<value_type> TBaseIt;

                    reference operator*() const {
                        return *mPointer;
                    }

                    pointer operator->() const {
                        return mPointer;
                    }

                    TIt &operator++() {
                        mPointer = mPointer->GetNext();
                        return *this;
                    }

                    TIt operator++(int) {
                        const TIt it(*this);
                        ++*this;
                        return it;
                    }

                    TIt &operator--() {
                        mPointer = mPointer->GetPrev();
                        return *this;
                    }

                    TIt operator--(int) {
                        const TIt it(*this);
                        --*this;
                        return it;
                    }
                    
                    explicit Iterator(pointer p) {
                        mPointer = p;
                    }

                    pointer mPointer;
                };

                Iterator GetBeginIter() {
                    return Iterator(mNode.GetNext());
                }

                Iterator GetEndIter() {
                    return Iterator(&mNode);
                }

                ~LinkListImpl();

                Iterator Insert(Iterator, pointer);
                Iterator Erase(pointer);
                Iterator Erase(Iterator);
                Iterator Erase(Iterator, Iterator);
                void Clear();

                size_type mSize;
                Node mNode;
            };
        };
    };
};