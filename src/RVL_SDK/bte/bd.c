#include "bd.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/sys/bd.c
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

#include "bt_types.h"

/*******************************************************************************
 * variables
 */

// .sdata2
BD_ADDR const bd_addr_any = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// .sbss2
BD_ADDR const bd_addr_null = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/*******************************************************************************
 * functions
 */

void bdcpy(BD_ADDR a, BD_ADDR const b)
{
	int i;

	for (i = BD_ADDR_LEN; i != 0; --i)
		*a++ = *b++;
}

int bdcmp(BD_ADDR const a, BD_ADDR const b)
{
	int i;

	for (i = BD_ADDR_LEN; i != 0; i--)
	{
		if (*a++ != *b++)
			return -1;
	}

	return 0;
}

int bdcmpany(BD_ADDR const a)
{
	return bdcmp(a, bd_addr_any);
}

void bdsetany(BD_ADDR a)
{
	bdcpy(a, bd_addr_any);
}

