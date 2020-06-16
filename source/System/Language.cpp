#include "System/Language.h"
#include "MR/StringUtil.h"
#include <revolution/sc.h>

namespace MR
{
    u32 getLanguageFromIPL()
    {
        return MR::getLanguage() & 0xFFFFFF0F;
    }

    const char* getCurrentLanguagePrefix()
    {
        u32 lang = MR::getLanguage();

        u32 curIdx = 0;

        while (curIdx < LANGUAGE_NUM)
        {
            if (lang == cLanguages[curIdx].mLanguageID)
            {
                return cLanguages[curIdx].mLanguageString;
            }

            curIdx++;
        }

        return "English";
    }

    const char* getCurrentRegionPrefix()
    {
        const char* langPrefix = MR::getCurrentLanguagePrefix();
        char extr;
        MR::extractString(&extr, langPrefix, 2, 3);

        if (MR::isEqualString(&extr, "Jp"))
        {
            return "Jp";
        }
        else
        {
            if (MR::isEqualString(&extr, "Us"))
            {
                return "Us";
            }
            else
            {
                if (MR::isEqualString(&extr, "Eu"))
                {
                    return "Eu";
                }
                else
                {
                    if (MR::isEqualString(&extr, "Cn"))
                    {
                        return "Cn";
                    }
                    else
                    {
                        if (MR::isEqualString(&extr, "Kr"))
                        {
                            return "Kr";
                        }
                        else
                        {
                            return 0;
                        }
                    }
                }
            }
        }
    }

    u32 getLanguageNum()
    {
        return LANGUAGE_NUM;
    }

    const char* getLanguagePrefixByIndex(u32 idx)
    {
        return cLanguages[idx].mLanguageString;
    }
};