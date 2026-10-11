#ifndef __INTERNAL_SIZE_T_H__
#define __INTERNAL_SIZE_T_H__

// 3DS port: MWCC (PPC) uses unsigned long; 32-bit ARM compilers expect unsigned int.
#ifdef __MWERKS__
typedef unsigned long int size_t;
#else
typedef __SIZE_TYPE__ size_t;
#endif

#endif // __INTERNAL_SIZE_T_H__
