#pragma once

#include <RVLFaceLib.h>
#include <revolution.h>

/// @brief The resource creator for the head of a Mii character.
class MiiFaceRecipe {
public:
    /// @brief Creates a new `MiiFaceRecipe`.
    /// @param dataSource The type of database to obtain Mii data from.
    /// @param index The position of the Mii data in the database.
    /// @param resolution The resolution of the icon texture.
    /// @param expressionFlag The combination of facial expressions.
    MiiFaceRecipe(RFLDataSource dataSource, int index, RFLResolution resolution, u32 expressionFlag);

    /// @brief Updates the pointer to the intermediate database.
    /// @param pMiddleDB The pointer to the intermediate database.
    void setDatabase(RFLMiddleDB* pMiddleDB);

    /// @brief Determines if the Mii character is assuming the given facial expression.
    /// @param expression The facial expression to test.
    /// @return `true` if the Mii character is assuming the facial expression, `false` otherwise.
    bool hasExpression(RFLExpression expression) const;

    /// @brief Computes the required size of the head model data buffer, in bytes.
    /// @return The required size of the data buffer, in bytes.
    u32 getModelBufferSize() const;

    /// @brief Creates the head model.
    /// @param pCharModel The pointer to the descriptor of the head model.
    /// @param pBuffer The output pointer for the model data buffer.
    void initFaceModel(RFLCharModel* pCharModel, void* pBuffer) const;

    /// @brief Creates the icon texture.
    /// @param pBuffer The output pointer for the texture data buffer.
    /// @param width The width of the texture, in pixels.
    /// @param height The height of the texture, in pixels.
    /// @param bgColor The color of the background.
    /// @retval RFLErrcode_Success The function was executed successfully.
    /// @retval RFLErrcode_NotAvailable The function is not ready to be executed.
    /// @retval RFLErrcode_Broken The Mii data is corrupted.
    RFLErrcode makeIcon(void* pBuffer, int width, int height, GXColor bgColor) const;

    /// @brief Creates the icon texture with the background colored using the Mii character's favorite color.
    /// @param pBuffer The output pointer for the texture data buffer.
    /// @param width The width of the texture, in pixels.
    /// @param height The height of the texture, in pixels.
    /// @retval RFLErrcode_Success The function was executed successfully.
    /// @retval RFLErrcode_NotAvailable The function is not ready to be executed.
    /// @retval RFLErrcode_Broken The Mii data is corrupted.
    RFLErrcode makeIconWithFavoriteColor(void* pBuffer, int width, int height) const;

    /// @brief The type of database to obtain Mii data from.
    /* 0x00 */ RFLDataSource mDataSource;

    /// @brief The pointer to the intermediate database.
    /* 0x04 */ RFLMiddleDB* mMiddleDB;

    /// @brief The position of the Mii data in the database.
    /* 0x08 */ u16 mIndex;

    /// @brief The resolution of the icon texture.
    /* 0x0C */ RFLResolution mResolution;

    /// @brief The combination of facial expressions.
    /* 0x10 */ u32 mExpressionFlag;
};
