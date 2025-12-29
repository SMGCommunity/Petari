#ifndef DATA_TYPES_H
#define DATA_TYPES_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * gki/linux/data_types.h
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

#include <stdint.h>

/*******************************************************************************
 * macros
 */

#define BCM_STRNCPY_S(x1, x2, x3, x4)	strncpy(x1, x3, x4)

/*******************************************************************************
 * types
 */

#ifdef __cplusplus
	extern "C" {
#endif

typedef int_least8_t INT8;
typedef int_least16_t INT16;
typedef int_least32_t INT32;

typedef uint_least8_t UINT8;
typedef uint_least16_t UINT16;
typedef uint_least32_t UINT32;

typedef unsigned char BOOLEAN;

#ifndef TRUE
# define TRUE	1
#endif

#ifndef FALSE
# define FALSE	0
#endif

#ifdef __cplusplus
	}
#endif

#endif // DATA_TYPES_H
