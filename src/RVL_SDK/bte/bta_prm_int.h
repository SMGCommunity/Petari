#ifndef BTE_BTA_PRM_INT_H
#define BTE_BTA_PRM_INT_H

/*******************************************************************************
 * headers
 */

#include "bt_types.h"
#include "data_types.h"

#include "bta_prm_api.h"
#include "bta_sys.h"

/*******************************************************************************
 * macros
 */

#define BTA_PRM_MAX_PATCH_SIZE	250

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	BTA_PRM_API_ENABLE_EVT = BTA_SYS_EVT_START(BTA_ID_PRM),
	BTA_PRM_INT_OPEN_EVT,
	BTA_PRM_INT_READ_EVT,
	BTA_PRM_INT_WRITE_EVT,
	BTA_PRM_INT_CLOSE_EVT,
	BTA_PRM_INT_5_EVT,
};

typedef UINT8 tBTA_PRM_STATE;
enum
{
	BTA_PRM_STATE_1	= 1,
	BTA_PRM_STATE_2	= 2,
	BTA_PRM_STATE_3	= 3,
};

typedef UINT16 tBTA_PRM_STATUS;
enum
{
	BTA_PRM_STATUS_0	= 0,
	BTA_PRM_STATUS_1	= 1,
	BTA_PRM_STATUS_4	= 4,
};

typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	tBTA_PRM_CBACK	*p_cback;	// size 0x04, offset 0x08
	char			*p_name;	// size 0x04, offset 0x0c
	UINT8			fs_app_id;	// size 0x01, offset 0x10
	/* 3 bytes padding */
} tBTA_PRM_API_ENABLE; // size 0x14

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	UINT32	file_size;	// size 0x01, offset 0x08
	int		fd;			// size 0x04, offset 0x0c
	UINT16	status;		// size 0x02, offset 0x10
	/* 2 bytes padding */
} tBTA_PRM_OPEN_EVT; // size 0x14

typedef struct
{
	BT_HDR	hdr;		// size 0x08, offset 0x00
	int		fd;			// size 0x04, offset 0x08
	UINT16	num_read;	// size 0x02, offset 0x0c
	UINT16	status;		// size 0x02, offset 0x0e
} tBTA_PRM_READ_EVT; // size 0x10

/* TODO: did they reuse tBTA_PRM_API_ENABLE for start_evt by duplicating the
 * type or by typedefing it?
 */
typedef struct
{
	BT_HDR			hdr;		// size 0x08, offset 0x00
	tBTA_PRM_CBACK	*p_cback;	// size 0x04, offset 0x08
	char			*p_name;	// size 0x04, offset 0x0c
	UINT8			fs_app_id;	// size 0x01, offset 0x10
	/* 3 bytes padding */
} tBTA_PRM_START_EVT; // size 0x14

typedef union
{
	BT_HDR				hdr;		// size 0x08
	tBTA_PRM_OPEN_EVT	open_evt;	// size 0x14
	tBTA_PRM_READ_EVT	read_evt;	// size 0x10
	tBTA_PRM_API_ENABLE	start_evt;	// size 0x14
} tBTA_PRM_DATA; // size 0x14

typedef struct
{
	tBTA_PRM_CBACK	*p_cback;							// size 0x004, offset 0x000
	UINT32			file_length;						// size 0x004, offset 0x004
	UINT32			read_length;						// size 0x004, offset 0x008
	int				fd;									// size 0x004, offset 0x00c
	UINT8			patch_data[BTA_PRM_MAX_PATCH_SIZE];	// size 0x0fa, offset 0x010
	tBTA_PRM_STATE	state;								// size 0x001, offset 0x10a
	UINT8			app_id;								// size 0x001, offset 0x10b
	BOOLEAN			internal_patch;						// size 0x001, offset 0x10c
	/* 3 bytes padding */
} tBTA_PRM_CB; // size 0x110

/*******************************************************************************
 * external globals
 */

extern tBTA_PRM_CB bta_prm_cb;

/*******************************************************************************
 * functions
 */

BOOLEAN bta_prm_hdl_event(BT_HDR *p_msg);

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_PRM_INT_H
