#pragma once

#include <revolution.h>

namespace MR {
    /// @brief A contiguous space-efficient array of bits.
    class BitArray {
    public:
        /// @brief Creates a new `BitArray`.
        /// @param bitNum The number of bits.
        BitArray(int bitNum);

        /// @brief Determines if the value of the given bit is equal to `1`.
        /// @param bitIdx The position of the bit.
        /// @return `true` if the value of the bit is equal to `1`, `false` otherwise.
        bool isOn(int bitIdx) const;

        /// @brief Updates the value of the given bit.
        /// @param bitIdx The position of the bit.
        /// @param isOn Determines if the value of the bit should be set to `1`.
        void set(int bitIdx, bool isOn);

        /// @brief Returns the number of bits.
        /// @return The number of bits.
        int size() const {
            return mArraySize;
        }

    private:
        /// @brief The pointer to the array of bytes.
        /* 0x0 */ u8* mArray;

        /// @brief The number of bits.
        /* 0x4 */ s32 mArraySize;
    };
};
