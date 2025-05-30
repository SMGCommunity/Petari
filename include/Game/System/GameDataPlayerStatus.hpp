#pragma once

#include "Game/System/BinaryDataChunkHolder.hpp"

class GameDataPlayerStatus : public BinaryDataChunkBase {
public:
    /// @brief Creates a new `GameDataPlayerStatus`.
    GameDataPlayerStatus();

    /// @brief Returns the identifying hash code for this object.
    /// @return The identifying hash code.
    virtual u32 makeHeaderHashCode() const;

    /// @brief Returns the identifying magic number for this object.
    /// @return The identifying magic number.
    virtual u32 getSignature() const;

    /// @brief Writes the member variables of this object to the given data buffer.
    /// @param pBuffer The pointer to the data buffer.
    /// @param size The size of the data buffer, in bytes.
    /// @return The number of bytes written.
    virtual s32 serialize(u8* pBuffer, u32 size) const;

    /// @brief Reads the member variables of this object from the given data buffer.
    /// @param pBuffer The pointer to the data buffer.
    /// @param size The size of the data buffer, in bytes.
    /// @return `0` or `1` if successful, unsuccessful otherwise.
    virtual s32 deserialize(const u8* pBuffer, u32 size);

    /// @brief Initializes the member variables of this object.
    virtual void initializeData();

    /// @brief Returns the number of extra lives.
    /// @return The number of extra lives.
    s32 getPlayerLeft() const;

    /// @brief Adds to the number of extra lives by the given term.
    /// @param num The number of extra lives to add.
    void addPlayerLeft(int num);

    /// @brief Determines if enough extra lives were earned during the previous session.
    /// @return `true` if enough extra lives were earned, `false` otherwise.
    bool isPlayerLeftSupply() const;

    /// @brief Reduces the number of extra lives from the previous session to zero.
    void offPlayerLeftSupply();

    /// @brief Adds to the number of universally collected Star Bits by the given term.
    /// @param num The number of Star Bits to add.
    void addStockedStarPiece(int num);

private:
    /// @brief The number of extra lives.
    /* 0x04 */ u16 mPlayerLeft;

    /// @brief The number of universally collected Star Bits.
    /* 0x08 */ u32 mStockedStarPiece;

    /// @brief TODO.
    /* 0x0C */ u8 mStoryProgress;

    /// @brief The number of extra lives from the previous session.
    /* 0x0E */ u16 mPlayerLeftSupply;
};
