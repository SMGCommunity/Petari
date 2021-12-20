#pragma once

#include "JSystem/JKernel/JKRArchive.h"
#include "Game/Speaker/SpkTable.h"
#include "revolution.h"

class SpkData {
public:
    SpkData(JKRArchive * archive);

    void loadTable(u16 table);
    void loadWave(u16 wave);
    bool isValid() const;

    SpkTable table;
    u32 unk_4; // This is an SpkWave.
    u32 unk_5;
};