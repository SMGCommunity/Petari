#pragma once

#include "JSystem/JKernel/JKRArchive.h"

class JKRMemArchive : public JKRArchive
{
public:
    JKRMemArchive();
    JKRMemArchive(s32, JKRArchive::EMountDirection);

    virtual ~JKRMemArchive();

    void fixedInit(s32);

    u32 _64;
    u32 _68;
    u32 _6C;
};