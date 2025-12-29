#include "bta_dm_int.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/dm/bta_dm_main.c
 */

/******************************************************************************
 *
 *  Copyright (C) 2003-2012 Broadcom Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/

/* Includes changes by muff1n1634 */

/*******************************************************************************
 * headers
 */

#include "bt_types.h" // BT_HDR
#include "data_types.h"

#include "bta_sys.h"

/*******************************************************************************
 * macros
 */

#define BTA_DM_NUM_ACTIONS				2
#define BTA_DM_NUM_COLS					3 // ERRATUM? bta_dm_sm_execute only goes over each with BTA_DM_NUM_ACTIONS

#define BTA_DM_IGNORE					(BTA_DM_MAX_EVENT - BTA_SYS_EVT_START(BTA_ID_DM))

#define BTA_DM_SEARCH_NUM_ACTIONS		2
#define BTA_DM_SEARCH_NUM_COLS			3

#define BTA_DM_SEARCH_IGNORE			BTA_DM_SEARCH_MAX_ACTION

#define BTA_DM_SEARCH_NEXT_STATE_INDEX	2

/*******************************************************************************
 * types
 */

/* typedef UINT8 tBTA_DM_SEARCH_ACTION_TYPE; */
enum
{
	BTA_DM_API_SEARCH,
	BTA_DM_API_SEARCH_CANCEL,
	BTA_DM_API_DISCOVER,
	BTA_DM_INQUIRY_CMPL,
	BTA_DM_REMT_NAME,
	BTA_DM_SDP_RESULT,
	BTA_DM_SEARCH_CMPL,
	BTA_DM_FREE_SDP_DB,
	BTA_DM_DISC_RESULT,
	BTA_DM_SEARCH_RESULT,
	BTA_DM_QUEUE_SEARCH,
	BTA_DM_QUEUE_DISC,
	BTA_DM_SEARCH_CLEAR_QUEUE,
	BTA_DM_SEARCH_CANCEL_CMPL,
	BTA_DM_SEARCH_CANCEL_NOTIFY,
	BTA_DM_SEARCH_CANCEL_TRANSAC_CMPL,
	BTA_DM_DISC_RMT_NAME,
	BTA_DM_API_DI_DISCOVER,

	BTA_DM_SEARCH_MAX_ACTION
};

// NOTE: state table typedefs are not arrays to match dwarf

typedef void tBTA_DM_ACTION(tBTA_DM_MSG *p_data);
typedef UINT8 tBTA_DM_STATE_TABLE_ENTRY[BTA_DM_NUM_COLS];
#if 0
typedef tBTA_DM_STATE_TABLE_ENTRY tBTA_DM_STATE_TABLE[];
#else
typedef tBTA_DM_STATE_TABLE_ENTRY tBTA_DM_STATE_TABLE;
#endif

typedef void tBTA_DM_SEARCH_ACTION(tBTA_DM_MSG *p_data);
typedef UINT8 tBTA_DM_SEARCH_STATE_TABLE_ENTRY[BTA_DM_SEARCH_NUM_COLS];
#if 0
typedef tBTA_DM_SEARCH_STATE_TABLE_ENTRY tBTA_DM_SEARCH_STATE_TABLE[];
#else
typedef tBTA_DM_SEARCH_STATE_TABLE_ENTRY tBTA_DM_SEARCH_STATE_TABLE;
#endif

/*******************************************************************************
 * variables
 */

// .rodata
tBTA_DM_ACTION * const bta_dm_action[] =
{
	&bta_dm_enable,
	&bta_dm_disable,
	&bta_dm_set_dev_name,
	&bta_dm_set_visibility,
	&bta_dm_bond,
	&bta_dm_pin_reply,
	&bta_dm_auth_reply,
	&bta_dm_signal_strength,
	&bta_dm_acl_change,
	&bta_dm_pm_btm_status,
	&bta_dm_pm_timer,
	&bta_dm_keep_acl,
	&bta_dm_send_hci_reset,
};

