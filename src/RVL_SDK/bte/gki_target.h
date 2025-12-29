#ifndef BTE_GKI_TARGET_H
#define BTE_GKI_TARGET_H

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * include/gki_target.h
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
 * Configuration options, sorted as seen in bluedroid's gki_target.h
 */

/* Tasks */

// possibly? but then this ID conflicts with PPC_TASK
#define BTU_TASK					2

#ifndef BTU_TASK
# define BTU_TASK					0
#endif

#ifndef PPC_TASK
# define PPC_TASK					2
#endif

#define GKI_MAX_TASKS				8

#ifndef GKI_MAX_TASKS
# define GKI_MAX_TASKS				3
#endif

/* Timers */

#ifndef TICKS_PER_SEC
# define TICKS_PER_SEC				100
#endif

#ifndef GKI_MS_TO_TICKS
# define GKI_MS_TO_TICKS(x)			((x) / (1000 / TICKS_PER_SEC))
#endif

#ifndef GKI_SECS_TO_TICKS
# define GKI_SECS_TO_TICKS(x)		((x) * (TICKS_PER_SEC))
#endif

/* Buffers */

#ifndef GKI_BUF0_SIZE
# define GKI_BUF0_SIZE				64
#endif

#ifndef GKI_BUF0_MAX
# define GKI_BUF0_MAX				48
#endif

#define GKI_BUF1_SIZE				128

#ifndef GKI_BUF1_SIZE
# define GKI_BUF1_SIZE				288
#endif

#ifndef GKI_BUF1_MAX
# define GKI_BUF1_MAX				26
#endif

#ifndef GKI_POOL_ID_1
# define GKI_POOL_ID_1				1
#endif

#ifndef GKI_BUF2_SIZE
# define GKI_BUF2_SIZE				660
#endif

#ifndef GKI_BUF2_MAX
# define GKI_BUF2_MAX				45
#endif

#ifndef GKI_POOL_ID_2
# define GKI_POOL_ID_2				2
#endif

#define GKI_BUF3_SIZE				1800

#ifndef GKI_BUF3_SIZE
# define GKI_BUF3_SIZE				(4096 + 16)
#endif

#define GKI_BUF3_MAX				30

#ifndef GKI_BUF3_MAX
#define GKI_BUF3_MAX				200
#endif

#ifndef GKI_POOL_ID_3
# define GKI_POOL_ID_3				3
#endif

#define GKI_BUF4_SIZE				0x2000

#ifndef GKI_BUF4_SIZE
# define GKI_BUF4_SIZE				(8080 + 26)
#endif

#define GKI_BUF4_MAX				9

#ifndef GKI_BUF4_MAX
# define GKI_BUF4_MAX				(OBX_NUM_SERVERS + OBX_NUM_CLIENTS)
#endif

// Max size of public buffers (buffers 0 - 3)
#ifndef GKI_MAX_BUF_SIZE
# define GKI_MAX_BUF_SIZE			GKI_BUF3_SIZE
#endif

#define GKI_NUM_FIXED_BUF_POOLS		5

#ifndef GKI_NUM_FIXED_BUF_POOLS
# define GKI_NUM_FIXED_BUF_POOLS	10
#endif

#define GKI_NUM_TOTAL_BUF_POOLS		9

#ifndef GKI_NUM_TOTAL_BUF_POOLS
# define GKI_NUM_TOTAL_BUF_POOLS	10
#endif

#define GKI_DEF_BUFPOOL_PERM_MASK	0xfff0

#ifndef GKI_DEF_BUFPOOL_PERM_MASK
# define GKI_DEF_BUFPOOL_PERM_MASK	0xfdf0
#endif

#endif // BTE_GKI_TARGET_H
