#pragma once

#ifndef ARRAY_LEN
#define ARRAY_LEN(o) (sizeof(o) / sizeof((o)[0]))
#endif

#ifndef ARRAY_END
#define ARRAY_END(o) ((o) + ARRAY_LEN(o))
#endif

#ifndef GET_REGISTER
#ifdef __MWERKS__
#define GET_REGISTER(var, reg) __asm { mr var, reg }
#else
#define GET_REGISTER(var, reg)
#endif
#endif

#ifndef DEBUG_LINE
#define DEBUG_LINE OSReport("%s:%d\n", __FILE__, __LINE__);
#endif

#ifndef DEBUG_LINE_MSG
#define DEBUG_LINE_MSG(msg) OSReport("%s:%d %s\n", __FILE__, __LINE__, msg);
#endif