#ifndef SPINDRIVERPATHSTORAGE_H
#define SPINDRIVERPATHSTORAGE_H

#include "types.h"

class SpinDriverPathStorageOne
{
public:
    SpinDriverPathStorageOne();

    void reset();
    void registerToUse(s32, s32);
    void serialize(u8 *) const;
    void deserialize(const u8 *, s32);

    s32 mZoneID; // _0
    s32 _4;
    f32 _8;
};

class SpinDriverPathStorageScenario
{
public:
    SpinDriverPathStorageScenario();

    void resetAllData();
    void setup(s32, s32, f32 *);
    void serialize(u8 *, u32) const;
    void deserialize(const u8 *, u32);
    s32 calcMaxZoneId() const;

    SpinDriverPathStorageOne* mPaths; // _0
    u32 mPathCount; // _4
    u32 _8;
};

#endif // SPINDRIVERPATHSTORAGE_H