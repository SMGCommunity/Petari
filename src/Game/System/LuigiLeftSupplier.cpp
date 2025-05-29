#include "Game/System/GameDataFunction.hpp"
#include "Game/System/LuigiLeftSupplier.hpp"

LuigiLeftSupplier::LuigiLeftSupplier() :
    _0(false)
{
    
}

void LuigiLeftSupplier::syncWithFlags() {
    if (!GameDataFunction::isDataMario()) {
        _0 = true;
    }
    else {
        _0 = false;
    }
}
