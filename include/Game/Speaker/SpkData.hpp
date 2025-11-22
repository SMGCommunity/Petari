#pragma once

#include "Game/Speaker/SpkTable.hpp"
#include "Game/Speaker/SpkWave.hpp"
#include "JSystem/JKernel/JKRArchive.hpp"
#include "revolution.h"

// This is something JKernel related, but idk what it is for the time being.
extern void* FUN_8040f918(JKRArchive* pArchive, u16 id);

class SpkData {
public:
    SpkData(JKRArchive*);

    void loadTable(u16);
    void loadWave(u16);
    bool isValid() const;

    SpkTable mTable;       // 0x0
    SpkWave mWave;         // 0x4
    JKRArchive* mArchive;  // 0x14
};