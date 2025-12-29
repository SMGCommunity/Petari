#ifndef BTE_MISC_H
#define BTE_MISC_H

// Declarations that I haven't figured out where to put yet

/*******************************************************************************
 * headers
 */

#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

/*******************************************************************************
 * functions
 */

// bte_main.c

#if !defined(NDEBUG)
void __btu_interrupt_handler_debug_msg(BOOLEAN enable);
#endif

void bta_usb_close_evt(INT8);

// uusb_ppc.c

void HCI_TRACE(char const *p_msg);
void HCI_TRACE1(char const *p_msg, UINT32 arg1);
void __ntd_set_ohci(UINT8 ohci);
void __ntd_set_pid_vid(int pid, int vid);
int __ntd_get_usb_file_descriptor(void);
UINT32 __ntd_get_allocated_mem_size(void);

#ifdef __cplusplus
	}
#endif

#endif // BTE_MISC_H
