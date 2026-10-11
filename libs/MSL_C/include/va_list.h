#ifndef VA_LIST_H
#define VA_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __MWERKS__
typedef struct __va_list_struct {
    char gpr;
    char fpr;
    char reserved[2];
    char* input_arg_area;
    char* reg_save_area;
} __va_list[1];

typedef __va_list va_list;
#else
// 3DS port: the PPC EABI va_list struct is meaningless on ARM; use the compiler's.
typedef __builtin_va_list va_list;
typedef va_list __va_list;
#endif

#ifdef __cplusplus
}
#endif

#endif // VA_LIST_H