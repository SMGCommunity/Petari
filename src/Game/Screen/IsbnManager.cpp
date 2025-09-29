#include "Game/Screen/IsbnManager.hpp"
// #include <nw4r/lyt/arcResourceAccessor.h>
#include <nw4r/lyt/layout.h>

const char* IsbnManager::cIsbnNumberPaneName[] = {
    "T_number_00",
    "T_number_01",
    "T_number_02",
    "T_number_03",
    "T_number_04",
};
const char* IsbnManager::cRegistNumberPaneName[] = {
    "T_number_05",
    "T_number_06",
    "T_number_07",
};
const char* IsbnManager::cOtherNumberPaneName[] = {
    "T_number_08",
    "T_number_09",
};

IsbnManager::IsbnManager(MEMAllocator* pAllocator) :
    _0(true),
    mpAllocator(pAllocator),
    mpLayout(nullptr),
    mpResAccessor(nullptr),
    mDrawInfo(),
    _68(0),
    _64(180),
    _66(180),
    mpIsbnBuffer(nullptr),
    mpLayoutBuffer(nullptr),
    mpResAccBuffer(nullptr)
{
    
}

IsbnManager::~IsbnManager() {
    
}

void IsbnManager::setAdjustRate(f32 x, f32 y) {
    // TODO: Should be `mDrawInfo.SetLocationAdjustScale(nw4r::math::VEC2(x, y))`
    mDrawInfo.mLocationAdjustScale = nw4r::math::VEC2(x, y);
    // TODO: Should be `mDrawInfo.SetLocationAdjust(true);
    mDrawInfo.mFlag.locationAdjust = true;
}

bool IsbnManager::calc(bool param1) {
    bool result = false;

    if (mpLayout == nullptr) {
        return false;
    }

    if (param1 && _68 > _64) {
        result = true;
    }
    else if (_68 > _66) {
        result = true;
    }

    calculateView();
    mpLayout->CalculateMtx(mDrawInfo);

    _68++;
    _0 = !result;

    return result;
}

void IsbnManager::draw() {
    if (mpLayout != nullptr) {
        mpLayout->Draw(mDrawInfo);
    }
}

void IsbnManager::reset() {
    _68 = 0;
    _0 = true;
}
