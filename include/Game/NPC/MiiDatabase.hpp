#pragma once

#include "Game/Util/Array.hpp"

struct RFLStoreData;

class MiiDatabase {
public:
    /// @brief Destroys the `MiiDatabase`.
    ~MiiDatabase();

public:
    /* 0x0 */ u8* _0;
    /* 0x4 */ MR::AssignableArray< RFLStoreData* >* mStoreDataArray;
};
