#ifndef BTE_HCI_MSGS_H
#define BTE_HCI_MSGS_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/hcimsgs.h
 */

/******************************************************************************
 *
 *  Copyright (C) 1999-2012 Broadcom Corporation
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

#include "data_types.h"
#include "bt_types.h"

/*******************************************************************************
 * macros
 */

#define HCIC_PARAM_SIZE_INQUIRY					5
#define HCIC_PARAM_SIZE_INQ_CANCEL				0
#define HCIC_PARAM_SIZE_PER_INQ_MODE			9
#define HCIC_PARAM_SIZE_EXIT_PER_INQ			0
#define HCIC_PARAM_SIZE_CREATE_CONN				13
#define HCIC_PARAM_SIZE_DISCONNECT				3
#define HCIC_PARAM_SIZE_ADD_SCO_CONN			4
#define HCIC_PARAM_SIZE_CREATE_CONN_CANCEL		6
#define HCIC_PARAM_SIZE_ACCEPT_CONN				7
#define HCIC_PARAM_SIZE_REJECT_CONN				7
#define HCIC_PARAM_SIZE_LINK_KEY_REQ_REPLY		22
#define HCIC_PARAM_SIZE_LINK_KEY_NEG_REPLY		6
#define HCIC_PARAM_SIZE_PIN_CODE_REQ_REPLY		23
#define HCIC_PARAM_SIZE_PIN_CODE_NEG_REPLY		6
#define HCIC_PARAM_SIZE_CHANGE_CONN_TYPE		4
#define HCIC_PARAM_SIZE_CMD_HANDLE				2
#define HCIC_PARAM_SIZE_SET_CONN_ENCRYPT		3
#define HCIC_PARAM_SIZE_MASTER_LINK_KEY			1
#define HCIC_PARAM_SIZE_RMT_NAME_REQ			10
#define HCIC_PARAM_SIZE_RMT_NAME_REQ_CANCEL		6
#define HCIC_PARAM_SIZE_RMT_EXT_FEATURES		3
#define HCIC_PARAM_SIZE_LOCAL_EXT_FEATURES		1
#define HCIC_PARAM_SIZE_SETUP_ESCO				17
#define HCIC_PARAM_SIZE_ACCEPT_ESCO				21
#define HCIC_PARAM_SIZE_REJECT_ESCO				7
#define HCIC_PARAM_SIZE_HOLD_MODE				6
#define HCIC_PARAM_SIZE_SNIFF_MODE				10
#define HCIC_PARAM_SIZE_PARK_MODE				6
#define HCIC_PARAM_SIZE_QOS_SETUP				20
#define HCIC_PARAM_SIZE_SWITCH_ROLE				7
#define HCIC_PARAM_SIZE_WRITE_POLICY_SET		4
#define HCIC_PARAM_SIZE_READ_DEF_POLICY_SET		0
#define HCIC_PARAM_SIZE_WRITE_DEF_POLICY_SET	2
#define HCIC_PARAM_SIZE_FLOW_SPEC				21
#define HCIC_PARAM_SIZE_SNIFF_SUB_RATE			8
#define HCIC_PARAM_SIZE_EXT_INQ_RESP			241
#define HCIC_PARAM_SIZE_SET_EVENT_MASK			8
#define HCIC_PARAM_SIZE_RESET					0
#define HCIC_PARAM_SIZE_NEW_UNIT_KEY			0
#define HCIC_PARAM_SIZE_READ_STORED_KEY			7
#define HCIC_PARAM_SIZE_DELETE_STORED_KEY		7
#define HCIC_PARAM_SIZE_CHANGE_NAME				BD_NAME_LEN
#define HCIC_PARAM_SIZE_READ_CMD				0
#define HCIC_PARAM_SIZE_WRITE_PARAM1			1
#define HCIC_PARAM_SIZE_WRITE_PARAM2			2
#define HCIC_PARAM_SIZE_WRITE_PARAM3			3
#define HCIC_PARAM_SIZE_SET_AFH_CHANNELS		10
#define HCIC_PARAM_SIZE_WRITE_PAGESCAN_CFG		4
#define HCIC_PARAM_SIZE_WRITE_INQSCAN_CFG		4
#define HCIC_PARAM_SIZE_WRITE_AUTO_FLUSH_TOUT	4
#define HCIC_PARAM_SIZE_READ_TX_POWER			3
#define HCIC_PARAM_SIZE_SET_HOST_BUF_SIZE		7
#define HCIC_PARAM_SIZE_WRITE_LINK_SUPER_TOUT	4
#define HCIC_PARAM_SIZE_READ_CLOCK				3

