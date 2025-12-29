#ifndef BT_TRACE_H
#define BT_TRACE_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * include/bt_trace.h
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

/*******************************************************************************
 * Log functions
 */

#ifdef __cplusplus
	extern "C" {
#endif

// clang-format off

extern void LogMsg(UINT32 trace_set_mask, char const *fmt_str, ...);

extern void LogMsg_0(UINT32 trace_set_mask, char const *fmt_str);
extern void LogMsg_1(UINT32 trace_set_mask, char const *fmt_str, UINT32 p1);
extern void LogMsg_2(UINT32 trace_set_mask, char const *fmt_str, UINT32 p1, UINT32 p2);
extern void LogMsg_3(UINT32 trace_set_mask, char const *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3);
extern void LogMsg_4(UINT32 trace_set_mask, char const *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4);
extern void LogMsg_5(UINT32 trace_set_mask, char const *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5);
extern void LogMsg_6(UINT32 trace_set_mask, char const *fmt_str, UINT32 p1, UINT32 p2, UINT32 p3, UINT32 p4, UINT32 p5, UINT32 p6);

// clang-format on

/*******************************************************************************
 * main macro work
 */

// clang-format off

#define BDLT_VA_APPLY_(m, _6, _5, _4, _3, _2, _1, _0, x, ...)	m(x)
#define BDLT_VA_APPLY(...)	BDLT_VA_APPLY_(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0, _)

#define BDLT_LOG_MSG_(x)	LogMsg_ ## x
#define BDLT_LOG_MSG(...)	BDLT_VA_APPLY(BDLT_LOG_MSG_, __VA_ARGS__)

#define BDLT_CAST_ARGS_(x)	BDLT_CAST_ARGS_ ## x
#define BDLT_CAST_ARGS(...)	BDLT_VA_APPLY(BDLT_CAST_ARGS_, __VA_ARGS__)(__VA_ARGS__, _)

/* in an effort to make it O(1) over O(n) at the slight cost of some duplicated
 * macros (but a lot less than the original!)
 */
#define BDLT_CAST_ARGS_0(m, ...)							m
#define BDLT_CAST_ARGS_1(m, _1, ...)						m, (UINT32)(_1)
#define BDLT_CAST_ARGS_2(m, _1, _2, ...)					m, (UINT32)(_1), (UINT32)(_2)
#define BDLT_CAST_ARGS_3(m, _1, _2, _3, ...)				m, (UINT32)(_1), (UINT32)(_2), (UINT32)(_3)
#define BDLT_CAST_ARGS_4(m, _1, _2, _3, _4, ...)			m, (UINT32)(_1), (UINT32)(_2), (UINT32)(_3), (UINT32)(_4)
#define BDLT_CAST_ARGS_5(m, _1, _2, _3, _4, _5, ...)		m, (UINT32)(_1), (UINT32)(_2), (UINT32)(_3), (UINT32)(_4), (UINT32)(_5)
#define BDLT_CAST_ARGS_6(m, _1, _2, _3, _4, _5, _6, ...)	m, (UINT32)(_1), (UINT32)(_2), (UINT32)(_3), (UINT32)(_4), (UINT32)(_5), (UINT32)(_6)

#define BDLT_TRACE(ctrl_, layer_, org_, type_, ...)	\
	BDLT_LOG_MSG(__VA_ARGS__)									\
	(															\
		TRACE_MASK(ctrl_, layer_, org_, type_),					\
		BDLT_CAST_ARGS(__VA_ARGS__)								\
	)

#define BDLT_TRACE_COND(max_level_, ctrl_, layer_, origin_, type_, ...)	\
	do																	\
	{																	\
		if ((max_level_) >= BT_TRACE_LEVEL_ ## type_)					\
			BDLT_TRACE(ctrl_, layer_, origin_, type_, __VA_ARGS__);		\
	} while (FALSE)

// clang-format on

/*******************************************************************************
 * Trace info
 */

// clang-format off

typedef UINT8 tBT_TRACE_LEVEL;
enum
{
	BT_TRACE_LEVEL_NONE			= 0,	/* No trace messages to be generated  */
	BT_TRACE_LEVEL_ERROR		= 1,	/* Error condition trace messages     */
	BT_TRACE_LEVEL_WARNING		= 2,	/* Warning condition trace messages   */
	BT_TRACE_LEVEL_API			= 3,	/* API traces                         */
	BT_TRACE_LEVEL_EVENT		= 4,	/* Debug messages for events          */
	BT_TRACE_LEVEL_DEBUG		= 5,	/* Full debug messages                */

	BT_MAX_TRACE_LEVEL			= BT_TRACE_LEVEL_DEBUG,
	BT_READ_TRACE_LEVEL			= 0xff,
};

// TRACE_CTRL						0xcc......
#define TRACE_CTRL(x)				(((UINT32)(x) << 24) & 0xff000000)
#define TRACE_GET_CTRL(x)			(((UINT32)(x) & 0xff000000) >> 24)

#define TRACE_CTRL_GENERAL			0

#define TRACE_CTRL_MAX_NUM			3

// TRACE_LAYER						0x..ll....
#define TRACE_LAYER(x)				(((UINT32)(x) << 16) & 0x00ff0000)
#define TRACE_GET_LAYER(x)			(((UINT32)(x) & 0x00ff0000) >> 16)

#define TRACE_LAYER_NONE			0
#define TRACE_LAYER_HCI				7
#define TRACE_LAYER_L2CAP			8
#define TRACE_LAYER_RFCOMM			9
#define TRACE_LAYER_SDP				10
#define TRACE_LAYER_BTM				13
#define TRACE_LAYER_GAP				14
#define TRACE_LAYER_GOEP			16
#define TRACE_LAYER_XML				16 // ?
#define TRACE_LAYER_HID				30

#define TRACE_LAYER_MAX_NUM			49

// TRACE_ORIGIN						0x....oo..
#define TRACE_ORIGIN(x)				(((UINT32)(x) << 8) & 0x0000ff00)
#define TRACE_GET_ORIGIN(x)			(((UINT32)(x) & 0x0000ff00) >> 8)

#define TRACE_ORIGIN_STACK			0
#define TRACE_ORIGIN_APPL			5

#define TRACE_ORIGIN_MAX_NUM		10

// TRACE_TYPE						0x......tt
#define TRACE_GET_TYPE(x)			((((UINT32)(x)) & 0x000000ff) >> 0)
#define TRACE_TYPE(x)				((((UINT32)(x)) << 0) & 0x000000ff)

#define TRACE_TYPE_ERROR			0
#define TRACE_TYPE_WARNING			1
#define TRACE_TYPE_API				2
#define TRACE_TYPE_EVENT			3
#define TRACE_TYPE_DEBUG			4

#define TRACE_TYPE_MAX_NUM			20

#define TRACE_MASK(ctrl_, layer_, origin_, type_)	\
	( TRACE_CTRL  (TRACE_CTRL_   ## ctrl_  )		\
	| TRACE_LAYER (TRACE_LAYER_  ## layer_ )		\
	| TRACE_ORIGIN(TRACE_ORIGIN_ ## origin_)		\
	| TRACE_TYPE  (TRACE_TYPE_   ## type_  ))

// clang-format on

/*******************************************************************************
 * Trace macros
 */

// clang-format off

#define   APPL_TRACE(        type_, ...)	BDLT_TRACE_COND(         appl_trace_level, GENERAL,   NONE,  APPL, type_, __VA_ARGS__)
#define     BT_TRACE(layer_, type_, ...)	BDLT_TRACE     (                           GENERAL, layer_, STACK, type_, __VA_ARGS__)
#define    BTM_TRACE(        type_, ...)	BDLT_TRACE_COND( btm_cb.      trace_level, GENERAL,    BTM, STACK, type_, __VA_ARGS__)
#define    GAP_TRACE(        type_, ...)	BDLT_TRACE_COND( gap_cb.      trace_level, GENERAL,    GAP, STACK, type_, __VA_ARGS__)
#define   GOEP_TRACE(        type_, ...)	BDLT_TRACE_COND(goep_cb.      trace_level, GENERAL,   GOEP, STACK, type_, __VA_ARGS__)
#define   HIDD_TRACE(        type_, ...)	BDLT_TRACE_COND(  hd_cb.      trace_level, GENERAL,    HID, STACK, type_, __VA_ARGS__)
#define   HIDH_TRACE(        type_, ...)	BDLT_TRACE_COND(  hh_cb.      trace_level, GENERAL,    HID, STACK, type_, __VA_ARGS__)
#define  L2CAP_TRACE(        type_, ...)	BDLT_TRACE_COND(   l2cb.l2cap_trace_level, GENERAL,  L2CAP, STACK, type_, __VA_ARGS__)
#define RFCOMM_TRACE(        type_, ...)	BDLT_TRACE_COND( rfc_cb.      trace_level, GENERAL, RFCOMM, STACK, type_, __VA_ARGS__)
#define    SDP_TRACE(        type_, ...)	BDLT_TRACE_COND( sdp_cb.      trace_level, GENERAL,    SDP, STACK, type_, __VA_ARGS__)
#define    XML_TRACE(        type_, ...)	BDLT_TRACE     (                           GENERAL,    XML, STACK, type_, __VA_ARGS__)

// clang-format on

/*******************************************************************************
 * Default logging levels
 */

#define APPL_INITIAL_TRACE_LEVEL	BT_TRACE_LEVEL_NONE

#ifndef APPL_INITIAL_TRACE_LEVEL
# define APPL_INITIAL_TRACE_LEVEL	BT_TRACE_LEVEL_WARNING
#endif

#ifdef __cplusplus
	}
#endif

#endif // BT_TRACE_H
