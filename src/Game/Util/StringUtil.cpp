#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cctype>
// #include <cstdarg>
#include <cstdio>

#define CENTISEC_PER_SEC 100
#define SEC_PER_MIN 60
#define MIN_PER_HOUR 60
#define FRAME_PER_SEC 60
#define FRAME_PER_MIN (FRAME_PER_SEC * SEC_PER_MIN)
#define FRAME_PER_HOUR (FRAME_PER_SEC * SEC_PER_MIN * MIN_PER_HOUR)

#ifdef __cplusplus
extern "C" {
    int strcasecmp(const char*, const char*);
    // extern int vswprintf(wchar_t*, size_t, const wchar_t*, va_list);
    int wcsncpy(wchar_t*, const wchar_t*, size_t);
};
#endif

namespace MR {
    struct Tag {
        u16 _0;
        u8 mDataSize;
        u8 _3;
        wchar_t mBuffer[1];
    };

    void addFilePrefix(char* pDst, u32 size, const char* pChild, const char* pParent) {
        if (*pChild == '/') {
            pChild++;
        }

        snprintf(pDst, size, "%s/%s", pParent, pChild);
    }

    void makeDateString(wchar_t* pDst, s32 size, s32 year, s32 month, s32 day) {
        const wchar_t* pMessage = MR::getGameMessageDirect("System_Date000");

        ReplaceTagFunction::ReplaceArgs(pDst, size, pMessage, year, month, day);
    }

    void makeTimeString(wchar_t* pDst, s32 size, s32 hour, s32 minute) {
        const wchar_t* pMessage = MR::getGameMessageDirect("System_Time002");

        ReplaceTagFunction::ReplaceArgs(pDst, size, pMessage, hour, minute);
    }

    void makeClearTimeString(wchar_t* pDst, u32 frame) {
        s32 minute;
        s32 second;
        s32 centisecond;

        if (frame >= FRAME_PER_HOUR) {
            minute = 59;
            second = 59;
            centisecond = 99;
        }
        else {
            s32 signedFrame = frame;

            minute = signedFrame / FRAME_PER_MIN;
            second = (signedFrame / FRAME_PER_SEC) % SEC_PER_MIN;
            centisecond = ((signedFrame % FRAME_PER_SEC) * CENTISEC_PER_SEC) / FRAME_PER_SEC;
        }

        const wchar_t* pMessage = MR::getGameMessageDirect("System_Time000");

        ReplaceTagFunction::ReplaceArgs(pDst, 9, pMessage, 0, minute, second, centisecond);
    }

    void makeMinuteAndSecondString(wchar_t* pDst, u32 frame) {
        s32 minute;
        s32 second;

        if (frame >= FRAME_PER_HOUR) {
            minute = 59;
            second = 59;
        }
        else {
            minute = frame / FRAME_PER_MIN;
            second = (frame % FRAME_PER_MIN) / FRAME_PER_SEC;
        }

        const wchar_t* pMessage = MR::getGameMessageDirect("System_Time001");

        ReplaceTagFunction::ReplaceArgs(pDst, 16, pMessage, 0, minute, second);
    }

    wchar_t* addNumberFontTag(wchar_t* pDst, int tag) {
        return MR::addNumberFontTag(pDst, L"%d", tag);
    }

    char* removeExtensionString(char* pDst, u32 size, const char* pPath) {
        snprintf(pDst, size, "%s", pPath);

        char* pExtSeparator = strrchr(pDst, '.');
        char* pDirSeparator = strrchr(pDst, '/');

        if (pExtSeparator < pDirSeparator || pDirSeparator + 1 == pExtSeparator) {
            return pDirSeparator;
        }

        *pExtSeparator = '\0';

        return pDirSeparator;
    }

    void makeRaceBestTimeString(wchar_t* pDst, int raceId) {
        MR::makeClearTimeString(pDst, getRaceBestTime(raceId));
    }

    void makeRaceCurrentTimeString(wchar_t* pDst) {
        MR::makeClearTimeString(pDst, getRaceCurrentTime());
    }

    void copyString(char* pDst, const char* pSrc, u32 num) {
        strncpy(pDst, pSrc, num);
    }

    void copyString(wchar_t* pDst, const wchar_t* pSrc, u32 num) {
        wcsncpy(pDst, pSrc, num);
    }

    wchar_t* addPictureFontCode(wchar_t* pDst, int code) {
        pDst[0] = code;
        pDst[1] = '\0';

        return &pDst[1];
    }

    wchar_t* addPictureFontTag(wchar_t* pDst, int tag) {
        Tag* pTag = reinterpret_cast<Tag*>(pDst);

        pTag->_0 = 26;
        pTag->mDataSize = sizeof(Tag);
        pTag->_3 = 3;
        pTag->mBuffer[0] = tag - '0';
        pTag->mBuffer[1] = '\0';

        return &pTag->mBuffer[1];
    }

    wchar_t* addPictureFontTagPlayerIcon(wchar_t* pDst) {
        int tag = isPlayerLuigi() ? 76 : 66;

        return MR::addPictureFontTag(pDst, tag);
    }

