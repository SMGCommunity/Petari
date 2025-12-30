#ifndef BCM2045_H
#define BCM2045_H

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef void BCM2045_PrmCallback(unsigned char);

/*******************************************************************************
 * functions
 */

/* WARNING: None of these functions have definitions. */
extern int BCM2045_PrmInit(BCM2045_PrmCallback *, unsigned char);
extern int BCM2045_PrmLaunchRam(void);
extern int BCM2045_PrmLoadData(unsigned char *, unsigned short);

#ifdef __cplusplus
	}
#endif

#endif // BCM2045_H
