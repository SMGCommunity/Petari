#ifndef STDARG_H
#define STDARG_H

#ifdef __MWERKS__
// extern void* __va_arg(void*, int);
#define __va_start(ap, fmt) ((void)fmt, __builtin_va_info(&ap))
#define __va_arg(ap, t) (*((t*)__va_arg(ap, _var_arg_typeof(t))))
#define va_start __va_start
#define va_arg __va_arg
#define va_end __va_end

#define __va_end(ap) ((void)0)
#else
// 3DS port: compiler built-ins (PPC EABI va_info does not exist on ARM).
#define va_start(ap, fmt) __builtin_va_start(ap, fmt)
#define va_arg(ap, t) __builtin_va_arg(ap, t)
#define va_end(ap) __builtin_va_end(ap)
#define va_copy(d, s) __builtin_va_copy(d, s)
#endif
#endif  // STDARG_H
