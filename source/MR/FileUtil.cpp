#include "MR/FileUtil.h"
#include "MR/StringUtil.h"
#include "System/Language.h"
#include <revolution/dvd.h>
#include <stdio.h>

namespace MR
{
    s32 isFileExist(const char *file, bool considerRegions)
    {
        s32 entryNum;

        if (considerRegions)
        {
            entryNum = MR::convertPathToEntrynumConsideringLanguage(file);
        }
        else
        {
            entryNum = DVDConvertPathToEntrynum(file);
        }
        
        return ((entryNum >> 31) & 0x1) ^ 1;
    }

    u32 getFileSize(const char *file, bool considerRegions)
    {
        s32 entryNum;

        if (considerRegions)
        {
            entryNum = MR::convertPathToEntrynumConsideringLanguage(file);
        }
        else
        {
            entryNum = DVDConvertPathToEntrynum(file);
        }

        DVDFileInfo inf;
        DVDFastOpen(entryNum, &inf);
        u32 size = inf.length;
        DVDClose(&inf);
        return size;
    }

    s32 convertPathToEntrynumConsideringLanguage(const char *file)
    {
        char val;
        MR::addFilePrefix(&val, 0x100, file, MR::getCurrentLanguagePrefix());

        if (!MR::isFileExist(&val, false))
        {
            snprintf(&val, 0x100, "%s", file);
        }

        return DVDConvertPathToEntrynum(&val);
    }
};