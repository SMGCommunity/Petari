#pragma once

#include <revolution/types.h>

namespace ReplaceTagProcessor {
    u32 Replace(wchar_t*, const wchar_t*);
};  // namespace ReplaceTagProcessor

namespace ReplaceTagFunction {
    u32 ReplaceArgs(wchar_t*, s32, const wchar_t*, ...);
};  // namespace ReplaceTagFunction
