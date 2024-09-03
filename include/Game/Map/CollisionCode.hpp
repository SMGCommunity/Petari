#pragma once

#include <revolution.h>

#include "Game/Map/HitInfo.hpp"
#include "Game/Util.hpp"

class CodeTable;

class CollisionCode {
public:
    CollisionCode();

    const char* getFloorCodeString(const Triangle &);
    const char* getWallCodeString(const Triangle &);
    const char* getSoundCodeString(const Triangle &);
    const char* getSoundCodeString(const JMapInfoIter &);

    u32 getCameraID(const Triangle &);
    u32 getCode(const JMapInfoIter &, CodeTable *, const char *);

    void createFloorTable();
    void createWallTable();
    void createSoundTable();
    void createCameraTable();

    u32 getFloorCode(const JMapInfoIter &);
    u32 getWallCode(const JMapInfoIter &);
    u32 getSoundCode(const JMapInfoIter &);
    u32 getCameraCode(const JMapInfoIter &);
    u32 getCameraID(const JMapInfoIter &);

    CodeTable* mFloorTable;     // 0x0
    CodeTable* mWallTable;      // 0x4
    CodeTable* mSoundTable;     // 0x8
    CodeTable* mCameraTable;    // 0xC
};

class CodeTable {
public:
    CodeTable(u32);

    void add(const char *, u32);
    const char* getString(u32);

    u32 mNumCodes;      // 0x0
    u32 mMaxNumCodes;   // 0x4
    u32* mHashCodes;    // 0x8
    u32* _C;
    const char** mNames; // 0x10
};