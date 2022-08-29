#ifndef __cstdarg__
#define __cstdarg__

#ifdef __cplusplus
extern "C" {
#endif

#include <va_list.h>

extern void* __va_arg(void*, int);

#define __va_start(ap,fmt) ((void)fmt,__builtin_va_info(&ap))
#define __va_arg(ap,t)	(*((t*)__va_arg(ap,_var_arg_typeof(t))))
#define va_start __va_start
#define va_arg   __va_arg
#define va_end   __va_end

#define __va_end(ap) ((void)0)

#ifdef __cplusplus
}
#endif

#endif // __cstdarg