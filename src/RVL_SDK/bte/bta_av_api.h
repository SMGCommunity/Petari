#ifndef BTE_BTA_AV_API_H
#define BTE_BTA_AV_API_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/include/bta_av_api.h
 */

/******************************************************************************
 *
 *  Copyright (C) 2004-2012 Broadcom Corporation
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
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

// TODO
typedef struct
{
	char unsigned	at_0x00[0x18];	// size 0x18, offset 0x00
	void			*at_0x18;		// size 0x04, offset 0x18
	void			*at_0x1c;		// size 0x04, offset 0x1c
} tBTA_AV_CFG; // size 0x20

#ifdef __cplusplus
	}
#endif

#endif // BTE_BTA_AV_API_H
