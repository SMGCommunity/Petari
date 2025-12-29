#ifndef BTE_BTA_FT_API_H
#define BTE_BTA_FT_API_H

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	void			*at_0x00;	// size 0x04, offset 0x00
	char unsigned	at_0x04[4];	// size 0x08, offset 0x04
	char			*at_0x08;	// size 0x04, offset 0x08
	char unsigned	at_0x0c[8]; // size 0x08, offset 0x0c
} tBTA_FT_CFG; // size 0x14

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_FT_API_H
