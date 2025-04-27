#pragma once

#include "Inline.hpp"
#include <revolution.h>

namespace MR {
    /// @brief A contiguous reallocatable array.
    template<class T>
    class AssignableArray {
    public:
        typedef T Item;

        /// @brief Creates a new `AssignableArray`.
        AssignableArray() {
            mArr = 0;
            mMaxSize = 0;
        }

        /// @brief Destroys the `AssignableArray`.
        ~AssignableArray() {
            if (mArr != NULL) {
                delete[] mArr;
            }
        }

        /// @brief Returns the element at the given position.
        /// @param idx The position of the element.
        /// @return The reference to the element.
        T& operator[](int idx) {
            return mArr[idx];
        }

        /// @brief Returns the element at the given position.
        /// @param idx The position of the element.
        /// @return The reference to the element.
        const T& operator[](int idx) const {
            return mArr[idx];
        }

        /// @brief Allocates memory for the array.
        /// @param num The number of elements.
        void init(s32 num) {
            mArr = new T[num];
            mMaxSize = num;
        }

        /// @brief Returns the number of elements.
        /// @return The number of elements.
        int size() const {
            return mMaxSize;
        }

        /// @brief Returns an iterator to the first element.
        /// @return The pointer to the first element.
        T* begin() {
            return &mArr[0];
        }

        /// @brief Returns an iterator to the first element.
        /// @return The pointer to the first element.
        const T* begin() const {
            return &mArr[0];
        }

        /// @brief Returns an iterator past the last element.
        /// @return The pointer past the last element.
        T* end() {
            return &mArr[mMaxSize];
        }

        /// @brief Returns an iterator past the last element.
        /// @return The pointer past the last element.
        const T* end() const {
            return &mArr[mMaxSize];
        }

        /// @brief The pointer to the array of elements.
        /* 0x0 */ T* mArr;

        /// @brief The number of elements.
        /* 0x4 */ s32 mMaxSize;
    };

    /// @brief A contiguous fixed-size array.
    template<class T, int N>
    class FixedArray {
    public:
        typedef T Item;

        /// @brief Returns the element at the given position.
        /// @param idx The position of the element.
        /// @return The reference to the element.
        T& operator[](int idx) {
            return mArr[idx];
        }

        /// @brief Returns the element at the given position.
        /// @param idx The position of the element.
        /// @return The reference to the element.
        const T& operator[](int idx) const {
            return mArr[idx];
        }

        /// @brief Returns the number of elements.
        /// @return The number of elements.
        int size() const {
            return N;
        }

        /// @brief Returns an iterator to the first element.
        /// @return The pointer to the first element.
        T* begin() {
            return &mArr[0];
        }

        /// @brief Returns an iterator to the first element.
        /// @return The pointer to the first element.
        const T* begin() const {
            return &mArr[0];
        }

        /// @brief Returns an iterator past the last element.
        /// @return The pointer past the last element.
        T* end() {
            return &mArr[N];
        }

        /// @brief Returns an iterator past the last element.
        /// @return The pointer past the last element.
        const T* end() const {
            return &mArr[N];
        }

    private:
        /// @brief The array of elements.
        /* 0x0 */ T mArr[N];
    };

    /// @brief A contiguous growable array.
    template<class T>
    class Vector {
    public:
        /// @brief Creates a new `Vector`.
        Vector() {
            clear();
        }

        /// @brief Destroys the `Vector`.
        ~Vector() {
            
        }

        /// @brief Returns the element at the given position.
        /// @param idx The position of the element.
        /// @return The reference to the element.
        T::Item& operator[](int idx) {
            return mArray[idx];
        }

        /// @brief Returns the element at the given position.
        /// @param idx The position of the element.
        /// @return The reference to the element.
        const T::Item& operator[](int idx) const {
            return mArray[idx];
        }

        /// @brief Allocates memory for the array.
        /// @param num The number of elements.
        void init(s32 num) {
            mArray.init(num);
        }

        /// @brief Returns the number of elements.
        /// @return The number of elements.
        int size() const {
            return mCount;
        }

        /// @brief Returns the number of elements that can be stored without reallocating.
        /// @return The number of elements that can be stored.
        int capacity() const {
            return mArray.size();
        }

        /// @brief Appends a copy of `rItem` to the end of the container.
        /// @param rItem The reference to the value to append.
        void push_back(const T::Item &rItem) {
            u32 index = mCount;

            mCount++;
            mArray[index] = rItem;
        }

        /// @brief Removes the value at the given position from the container.
        /// @param pIter The pointer to the position where the value should be removed.
        /// @return The pointer to the position of the removed value.
        T::Item* erase(T::Item* pIter) NO_INLINE {
            if (end() - pIter - 1 > 0) {
                for (T::Item* p = pIter; p + 1 != end(); p++) {
                    *p = *(p + 1);
                }
            }

            mCount--;

            return pIter;
        }

        /// @brief Removes all values from the container.
        void clear() {
            mCount = 0;
        }

        /// @brief Returns an iterator to the first element.
        /// @return The pointer to the first element.
        T::Item* begin() {
            return &mArray[0];
        }

        /// @brief Returns an iterator to the first element.
        /// @return The pointer to the first element.
        const T::Item* begin() const {
            return &mArray[0];
        }

        /// @brief Returns an iterator past the last element.
        /// @return The pointer past the last element.
        T::Item* end() {
            return &mArray[mCount];
        }

        /// @brief Returns an iterator past the last element.
        /// @return The pointer past the last element.
        const T::Item* end() const {
            return &mArray[mCount];
        }

        /// @brief The array of elements.
        /* 0x0 */ T mArray;

        /// @brief The number of valid elements.
        /* 0x? */ s32 mCount;
    };

    template<class T, int S>
    class FixedRingBuffer {
    public:
        class iterator {
        public:
            iterator(T* head, T* tail) {
                mHead = head;
                mTail = tail;
                mEnd = head + S;
            }

            T* mHead;   // 0x0
            T* mTail;   // 0x4
            T* mEnd;    // 0x8
        };
    };
};
