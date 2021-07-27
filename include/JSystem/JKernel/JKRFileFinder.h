#pragma once

#include <revolution.h>

class JKRFileFinder
{
public:
    JKRFileFinder();

    virtual ~JKRFileFinder();

    u8 _0[0x10];
};

class JKRArcFinder : public JKRFileFinder
{
public:
    JKRArcFinder();

    u8 _14[0x10];
};