#ifndef STDIO_API_H
#define STDIO_API_H

#include "size_t.h"
#include "wchar_t.h"

enum __ReadProcActions {
	__GetAChar,
	__UngetAChar,
	__TestForError
};

typedef struct{
	char* NextChar;
	int NullCharDetected;
} __InStrCtrl;

typedef struct {
	wchar_t * wCharStr;
	size_t MaxCharCount;
	size_t CharsWritten;
} __wOutStrCtrl;

typedef struct {
	wchar_t * wNextChar;
	int    wNullCharDetected;
} __wInStrCtrl;

int __StringRead(void *, int, int);

#endif // STDIO_API_H