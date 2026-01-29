#ifndef BTE_BTA_BI_API_H
#define BTE_BTA_BI_API_H

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef struct
{
	char unsigned	at_0x00[0x18];	// size 0x18, offset 0x00
	void			*at_0x18;		// size 0x04, offset 0x18
	void			*at_0x1c;		// size 0x04, offset 0x1c
	char unsigned	at_0x20[0x10];	// size 0x10, offset 0x20
} tBTA_BI_IMAGING_CAPABILITIES; // size 0x30

typedef struct
{
	void	*at_0x00;	// size 0x04, offset 0x00
	void	*at_0x04;	// size 0x04, offset 0x04
	void	*at_0x08;	// size 0x04, offset 0x08
	void	*at_0x0c;	// size 0x04, offset 0x0c
} tBTA_BI_CFG; // size 0x10

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_BI_API_H