// clang-format off
tBTA_DM_STATE_TABLE_ENTRY const bta_dm_st_table[] =
{
	{BTA_DM_API_ENABLE_EVT         - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_DISABLE_EVT        - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_SET_NAME_EVT       - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_SET_VISIBILITY_EVT - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
#if 0
	// not over here?
	{BTA_DM_API_BOND_EVT           - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
#endif
	{BTA_DM_API_PIN_REPLY_EVT      - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_AUTH_REPLY_EVT     - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_SIG_STRENGTH_EVT   - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_ACL_CHANGE_EVT         - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
#if 1
	// over here?
	{BTA_DM_API_BOND_EVT           - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
#endif
	{BTA_DM_PM_BTM_STATUS_EVT      - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_PM_TIMER_EVT           - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_KEEP_ACL_LINKS     - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
	{BTA_DM_API_RESET_HCI          - BTA_SYS_EVT_START(BTA_ID_DM), BTA_DM_IGNORE},
};
// clang-format on

tBTA_DM_SEARCH_ACTION * const bta_dm_search_action[] =
{
	&bta_dm_search_start,
	&bta_dm_search_cancel,
	&bta_dm_discover,
	&bta_dm_inq_cmpl,
	&bta_dm_rmt_name,
	&bta_dm_sdp_result,
	&bta_dm_search_cmpl,
	&bta_dm_free_sdp_db,
	&bta_dm_disc_result,
	&bta_dm_search_result,
	&bta_dm_queue_search,
	&bta_dm_queue_disc,
	&bta_dm_search_clear_queue,
	&bta_dm_search_cancel_cmpl,
	&bta_dm_search_cancel_notify,
	&bta_dm_search_cancel_transac_cmpl,
	&bta_dm_disc_rmt_name,
	&bta_dm_cancel_rmt_name,
};

// clang-format off
tBTA_DM_SEARCH_STATE_TABLE_ENTRY const bta_dm_search_idle_st_table[] =
{
	{BTA_DM_API_SEARCH,                 BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_SEARCH_CANCEL_NOTIFY,       BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_API_DISCOVER,               BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_FREE_SDP_DB,                BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
};

tBTA_DM_SEARCH_STATE_TABLE_ENTRY const bta_dm_search_search_active_st_table[] =
{
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_API_SEARCH_CANCEL,          BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_CANCELLING},
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_INQUIRY_CMPL,               BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_REMT_NAME,                  BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_API_DI_DISCOVER,            BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_SDP_RESULT,                 BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
	{BTA_DM_SEARCH_CMPL,                BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_RESULT,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_ACTIVE    },
};

tBTA_DM_SEARCH_STATE_TABLE_ENTRY const bta_dm_search_search_cancelling_st_table[] =
{
	{BTA_DM_QUEUE_SEARCH,               BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_CANCELLING},
	{BTA_DM_SEARCH_CLEAR_QUEUE,         BTA_DM_SEARCH_CANCEL_NOTIFY, BTA_DM_SEARCH_CANCELLING},
	{BTA_DM_QUEUE_DISC,                 BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_CANCELLING},
	{BTA_DM_SEARCH_CANCEL_CMPL,         BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_CANCEL_TRANSAC_CMPL, BTA_DM_SEARCH_CANCEL_CMPL,   BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_CANCELLING},
	{BTA_DM_SEARCH_CANCEL_TRANSAC_CMPL, BTA_DM_SEARCH_CANCEL_CMPL,   BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_CANCEL_CMPL,         BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_SEARCH_CANCEL_CMPL,         BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
};

tBTA_DM_SEARCH_STATE_TABLE_ENTRY const bta_dm_search_disc_active_st_table[] =
{
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_SEARCH_CANCEL_NOTIFY,       BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_DISC_RMT_NAME,              BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_API_DI_DISCOVER,            BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_SDP_RESULT,                 BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
	{BTA_DM_SEARCH_IGNORE,              BTA_DM_SEARCH_IGNORE,        BTA_DM_SEARCH_IDLE      },
	{BTA_DM_DISC_RESULT,                BTA_DM_SEARCH_IGNORE,        BTA_DM_DISCOVER_ACTIVE  },
};
// clang-format on

tBTA_DM_SEARCH_STATE_TABLE const * const bta_dm_search_st_tbl[] =
{
	bta_dm_search_idle_st_table,
	bta_dm_search_search_active_st_table,
	bta_dm_search_search_cancelling_st_table,
	bta_dm_search_disc_active_st_table
};

// .bss
// Makes more sense in this order.
tBTA_DM_CB bta_dm_cb;
tBTA_DM_SEARCH_CB bta_dm_search_cb;

/*******************************************************************************
 * functions
 */

BOOLEAN bta_dm_sm_execute(BT_HDR *p_msg)
{
	tBTA_DM_STATE_TABLE const *state_table = bta_dm_st_table;
	UINT8 action;
	int i;

	for (i = 0; i < BTA_DM_NUM_ACTIONS; ++i)
	{
		if ((action = state_table[p_msg->event & 0xff][i]) == BTA_DM_IGNORE)
			break;

		(*bta_dm_action[action])((tBTA_DM_MSG *)p_msg);
	}

	return TRUE;
}

BOOLEAN bta_dm_search_sm_execute(BT_HDR *p_msg)
{
	tBTA_DM_SEARCH_STATE_TABLE const *state_table =
		bta_dm_search_st_tbl[bta_dm_search_cb.state];
	UINT8 action;
	int i;

	bta_dm_search_cb.state =
		state_table[p_msg->event & 0xff][BTA_DM_SEARCH_NEXT_STATE_INDEX];

	for (i = 0; i < BTA_DM_SEARCH_NUM_ACTIONS; ++i)
	{
		if ((action = state_table[p_msg->event & 0xff][i])
		    == BTA_DM_SEARCH_IGNORE)
		{
			break;
		}

		(*bta_dm_search_action[action])((tBTA_DM_MSG *)p_msg);
	}

	return TRUE;
}
