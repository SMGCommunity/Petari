#ifndef STDIO_API_H
#define STDIO_API_H

enum __ReadProcActions {
	__GetAChar,
	__UngetAChar,
	__TestForError
};

typedef struct{
	char* NextChar;
	int NullCharDetected;
} __InStrCtrl;

int __StringRead(void *, int, int);

#endif // STDIO_API_H