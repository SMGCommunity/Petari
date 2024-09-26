#ifndef OSFASTCAST_H
#define OSFASTCAST_H

#ifdef __cplusplus
extern "C" {
#endif

#define OSf32tou16(in,out)  asm volatile ("psq_st   %1, 0(%0), 1, 3 " : : "b" (out) , "f" (*(in)) : "memory")
#define OSu16tof32(in,out)  asm volatile ("psq_l   %0, 0(%1), 1, 3  " : "=f" (*(out)) : "b" (in) )

#ifdef __cplusplus
}
#endif

#endif // OSFASTCAST_H