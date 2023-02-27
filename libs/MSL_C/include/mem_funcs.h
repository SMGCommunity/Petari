#ifndef MEM_FUNCS_H
#define MEM_FUNCS_H

#ifdef __cplusplus
extern "C" {
#endif

void __copy_longs_aligned(void *pDest, const void *pSrc, unsigned long len);
void __copy_longs_rev_aligned(void *pDest, const void *pSrc, unsigned long len);
void __copy_longs_unaligned(void *pDest, const void *pSrc, unsigned long len);
void __copy_longs_rev_unaligned(void *pDest, const void *pSrc, unsigned long len);

#ifdef __cplusplus
}
#endif

#endif // MEM_FUNCS_H