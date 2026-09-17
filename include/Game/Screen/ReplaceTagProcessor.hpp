#pragma once

#include <cstdarg>
#include <revolution/types.h>

class MessageEditorMessageTag;

namespace ReplaceTagProcessor {
    u32 Replace(wchar_t* pDst, const wchar_t* pSrc);
    u32 ReplaceArgs(wchar_t* pDst, const wchar_t* pSrc, va_list args);
    u32 exePictureGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag);
    u32 exeStringGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag, va_list args);
    u32 exeNumberGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag, va_list args);
    u32 exeLocalizeGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag);
    u32 exeLocalizeGroupPlayerName(wchar_t* pDst, u8 form);
    u32 exeRaceTimeGroup(wchar_t* pDst, const MessageEditorMessageTag& rTag);
}  // namespace ReplaceTagProcessor

namespace ReplaceTagFunction {
    u32 ReplaceArgs(wchar_t* pDst, s32 length, const wchar_t* pFormat, ...);
}
