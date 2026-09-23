#pragma once

#include <revolution/types.h>

struct MiiDatabaseStoreData;

class MiiDatabase {
public:
    ~MiiDatabase();

private:
    /* 0x00 */ u8* _0;
    /* 0x04 */ MiiDatabaseStoreData* mStoreData;
};
