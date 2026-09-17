#pragma once

#include "Game/MapObj/BenefitItemObj.hpp"

class BenefitItemOneUp : public BenefitItemObj {
public:
    /// @brief Creates a new `BenefitItemOneUp`.
    /// @param pName A pointer to the null-terminated name of the object.
    /// @param pModelName A pointer to the null-terminated name of the model.
    BenefitItemOneUp(const char* pName) : BenefitItemObj(pName, "KinokoOneUp") {
    }

    BenefitItemOneUp(const char* pName, const char* pModelName) : BenefitItemObj(pName, pModelName) {
    }
};