#define HCI_MAX_NUM_OF_LINK_KEYS_PER_CMD		11

#define HCI_DATA_EVENT_MASK						0x03
#define HCI_DATA_EVENT_OFFSET					12
#define HCID_GET_EVENT(u16)						(UINT8)(((u16) >> HCI_DATA_EVENT_OFFSET) & HCI_DATA_EVENT_MASK)

#define HCI_DATA_HANDLE_MASK					0x0fff
#define HCID_GET_HANDLE(u16)					(UINT16)((u16) & HCI_DATA_HANDLE_MASK)

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

enum
{
	HCI_HOST_FLOW_CTRL_ACL_ON	= 1,
};

/*******************************************************************************
 * functions
 */

BOOLEAN btsnd_hcic_inquiry(LAP const inq_lap, UINT8 duration,
                           UINT8 response_cnt);
BOOLEAN btsnd_hcic_inq_cancel(void);
BOOLEAN btsnd_hcic_per_inq_mode(UINT16 max_period, UINT16 min_period,
                                LAP const inq_lap, UINT8 duration,
                                UINT8 response_cnt);
BOOLEAN btsnd_hcic_exit_per_inq(void);
BOOLEAN btsnd_hcic_create_conn(BD_ADDR dest, UINT16 packet_types,
                               UINT8 page_scan_rep_mode, UINT8 page_scan_mode,
                               UINT16 clock_offset, UINT8 allow_switch);
BOOLEAN btsnd_hcic_disconnect(UINT16 handle, UINT8 reason);
BOOLEAN btsnd_hcic_add_SCO_conn(UINT16 handle, UINT16 packet_types);
BOOLEAN btsnd_hcic_create_conn_cancel(BD_ADDR dest);
void btsnd_hcic_accept_conn(void *buffer, BD_ADDR dest, UINT8 role);
void btsnd_hcic_reject_conn(void *buffer, BD_ADDR dest, UINT8 reason);
BOOLEAN btsnd_hcic_link_key_req_reply(BD_ADDR bd_addr, LINK_KEY link_key);
BOOLEAN btsnd_hcic_link_key_neg_reply(BD_ADDR bd_addr);
BOOLEAN btsnd_hcic_pin_code_req_reply(BD_ADDR bd_addr, UINT8 pin_code_len,
                                      PIN_CODE pin_code);
BOOLEAN btsnd_hcic_pin_code_neg_reply(BD_ADDR bd_addr);
BOOLEAN btsnd_hcic_change_conn_type(UINT16 handle, UINT16 packet_types);
BOOLEAN btsnd_hcic_auth_request(UINT16 handle);
BOOLEAN btsnd_hcic_set_conn_encrypt(UINT16 handle, BOOLEAN enable);
BOOLEAN btsnd_hcic_change_link_key(UINT16 handle);
BOOLEAN btsnd_hcic_master_link_key(BOOLEAN key_flag);
BOOLEAN btsnd_hcic_rmt_name_req(BD_ADDR bd_addr, UINT8 page_scan_rep_mode,
                                UINT8 page_scan_mode, UINT16 clock_offset);
BOOLEAN btsnd_hcic_rmt_name_req_cancel(BD_ADDR bd_addr);
BOOLEAN btsnd_hcic_rmt_features_req(UINT16 handle);
void btsnd_hcic_rmt_ext_features(void *buffer, UINT16 handle, UINT8 page_num);
BOOLEAN btsnd_hcic_rmt_ver_req(UINT16 handle);
BOOLEAN btsnd_hcic_read_rmt_clk_offset(UINT16 handle);
void btsnd_hcic_read_lmp_handle(void *buffer, UINT16 handle);
BOOLEAN btsnd_hcic_setup_esco_conn(UINT16 handle, UINT32 tx_bw, UINT32 rx_bw,
                                   UINT16 max_latency, UINT16 voice,
                                   UINT8 retrans_effort, UINT16 packet_types);
void btsnd_hcic_accept_esco_conn(void *buffer, BD_ADDR bd_addr, UINT32 tx_bw,
                                 UINT32 rx_bw, UINT16 max_latency,
                                 UINT16 content_fmt, UINT8 retrans_effort,
                                 UINT16 packet_types);
void btsnd_hcic_reject_esco_conn(void *buffer, BD_ADDR bd_addr, UINT8 reason);
BOOLEAN btsnd_hcic_hold_mode(void *buffer, UINT16 handle,
                             UINT16 max_hold_period, UINT16 min_hold_period);
