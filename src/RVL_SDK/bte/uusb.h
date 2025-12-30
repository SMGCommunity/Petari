#ifndef BTE_UUSB_H
#define BTE_UUSB_H

/*******************************************************************************
 * headers
 */

#include <decomp.h>

#include "data_types.h"

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef enum
{
	UUSB_OP_NONE,
} tUUSB_IOCTL_OP;

typedef void tUUSB_CBACK(UINT8, INT8);

typedef struct
{
	char	pad0_[4];
	UINT16	at_0x04;
	UINT16	at_0x06;
	UINT16	at_0x08;
	char	pad1_[1];
	char	at_0x0b;
	char	pad2_[1]; // for stack
} tUUSB;

/*******************************************************************************
 * external globals
 */

extern UINT32 volatile wait4hci;

/*******************************************************************************
 * functions
 */

void uusb_set_trace_state(UINT8 state);
UINT8 uusb_get_trace_state(void);

void UUSB_Register(tUUSB *uusb);
void UUSB_Open(tUUSB *uusb, tUUSB_CBACK *cback);
UINT16 UUSB_Read(UINT8, void *, UINT16);
void UUSB_ReadBuf(/* ... */);
unk_t UUSB_WriteBuf(/* ... */);
UINT16 UUSB_Write(UINT8, void *p_data, UINT16 len, void *);
void UUSB_Ioctl(tUUSB_IOCTL_OP op, void *p_data);
void UUSB_Close(void);
void UUSB_Unregister(void);
unk_t UUSB_Feature(void);

#ifdef __cplusplus
	}
#endif

#endif // BTE_UUSB_H
