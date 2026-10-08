#include "Game/System/Language.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <revolution/sc.h>

#if (VERSION == RMGJ01)
#define SCLanguage2GameLanguageTableIndex 0
#elif (VERSION == RMGE01)
#define SCLanguage2GameLanguageTableIndex 1
#elif (VERSION == RMGP01)
#define SCLanguage2GameLanguageTableIndex 2
#elif (VERSION == RMGK01)
#define SCLanguage2GameLanguageTableIndex 4
#else
#define SCLanguage2GameLanguageTableIndex -1
#endif

namespace {
    const u8 cSCLanguage2GameLanguageTable[][10] = {
        {
            LANGUAGE_JPJAPANESE,
            LANGUAGE_USENGLISH,
            LANGUAGE_EUGERMAN,
            LANGUAGE_EUFRENCH,
            LANGUAGE_EUSPANISH,
            LANGUAGE_EUITALIAN,
            LANGUAGE_EUDUTCH,
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
            LANGUAGE_EUENGLISH,
            LANGUAGE_EUENGLISH,
            LANGUAGE_EUENGLISH,
#else
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_KRKOREAN,
#endif
        },
        {
            LANGUAGE_JPJAPANESE,
            LANGUAGE_USENGLISH,
            LANGUAGE_EUGERMAN,
            LANGUAGE_USFRENCH,
            LANGUAGE_USSPANISH,
            LANGUAGE_EUITALIAN,
            LANGUAGE_EUDUTCH,
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
            LANGUAGE_USENGLISH,
            LANGUAGE_USENGLISH,
            LANGUAGE_USENGLISH,
#else
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_KRKOREAN,
#endif
        },
        {
            LANGUAGE_JPJAPANESE,
            LANGUAGE_EUENGLISH,
            LANGUAGE_EUGERMAN,
            LANGUAGE_EUFRENCH,
            LANGUAGE_EUSPANISH,
            LANGUAGE_EUITALIAN,
            LANGUAGE_EUDUTCH,
#if (VERSION == RMGJ01 || VERSION == RMGE01 || VERSION == RMGP01)
            LANGUAGE_EUENGLISH,
            LANGUAGE_EUENGLISH,
            LANGUAGE_EUENGLISH,
#else
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_KRKOREAN,
#endif
        },
#if (VERSION == RMGK01)
        {
            LANGUAGE_JPJAPANESE,
            LANGUAGE_USENGLISH,
            LANGUAGE_EUGERMAN,
            LANGUAGE_EUFRENCH,
            LANGUAGE_EUSPANISH,
            LANGUAGE_EUITALIAN,
            LANGUAGE_EUDUTCH,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_KRKOREAN,
        },
        {
            LANGUAGE_JPJAPANESE,
            LANGUAGE_USENGLISH,
            LANGUAGE_EUGERMAN,
            LANGUAGE_EUFRENCH,
            LANGUAGE_EUSPANISH,
            LANGUAGE_EUITALIAN,
            LANGUAGE_EUDUTCH,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_KRKOREAN,
        },
        {
            LANGUAGE_JPJAPANESE,
            LANGUAGE_USENGLISH,
            LANGUAGE_EUGERMAN,
            LANGUAGE_EUFRENCH,
            LANGUAGE_EUSPANISH,
            LANGUAGE_EUITALIAN,
            LANGUAGE_EUDUTCH,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_CNSIMPCHINESE,
            LANGUAGE_KRKOREAN,
        },
#endif
    };
    const Language cLanguages[] = {
        {LANGUAGE_JPJAPANESE, "JpJapanese"},       {LANGUAGE_USENGLISH, "UsEnglish"},
        {LANGUAGE_USSPANISH, "UsSpanish"},         {LANGUAGE_USFRENCH, "UsFrench"},
        {LANGUAGE_EUENGLISH, "EuEnglish"},         {LANGUAGE_EUSPANISH, "EuSpanish"},
        {LANGUAGE_EUFRENCH, "EuFrench"},           {LANGUAGE_EUGERMAN, "EuGerman"},
        {LANGUAGE_EUITALIAN, "EuItalian"},         {LANGUAGE_EUDUTCH, "EuDutch"},
#if (VERSION == RMGK01)
        {LANGUAGE_CNSIMPCHINESE, "CnSimpChinese"}, {LANGUAGE_KRKOREAN, "KrKorean"},
#endif
    };
};  // namespace

namespace MR {
    u32 getDecidedLanguageFromIPL() {
        s32 language = SCGetLanguage();
        s32 i;

        if (language < 0) {
            i = 0;
        } else {
            s32 size = ARRAY_SIZE(::cSCLanguage2GameLanguageTable[SCLanguage2GameLanguageTableIndex]);

            if (language <= size) {
                i = language;
            } else {
                i = size;
            }
        }

        return ::cSCLanguage2GameLanguageTable[SCLanguage2GameLanguageTableIndex][i];
    }

    u32 getLanguage() {
        return SingletonHolder< GameSystem >::get()->mObjHolder->mLanguage;
    }

    u32 getLanguageFromIPL() {
        return getLanguage() & LANGUAGE_MASK;
    }

    const char* getCurrentLanguagePrefix() {
        u32 id = getLanguage();

        for (int i = 0; i < getLanguageNum(); i++) {
            if (id == ::cLanguages[i].mId) {
                return ::cLanguages[i].mName;
            }
        }

        return "English";
    }

    const char* getCurrentRegionPrefix() {
        char prefix[2];
        MR::extractString(prefix, getCurrentLanguagePrefix(), sizeof(prefix), 3);

        if (MR::isEqualString(prefix, "Jp")) {
            return "Jp";
        }

        if (MR::isEqualString(prefix, "Us")) {
            return "Us";
        }

        if (MR::isEqualString(prefix, "Eu")) {
            return "Eu";
        }
#if (VERSION == RMGK01)
        if (MR::isEqualString(prefix, "Cn")) {
            return "Cn";
        }

        if (MR::isEqualString(prefix, "Kr")) {
            return "Kr";
        }
#endif
        return nullptr;
    }

    u32 getLanguageNum() {
        return ARRAY_SIZE(::cLanguages);
    }

    const char* getLanguagePrefixByIndex(u32 index) {
        return ::cLanguages[index].mName;
    }
};  // namespace MR