    wchar_t* addNewLine(wchar_t* pDst) {
        pDst[0] = '\n';
        pDst[1] = '\0';

        return &pDst[1];
    }

#ifdef NON_MATCHING
    wchar_t* addNumberFontTag(wchar_t* pDst, const wchar_t* pFmt, ...) {
        va_list args;
        va_start(args, pFmt);

        Tag* pTag = reinterpret_cast<Tag*>(pDst);

        int num = vswprintf(pTag->mBuffer, 256, pFmt, args);

        pTag->_0 = 26;
        pTag->mDataSize = num * sizeof(wchar_t) + sizeof(Tag);
        pTag->_3 = 10;
        pTag->mBuffer[num] = '\0';

        return pDst + num;
    }
#endif

    // FIXME: Missing stack accesses.
    const char* getBasename(const char* pPath) {
        const char* pBasename = strrchr(pPath, '/');

        if (pBasename == nullptr) {
            return pBasename;
        }

        return pBasename + 1;
    }

    void extractString(char* pDst, const char* pSrc, u32 num, u32) {
        strncpy(pDst, pSrc, num);

        pDst[num] = '\0';
    }

    int convertUTF16ToASCII(char* pDst, const wchar_t* pSrc, s32 num) {
        u32 i;

        for (i = 0; i < num - 1; pDst++, pSrc++, i++) {
            const char* p = reinterpret_cast<const char*>(pSrc);

            if (p[0] != '\0') {
                break;
            }

            if (p[1] == '\0') {
                break;
            }

            *pDst = p[1];
        }

        *pDst = '\0';

        return i;
    }

    int strcasecmp(const char* pStr1, const char* pStr2) {
        return ::strcasecmp(pStr1, pStr2);
    }

    bool isEqualString(const char* pStr1, const char* pStr2) {
        return strcmp(pStr1, pStr2) == 0;
    }

    bool isEqualStringCase(const char* pStr1, const char* pStr2) {
        return strcasecmp(pStr1, pStr2) == 0;
    }

    bool isEqualSubString(const char* pStr, const char* pSubStr) {
        return strstr(pStr, pSubStr) != nullptr;
    }

    bool hasStringSpace(const char* pStr) {
        return strchr(pStr, ' ') != nullptr;
    }

    bool isDigitStringTail(const char* pStr, int digitNum) {
        for (int i = 1; i <= digitNum; i++) {
            int ch = pStr[strlen(pStr) - i];

            if (isdigit(ch) == 0) {
                return false;
            }
        }

        return true;
    }

    bool isExistString(const char* pStr, const char* const* ppStr, u32 strNum) {
        for (int i = 0; i < strNum; i++) {
            if (MR::isEqualString(ppStr[i], pStr)) {
                return true;
            }
        }

        return false;
    }

    bool isNullOrEmptyString(const char* pStr) {
        return pStr == nullptr || MR::isEqualString(pStr, "");
    }

    bool isMessageEditorNextTag(const wchar_t* pStr) {
        const Tag* pTag = reinterpret_cast<const Tag*>(pStr);
        u8 v1 = pTag->_3;
        wchar_t v2 = pTag->mBuffer[0];

        return v1 == 1 && v2 == 1;
    }

    int getStringLengthWithMessageTag(const wchar_t* pMessage) {
        int length = 0;

        while (*pMessage != '\0') {
            if (*pMessage == 0x1A) {
                if (isMessageEditorNextTag(pMessage)) {
                    break;
                }

                u16 dataSize = reinterpret_cast<const Tag*>(pMessage)->mDataSize;

                // FIXME: r3-r4 used instead of r30-r31, and slwi used instead of clrrwi.
                pMessage = (pMessage + dataSize) - 1;
                length += (dataSize / sizeof(u16)) - 1;
            }

            pMessage++;
            length++;
        }

        return length;
    }

    void scan32(const char* pSrc, const char* pSubStr, s32* pDst) {
        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t%d", pDst);
    }

    void scan16(const char* pSrc, const char* pSubStr, u16* pDst) {
        int temp;

        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t%d", &temp);

        *pDst = temp;
    }

    void scan8(const char* pSrc, const char* pSubStr, u8* pDst) {
        int temp;

        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t%d", &temp);

        *pDst = temp;
    }

    void scanf32(const char* pSrc, const char* pSubStr, f32* pDst) {
        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t%ff", pDst);
    }

    void scanu8x4(const char* pSrc, const char* pSubStr, u8* pDst) {
        int temp[4];

        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t{%d,%d,%d,%d}", &temp[0], &temp[1], &temp[2], &temp[3]);

        for (int i = 0; i < sizeof(temp) / sizeof(*temp); i++) {
            pDst[i] = temp[i];
        }
    }

    void scans16x4(const char* pSrc, const char* pSubStr, s16* pDst) {
        int temp[4];

        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t{%d,%d,%d,%d}", &temp[0], &temp[1], &temp[2], &temp[3]);

        for (int i = 0; i < sizeof(temp) / sizeof(*temp); i++) {
            pDst[i] = temp[i];
        }
    }

    void scanf32x4(const char* pSrc, const char* pSubStr, f32* pDst) {
        if (strstr(pSrc, pSubStr) == nullptr) {
            return;
        }

        sscanf(pSrc, "\t{%ff,%ff,%ff,%ff}", &pDst[0], &pDst[1], &pDst[2], &pDst[3]);
    }
};
