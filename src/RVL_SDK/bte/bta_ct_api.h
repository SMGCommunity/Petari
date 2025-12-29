#ifndef BTE_BTA_CT_API_H
#define BTE_BTA_CT_API_H

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	void			*at_0x00;	// size 0x04, offset 0x00
	void			*at_0x04;	// size 0x04, offset 0x04
	char unsigned	at_0x08[4];	// size 0x04, offset 0x08
} tBTA_CT_CFG; // size 0x0c

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_CT_API_H