BOOLEAN btsnd_hcic_sniff_mode(void *buffer, UINT16 handle,
                              UINT16 max_sniff_period, UINT16 min_sniff_period,
                              UINT16 sniff_attempt, UINT16 sniff_timeout);
BOOLEAN btsnd_hcic_exit_sniff_mode(void *buffer, UINT16 handle);
BOOLEAN btsnd_hcic_park_mode(void *buffer, UINT16 handle,
                             UINT16 beacon_max_interval,
                             UINT16 beacon_min_interval);
BOOLEAN btsnd_hcic_exit_park_mode(void *buffer, UINT16 handle);
BOOLEAN btsnd_hcic_qos_setup(UINT16 handle, UINT8 flags, UINT8 service_type,
                             UINT32 token_rate, UINT32 peak, UINT32 latency,
                             UINT32 delay_var);
void btsnd_hcic_role_discovery(void *buffer, UINT16 handle);
BOOLEAN btsnd_hcic_switch_role(BD_ADDR bd_addr, UINT8 role);
BOOLEAN btsnd_hcic_read_policy_set(UINT16 handle);
BOOLEAN btsnd_hcic_write_policy_set(UINT16 handle, UINT16 settings);
void btsnd_hcic_read_def_policy_set(void *buffer);
void btsnd_hcic_write_def_policy_set(void *buffer, UINT16 settings);
void btsnd_hcic_flow_specification(void *buffer, UINT16 handle, UINT8 flags,
                                   UINT8 flow_direct, UINT8 service_type,
                                   UINT32 token_rate, UINT32 token_bucket_size,
                                   UINT32 peak, UINT32 latency);
void btsnd_hcic_sniff_sub_rate(void *buffer, UINT16 handle, UINT16 max_lat,
                               UINT16 min_remote_lat, UINT16 min_local_lat);
void btsnd_hcic_set_event_mask(void *buffer, BT_EVENT_MASK event_mask);
BOOLEAN btsnd_hcic_reset(void);
void btsnd_hcic_set_event_filter(void *buffer, UINT8 filt_type,
                                 UINT8 filt_cond_type, UINT8 *filt_cond,
                                 UINT8 filt_cond_len);
void btsnd_hcic_flush(void *buffer, UINT16 handle);
void btsnd_hcic_read_pin_type(void *buffer);
BOOLEAN btsnd_hcic_write_pin_type(UINT8 type);
void btsnd_hcic_new_unit_key(void *buffer);
void btsnd_hcic_read_stored_key(void *buffer, BD_ADDR bd_addr,
                                BOOLEAN read_all_flag);
void btsnd_hcic_write_stored_key(void *buffer, UINT8 num_keys, BD_ADDR *bd_addr,
                                 LINK_KEY *link_key);
BOOLEAN btsnd_hcic_delete_stored_key(BD_ADDR bd_addr, BOOLEAN delete_all_flag);
void btsnd_hcic_change_name(void *buffer, BD_NAME name);
void btsnd_hcic_read_name(void *buffer);
void btsnd_hcic_read_conn_acc_tout(void *buffer);
void btsnd_hcic_write_conn_acc_tout(void *buffer, UINT16 timeout);
void btsnd_hcic_read_page_tout(void *buffer);
void btsnd_hcic_write_page_tout(void *buffer, UINT16 timeout);
void btsnd_hcic_read_scan_enable(void *buffer);
void btsnd_hcic_write_scan_enable(void *buffer, UINT8 flag);
void btsnd_hcic_read_pagescan_cfg(void *buffer);
void btsnd_hcic_write_pagescan_cfg(void *buffer, UINT16 interval,
                                   UINT16 window);
void btsnd_hcic_read_inqscan_cfg(void *buffer);
void btsnd_hcic_write_inqscan_cfg(void *buffer, UINT16 interval, UINT16 window);
void btsnd_hcic_read_auth_enable(void *buffer);
BOOLEAN btsnd_hcic_write_auth_enable(UINT8 flag);
void btsnd_hcic_read_encr_mode(void *buffer);
BOOLEAN btsnd_hcic_write_encr_mode(UINT8 mode);
void btsnd_hcic_read_dev_class(void *buffer);
void btsnd_hcic_write_dev_class(void *buffer, DEV_CLASS dev_class);
void btsnd_hcic_read_voice_settings(void *buffer);
void btsnd_hcic_write_voice_settings(void *buffer, UINT16 flags);
void btsnd_hcic_read_auto_flush_tout(void *buffer, UINT16 handle);
void btsnd_hcic_write_auto_flush_tout(void *buffer, UINT16 handle, UINT16 tout);
void btsnd_hcic_read_num_bcast_xmit(void *buffer);
void btsnd_hcic_write_num_bcast_xmit(void *buffer, UINT8 num);
void btsnd_hcic_read_hold_mode_act(void *buffer);
void btsnd_hcic_write_hold_mode_act(void *buffer, UINT8 flags);
void btsnd_hcic_read_tx_power(void *buffer, UINT16 handle, UINT8 type);
void btsnd_hcic_read_sco_flow_enable(void *buffer);
void btsnd_hcic_write_sco_flow_enable(void *buffer, UINT8 flag);
BOOLEAN btsnd_hcic_set_host_flow_ctrl(UINT8 value);
BOOLEAN btsnd_hcic_set_host_buf_size(UINT16 acl_len, UINT8 sco_len,
                                     UINT16 acl_num, UINT16 sco_num);
