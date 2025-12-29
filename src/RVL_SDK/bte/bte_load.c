#include "bte.h"

/*******************************************************************************
 * headers
 */

#include <stddef.h> // NULL

#include "data_types.h" // UINT32

#include "gki.h"

/*******************************************************************************
 * types
 */

// This is probably somewhat accurate
typedef struct bt_load
{
	struct bt_load	*next;	// size 0x04, offset 0x00
	UINT32			size;	// size 0x04, offset 0x04
} tBT_LOAD; // size 0x08

/*******************************************************************************
 * variables
 */

// .sbss
tBT_LOAD bt_load_tbl[] =
{
	{NULL, 0},
};

/*******************************************************************************
 * functions
 */

void BTE_LoadStack(void)
{
	tBT_LOAD *load = bt_load_tbl;

	for (load = bt_load_tbl; load->next; ++load)
		load->next->next = GKI_os_malloc(load->size);
}

void BTE_UnloadStack(void)
{
	tBT_LOAD *load = bt_load_tbl;

	for (load = bt_load_tbl; load->next; ++load)
	{
		if (load->next->next)
		{
			GKI_os_free(load->next->next);
			load->next->next = NULL;
		}
	}
}
