#include "bta_ss_int.h"

/*******************************************************************************
 * headers
 */

#include "bta_ss_api.h"

/*******************************************************************************
 * variables
 */

// .sdata2
tBTA_SS_CFG const bta_ss_cfg =
{
	0x00, 0x00, 0x00, 0xc8
};

// .sdata
tBTA_SS_CFG *p_bta_ss_cfg = (tBTA_SS_CFG *)&bta_ss_cfg;

// .rodata, .data, .sdata
char const * const bta_ss_dev_info_prop[][2] =
{
	{"MANU", "Widcomm"},
	{"MOD", "0001"},
	{"OEM", "Widcomm"},
	{"FW-VERSION", "1.0"},
	{"FW-DATE", "20030925T000000"},
	{"SW-VERSION", "1.0"},
	{"SW-DATE", "20030925T000000"},
	{"IRMC-VERSION", "1.1"},
	{"HW-VERSION", "1.0"},
	{"HW-DATE", "20030925T000000"},
	{"SN", "SN02"},
	{"PB-TYPE-TX", "VCARD2.1"},
	{"PB-TYPE-RX", "VCARD2.1"},
	{"CAL-TYPE-TX", "VCAL1.0"},
	{"CAL-TYPE-RX", "VCAL1.0"},
	{"MSG-TYPE-TX", "VMSG1.1"},
	{"MSG-TYPE-RX", "VMSG1.1"},
	{"NOTE-TYPE-TX", "VNOTE1.1"},
	{"NOTE-TYPE-RX", "VNOTE1.1"},
	{"INBOX", "SINGLE"},
	{"MSG-SENT-BOX", "NO"},
	{"", ""}
};

// ...I believe this is compatible.
char const ***p_bta_ss_dev_info_prop = (char const ***)bta_ss_dev_info_prop;

char const * const bta_ss_pb_info_prop[][2] =
{
	{"Total-Records", "0"},
	{"Last-Used-Index", "0"},
	{"Maximum-Records", "*"},
	{"IEL", "8"},
	{"HD", "NO"},
	{"SAT", "CC"},
	{"SAI", "NO"},
	{"SAU", "YES"},
	{"DID", "3a"},
	{"ICL", "NO"},
	{"OCL", "NO"},
	{"MCL", "NO"},
	{"X-IRMC-FIELDS", ""},
	{"<Begin>", ""},
	{"VERSION", ""},
	{"N", "=18"},
	{"FN", ""},
	{"EMAIL", ""},
	{"TITLE", ""},
	{"ORG", ""},
	{"NOTE", ""},
	{"ADR", ""},
	{"TEL;TYPE=HOME;WORK;FAX", ""},
	{"X-IRMC-LUID", ""},
	{"<End>", ""},
	{"", ""}
};

char const ***p_bta_ss_pb_info_prop = (char const ***)bta_ss_pb_info_prop;

char const * const bta_ss_cal_info_prop[][2] =
{
	{"Total-Records", "0"},
	{"Last-Used-Index", "0"},
	{"Maximum-Records", "*"},
	{"IEL", "8"},
	{"HD", "NO"},
	{"SAT", "CC"},
	{"SAI", "NO"},
	{"SAU", "YES"},
	{"DID", "3a"},
	{"X-IRMC-FIELDS", ""},
	{"<Begin>", ""},
	{"VERSION", ""},
	{"CATEGORIES", "MEETING;PHONE CALL;MISCELLANEOUS;"},
	{"COMPLETED", ""},
	{"DESCRIPTION", "=100"},
	{"DTSTART", ""},
	{"PRIORITY", ""},
	{"STATUS", ""},
	{"DTEND", ""},
	{"DUE", ""},
	{"SUMMARY", ""},
	{"LOCATION", ""},
	{"X-IRMC-LUID", ""},
	{"<End>", ""},
	{"", ""}
};

char const ***p_bta_ss_cal_info_prop = (char const ***)bta_ss_cal_info_prop;

char const * const bta_ss_note_info_prop[][2] =
{
	{"Total-Records", "0"},
	{"Last-Used-Index", "0"},
	{"Maximum-Records", "*"},
	{"IEL", "8"},
	{"HD", "NO"},
	{"SAT", "CC"},
	{"SAI", "NO"},
	{"SAU", "YES"},
	{"DID", "3a"},
	{"X-IRMC-FIELDS", ""},
	{"<Begin>", ""},
	{"VERSION", ""},
	{"BODY", "=200"},
	{"SUMMARY", "=20"},
	{"X-IRMC-LUID", ""},
	{"<End>", ""},
	{"", ""}
};

char const ***p_bta_ss_note_info_prop = (char const ***)bta_ss_note_info_prop;

char const * const bta_ss_msg_info_prop[][2] =
{
	{"Total-Records", "0"},
	{"Last-Used-Index", "0"},
	{"Maximum-Records", "*"},
	{"IEL", "8"},
	{"HD", "NO"},
	{"SAT", "CC"},
	{"SAI", "NO"},
	{"SAU", "YES"},
	{"DID", "3a"},
	{"MMHL", "NO"},
	{"X-IRMC-FIELDS", ""},
	{"<Begin>", ""},
	{"VERSION", ""},
	{"Subject", "=20"},
	{"From", ""},
	{"To", ""},
	{"Date", ""},
	{"VBODY", "=400"},
	{"Content-Type", ""},
	{"Content-Transfer-Encoding", ""},
	{"<End>", ""},
	{"", ""}
};

char const ***p_bta_ss_msg_info_prop = (char const ***)bta_ss_msg_info_prop;
