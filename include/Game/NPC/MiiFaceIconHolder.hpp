#pragma once

#include "Game/NameObj/NameObj.hpp"

class MiiFaceIcon;

/// @brief The container for each `MiiFaceIcon` instance.
class MiiFaceIconHolder : public NameObj {
public:
    /// @brief Creates a new `MiiFaceIconHolder`.
    /// @param iconNum The number of `MiiFaceIcon` objects to allocate memory for.
    /// @param pName The pointer to the null-terminated name of the object.
    MiiFaceIconHolder(u32 iconNum, const char* pName);

    /// @brief Creates the icon texture for the first Mii character with an open request.
    void drawIcons();

    /// @brief Associates `pIcon` with the underlying array.
    /// @param pIcon The reference to the resource creator for the icon of a Mii character.
    void registerIcon(MiiFaceIcon* pIcon);

public:
    /// @brief The maximum number of icon resource creator instances.
    /* 0x0C */ u32 mIconNumMax;

    /// @brief The current number of icon resource creator instances.
    /* 0x10 */ u32 mIconNum;

    /// @brief The pointer to the array of pointers to icon resource creator instances.
    /* 0x14 */ MiiFaceIcon** mIcon;
};

namespace MR {
    /// @brief Returns the working `MiiFaceIconHolder` instance.
    MiiFaceIconHolder* getMiiFaceIconHolder();

    /// @brief Associates `pIcon` with the working `MiiFaceIconHolder` instance.
    /// @param pIcon The reference to the resource creator for the icon of a Mii character.
    void registerMiiFaceIcon(MiiFaceIcon* pIcon);
};  // namespace MR
