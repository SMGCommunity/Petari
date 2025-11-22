#include "Game/System/LuigiLeftSupplier.hpp"
#include "Game/System/GameDataFunction.hpp"

LuigiLeftSupplier::LuigiLeftSupplier() : _0(false) {}

void LuigiLeftSupplier::syncWithFlags() {
    if (!GameDataFunction::isDataMario()) {
        _0 = true;
    } else {
        _0 = false;
    }
}
