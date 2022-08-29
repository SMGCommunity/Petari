#ifndef VA_LIST_H
#define VA_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char gpr;
    char fpr;
    char _2;
    char _3;
    char* _4;
    char* _8;
} __va_list[1];

typedef __va_list va_list;

#ifdef __cplusplus
}
#endif

#endif // VA_LIST_H