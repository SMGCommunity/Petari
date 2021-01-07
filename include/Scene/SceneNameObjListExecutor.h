#pragma once

#include "NameObj/NameObjListExecutor.h"

namespace
{
    const CategoryListInitialTable cMovementListInitTable[47] =
    {
        { 0, 1 },
        { 1, 0x10 },
        { 2, 1 },
        { 3, 0x10 },
        { 4, 1 },
        { 5, 1 },
        { 6, 0x20 },
        { 7, 0x10 },
        { 8, 1 },
        { 9, 1 },
        { 0xA, 1 },
        { 0x1B, 1 },
        { 0xB, 1 },
        { 0xC, 0x20 },
        { 0xD, 0x100 },
        { 0xE, 0x60 },
        { 0xF, 0x20 },
        { 0x10, 1 },
        { 0x13, 8 },
        { 0x11, 8 },
        { 0x12, 0x10 },
        { 0x16, 1 },
        { 0x18, 1 },
        { 0x19, 1 },
        { 0x20, 1 },
        { 0x2D, 1 },
        { 0x1A, 1 },
        { 0x1D, 0x60 },
        { 0x25, 0x10 },
        { 0x26, 0x20 },
        { 0x27, 1 },
        { 0x28, 0x60 },
        { 0x1F, 0x80 },
        { 0x2A, 0x400 },
        { 0x2B, 0x300 },
        { 0x15, 0x40 },
        { 0x1C, 0x800 },
        { 0x1E, 0x400 },
        { 0x29, 0x100 },
        { 0x22, 0x800 },
        { 0x23, 0x200 },
        { 0x2C, 0x200 },
        { 0x21, 0x200 },
        { 0x24, 8 },
        { 0x17, 4 },
        { 0x14, 1 },
        { 0xFFFFFFFF, 0 }
    };
}

class SceneNameObjListExecutor : public NameObjListExecutor
{
public:
    // todo -- why doesn't this properly inline?
    inline SceneNameObjListExecutor() : NameObjListExecutor() { }

    virtual ~SceneNameObjListExecutor();
    virtual void initMovementList();
    virtual void initCalcAnimList();
    virtual void initCalcViewAndEntryList();
    virtual void initDrawList();
};