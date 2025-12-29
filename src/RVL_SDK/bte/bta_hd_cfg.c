#include "bta_hd_int.h"

/*******************************************************************************
 * headers
 */

#include "data_types.h"

/*******************************************************************************
 * variables
 */

// .rodata

/* once again shout out to
 * https://eleccelerator.com/usbdescreqparser
 */
UINT8 const bta_hd_cfg_report[] =
{
	0x05, 0x01,			//	Usage Page (Generic Desktop Ctrls)
	0x09, 0x06,			//	Usage (Keyboard)

	0xa1, 0x01,			//	Collection (Application)

	0x05, 0x07,			//		Usage Page (Kbrd/Keypad)
	0x85, 0x01,			//		Report ID (1)
	0x19, 0xe0,			//		Usage Minimum (0xE0)
	0x29, 0xe7,			//		Usage Maximum (0xE7)
	0x15, 0x00,			//		Logical Minimum (0)
	0x25, 0x01,			//		Logical Maximum (1)
	0x75, 0x01,			//		Report Size (1)
	0x95, 0x08,			//		Report Count (8)
	0x81, 0x02,			//		Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x01,			//		Report Count (1)
	0x75, 0x08,			//		Report Size (8)
	0x81, 0x01,			//		Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x05,			//		Report Count (5)
	0x75, 0x01,			//		Report Size (1)

	0x05, 0x08,			//		Usage Page (LEDs)
	0x19, 0x01,			//		Usage Minimum (Num Lock)
	0x29, 0x05,			//		Usage Maximum (Kana)
	0x91, 0x02,			//		Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x95, 0x01,			//		Report Count (1)
	0x75, 0x03,			//		Report Size (3)
	0x91, 0x01,			//		Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
	0x95, 0x06,			//		Report Count (6)
	0x75, 0x08,			//		Report Size (8)
	0x15, 0x00,			//		Logical Minimum (0)
	0x26, 0xa4, 0x00,	//		Logical Maximum (164)

	0x05, 0x07,			//		Usage Page (Kbrd/Keypad)
	0x19, 0x00,			//		Usage Minimum (0x00)
	0x29, 0xa4,			//		Usage Maximum (0xA4)
	0x81, 0x00,			//		Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

	0xc0,				//	End Collection

	0x05, 0x01,			//	Usage Page (Generic Desktop Ctrls)
	0x09, 0x02,			//	Usage (Mouse)

	0xa1, 0x01,			//	Collection (Application)

	0x09, 0x01,			//		Usage (Pointer)

	0xa1, 0x00,			//		Collection (Physical)

	0x85, 0x02,			//			Report ID (2)
	0x05, 0x09,			//			Usage Page (Button)
	0x19, 0x01,			//			Usage Minimum (0x01)
	0x29, 0x03,			//			Usage Maximum (0x03)
	0x15, 0x00,			//			Logical Minimum (0)
	0x25, 0x01,			//			Logical Maximum (1)
	0x95, 0x03,			//			Report Count (3)
	0x75, 0x01,			//			Report Size (1)
	0x81, 0x02,			//			Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x95, 0x01,			//			Report Count (1)
	0x75, 0x05,			//			Report Size (5)
	0x81, 0x03,			//			Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
	0x05, 0x01,			//			Usage Page (Generic Desktop Ctrls)
	0x09, 0x30,			//			Usage (X)
	0x09, 0x31,			//			Usage (Y)
	0x09, 0x38,			//			Usage (Wheel)
	0x15, 0x81,			//			Logical Minimum (-127)
	0x25, 0x7f,			//			Logical Maximum (127)
	0x75, 0x08,			//			Report Size (8)
	0x95, 0x03,			//			Report Count (3)
	0x81, 0x06,			//			Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)

	0xc0,				//		End Collection

	0xc0,				//	End Collection

	0x05, 0x0c,			//	Usage Page (Consumer)
	0x09, 0x01,			//	Usage (Consumer Control)

	0xa1, 0x01,			//	Collection (Application)

	0x85, 0x03,			//		Report ID (3)
	0x75, 0x10,			//		Report Size (16)
	0x95, 0x02,			//		Report Count (2)
	0x15, 0x01,			//		Logical Minimum (1)
	0x26, 0x8c, 0x02,	//		Logical Maximum (652)
	0x19, 0x01,			//		Usage Minimum (Consumer Control)
	0x2a, 0x8c, 0x02,	//		Usage Maximum (AC Send)

	0x81, 0x60,			//		Input (Data,Array,Abs,No Wrap,Linear,No Preferred State,Null State)

	0xc0,				//	End Collection

	0x06, 0x00, 0xff,	//	Usage Page (Vendor Defined 0xFF00)
	0x09, 0x01,			//	Usage (0x01)

	0xa1, 0x01,			//	Collection (Application)

	0x85, 0x10,			//		Report ID (16)
	0x75, 0x08,			//		Report Size (8)
	0x95, 0x28,			//		Report Count (40)
	0x15, 0x00,			//		Logical Minimum (0)
	0x26, 0xff, 0x00,	//		Logical Maximum (255)

	0x09, 0x01,			//		Usage (0x01)
	0x81, 0x00,			//		Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)

	0x09, 0x01,			//		Usage (0x01)
	0x91, 0x00,			//		Output (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)

	0xc0,				//	End Collection
};

tBTA_HD_CFG const bta_hd_cfg =
{
	{
		{0, 1, 800, 8,   0, 0xffffffff, 0xffffffff},
		{0, 2, 300, 4, 300,      10000,      10000},
		{0, 2, 400, 8, 800,      10000,      10000},
	},
	{
		"BTA HID Device",
		"Remote Control", // Hello. I am remote control - Nintendo Wii, 2009
		"Broadcom Corp",
		0x100,
		0x111,
		0xc0,
		0x00,
		0x00,
		{sizeof bta_hd_cfg_report, (UINT8 *)bta_hd_cfg_report}
	},
	TRUE
};

// .sdata
tBTA_HD_CFG *p_bta_hd_cfg = (tBTA_HD_CFG *)&bta_hd_cfg;
