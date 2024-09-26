#ifndef MBSTRING_H
#define MBSTRING_H

#include "ansi_params.h"
#include "size_t.h"
#include "wchar_io.h"

int __mbtowc_noconv(wchar_t *, const char *, __std(size_t));
int __wctomb_noconv(char *, wchar_t);

#endif // MBSTRING_H