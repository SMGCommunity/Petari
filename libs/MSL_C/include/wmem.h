#ifndef WMEM_H
#define WMEM_H

#include "wchar_t.h"
#include "size_t.h"

wchar_t* wmemcpy(wchar_t *, const wchar_t *, size_t);
wchar_t* wmemchr(const wchar_t *, wchar_t, size_t );

#endif // WMEM_H