void btsnd_hcic_host_num_xmitted_pkts(void *buffer, UINT8 num_handles,
                                      UINT16 *handle, UINT16 *num_pkts);
void btsnd_hcic_read_link_super_tout(void *buffer, UINT16 handle);
BOOLEAN btsnd_hcic_write_link_super_tout(UINT16 handle, UINT16 timeout);
void btsnd_hcic_read_max_iac(void *buffer);
void btsnd_hcic_read_cur_iac_lap(void *buffer);
void btsnd_hcic_write_cur_iac_lap(void *buffer, UINT8 num_cur_iac,
                                  LAP * const iac_lap);
void btsnd_hcic_read_page_scan_per(void *buffer);
void btsnd_hcic_write_page_scan_per(void *buffer, UINT8 mode);
void btsnd_hcic_read_page_scan_mode(void *buffer);
void btsnd_hcic_write_page_scan_mode(void *buffer, UINT8 mode);
void btsnd_hcic_read_ext_inquiry_response(void *buffer);
void btsnd_hcic_write_ext_inquiry_response(void *buffer, UINT8 fec_req,
                                           UINT8 *p_data);
BOOLEAN btsnd_hcic_read_local_ver(void);
void btsnd_hcic_read_local_supported_cmds(void *buffer);
BOOLEAN btsnd_hcic_read_local_features(void);
void btsnd_hcic_read_local_ext_features(void *buffer, UINT8 page_num);
void btsnd_hcic_read_buffer_size(void *buffer);
void btsnd_hcic_read_country_code(void *buffer);
BOOLEAN btsnd_hcic_read_bd_addr(void);
void btsnd_hcic_read_fail_conn_count(void *buffer, UINT16 handle);
void btsnd_hcic_reset_fail_conn_count(void *buffer, UINT16 handle);
BOOLEAN btsnd_hcic_get_link_quality(UINT16 handle);
BOOLEAN btsnd_hcic_read_rssi(UINT16 handle);
void btsnd_hcic_read_loopback_mode(void *buffer);
void btsnd_hcic_write_loopback_mode(void *buffer, UINT8 mode);
void btsnd_hcic_enable_test_mode(void *buffer);
BOOLEAN btsnd_hcic_write_afh_channel_assessment_mode(UINT8 mode);
void btsnd_hcic_read_afh_channel_assessment_mode(void *buffer);
BOOLEAN btsnd_hcic_set_afh_channels(UINT8 first, UINT8 last);
void btsnd_hcic_set_afh_host_channel_class(void *buffer,
                                           UINT8 *p_afhchannelmap);
void btsnd_hcic_read_afh_channel_map(void *buffer, UINT16 handle);
void btsnd_hcic_read_clock(void *buffer, UINT16 handle, UINT8 which_clock);
void btsnd_hcic_read_inqscan_type(void *buffer);
void btsnd_hcic_write_inqscan_type(void *buffer, UINT8 type);
void btsnd_hcic_read_inquiry_mode(void *buffer);
void btsnd_hcic_write_inquiry_mode(void *buffer, UINT8 mode);
void btsnd_hcic_read_pagescan_type(void *buffer);
void btsnd_hcic_write_pagescan_type(void *buffer, UINT8 type);
void btsnd_hcic_vendor_spec_cmd(void *buffer, UINT16 opcode, UINT8 len,
                                UINT8 *p_data);
void btsnd_hcic_data(BT_HDR *p_buf, UINT16 len, UINT16 handle, UINT8 boundary,
                     UINT8 broadcast);
void btsnd_hcic_nop(void *buffer);

#ifdef __cplusplus
	}
#endif

#endif // BTE_HCI_MSGS_H
