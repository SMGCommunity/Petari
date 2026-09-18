#include "Game/Screen/ReplaceTagProcessor.hpp"
#include "Game/Screen/MessageTagSkipTagProcessor.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>
extern "C" int swprintf(wchar_t*, size_t, const wchar_t*, ...);

namespace ReplaceTagProcessor {
    namespace Impl {
        struct GroupFunctionInfo {
            u8 mGroup;
            u32 (*mFunction)(wchar_t*, const MessageEditorMessageTag&);
        };

        const GroupFunctionInfo sGroupFunctionTable[] = {{5, exeLocalizeGroup}, {9, exeRaceTimeGroup}, {3, exePictureGroup}, {0, nullptr}};

        const GroupFunctionInfo* findGroupFunctionInfo(int group) {
            for (const GroupFunctionInfo* pInfo = sGroupFunctionTable; pInfo->mFunction != nullptr; pInfo++) {
                if (pInfo->mGroup == group) {
                    return pInfo;
                }
            }

            return nullptr;
        }
    }  // namespace Impl

    namespace ImplArgs {
        struct GroupFunctionInfo {
            u8 mGroup;
            u32 (*mFunction)(wchar_t*, const MessageEditorMessageTag&, va_list);
        };

        const GroupFunctionInfo sGroupFunctionTable[] = {{7, exeStringGroup}, {6, exeNumberGroup}, {0, nullptr}};

        const GroupFunctionInfo* findGroupFunctionInfo(int group) {
            for (const GroupFunctionInfo* pInfo = sGroupFunctionTable; pInfo->mFunction != nullptr; pInfo++) {
                if (pInfo->mGroup == group) {
                    return pInfo;
                }
            }

            return nullptr;
        }
    }  // namespace ImplArgs

    u32 Replace(wchar_t* pDst, const wchar_t* pSrc) {
        wchar_t* pStart = pDst;
        while (*pSrc != 0) {
            if (MR::isMessageEditorNextTag(pSrc)) {
                *pDst = 0;
                return pDst - pStart;
            }

            if (*pSrc == 0x1A) {
                MessageEditorMessageTag tag(pSrc + 1);
                const Impl::GroupFunctionInfo* pInfo = Impl::findGroupFunctionInfo(tag.getGroup());
                if (pInfo != nullptr) {
                    pDst += pInfo->mFunction(pDst, tag);
                } else {
                    MR::copyMemory(pDst, pSrc, (tag.getSkipLength() + 1) * sizeof(wchar_t));
                    pDst += tag.getSkipLength() + 1;
                }

                pSrc += tag.getSkipLength() + 1;
            } else {
                *pDst = *pSrc;
                pDst++;
                pSrc++;
            }
        }

        *pDst = *pSrc;
        return pDst - pStart;
    }

    u32 ReplaceArgs(wchar_t* pDst, const wchar_t* pSrc, va_list args) {
        wchar_t* pStart = pDst;
        while (*pSrc != 0) {
            if (MR::isMessageEditorNextTag(pSrc)) {
                *pDst = 0;
                return pDst - pStart;
            }

            if (*pSrc == 0x1A) {
                MessageEditorMessageTag tag(pSrc + 1);
                const ImplArgs::GroupFunctionInfo* pInfo = ImplArgs::findGroupFunctionInfo(tag.getGroup());
                if (pInfo != nullptr) {
                    pDst += pInfo->mFunction(pDst, tag, args);
                } else {
                    MR::copyMemory(pDst, pSrc, (tag.getSkipLength() + 1) * sizeof(wchar_t));
                    pDst += tag.getSkipLength() + 1;
                }

                pSrc += tag.getSkipLength() + 1;
            } else {
                *pDst = *pSrc;
                pDst++;
                pSrc++;
            }
        }

        *pDst = *pSrc;
        return pDst - pStart;
    }

    u32 exePictureGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag) {
        struct PictureTag {
            u8 mLength;
            u8 mGroup;
            u16 mTag;
        } tag;
        tag.mLength = rTag.getTagLength() + 2;
        tag.mGroup = rTag.getGroup();
        tag.mTag = rTag.getTag();
        if (static_cast< wchar_t >(rTag.getTag() + 0x30) == 0x5B) {
            if (MR::isPlayerLuigi()) {
                tag.mTag = 0x1C;
            } else {
                tag.mTag = 0x12;
            }
        }

        *pDst++ = 0x1A;
        MR::copyMemory(pDst, &tag, rTag.getTagLength());
        return rTag.getTagLength() / 2 + 1;
    }

    u32 exeStringGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag, va_list args) {
        va_list copy;
        const wchar_t* pString = nullptr;
        *copy = *args;
        for (u32 i = 0; i <= rTag.getParam32(1); i++) {
            pString = va_arg(copy, const wchar_t*);
        }

        return swprintf(pDst, 0x100, L"%ls", pString);
    }

    u32 exeNumberGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag, va_list args) {
        va_list copy;
        int number = 0;
        *copy = *args;
        for (u32 i = 0; i <= rTag.getParam32(1); i++) {
            number = va_arg(copy, int);
        }

        switch (rTag.getTag()) {
        case 5:
            return swprintf(pDst, 0x100, L"%02d", number);
        case 6:
            return swprintf(pDst, 0x100, L"%03d", number);
        case 7:
            return swprintf(pDst, 0x100, L"%04d", number);
        case 8:
            return swprintf(pDst, 0x100, L"%05d", number);
        case 9:
            return swprintf(pDst, 0x100, L"%06d", number);
        default:
            return swprintf(pDst, 0x100, L"%d", number);
        }
    }

    u32 exeLocalizeGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag) {
        switch (rTag.getTag()) {
        case 0:
            return exeLocalizeGroupPlayerName(pDst, rTag.getParam8(0));
        }

        return 0;
    }

    u32 exeLocalizeGroupPlayerName(wchar_t* pDst, u8 form) {
        char name[0x100];
        if (MR::isPlayerLuigi()) {
            snprintf(name, sizeof(name), "System_PlayerName1%02d", form);
        } else {
            snprintf(name, sizeof(name), "System_PlayerName0%02d", form);
        }

        const wchar_t* pMessage = MR::getGameMessageDirect(name);
        int length = MR::getStringLengthWithMessageTag(pMessage);
        MR::copyString(pDst, pMessage, length + 1);
        return length;
    }

    u32 exeRaceTimeGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag) {
        if (rTag.getTag() == 5) {
            MR::makeRaceCurrentTimeString(pDst);
        } else {
            MR::makeRaceBestTimeString(pDst, rTag.getTag());
        }

        return 8;
    }
}  // namespace ReplaceTagProcessor

u32 ReplaceTagFunction::ReplaceArgs(wchar_t* pDst, s32 length, const wchar_t* pFormat, ...) {
    wchar_t buffer[0x200];
    va_list args;
    va_start(args, pFormat);
    s32 result = ReplaceTagProcessor::ReplaceArgs(buffer, pFormat, args);
    if (result >= length) {
        buffer[length - 1] = 0;
        length = result;
    }

    MR::copyString(pDst, buffer, length);
    va_end(args);
    return length;
}
