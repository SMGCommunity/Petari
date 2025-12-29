#include "utl.h"

/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * bta/sys/utl.c
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

#include <stddef.h> // NULL

#include "bt_types.h"
#include "data_types.h"

#include "btm_api.h"
#include "gki.h"

/*******************************************************************************
 * functions
 */

INT16 utl_str2int(char const *p_s)
{
	INT32 val = 0;

	while (*p_s == ' ' && *p_s != '\0')
		++p_s;

	if (*p_s == '\0')
		return -1;

	while (TRUE)
	{
		if (*p_s < '0' || *p_s > '9')
			return -1;

		val += (INT32)(*p_s++ - '0');

		if (val > 32767)
			return -1;

		if (*p_s == 0)
			return (INT16)val;
		else
			val *= 10;
	}
}

int utl_strucmp(char const *p_s, char const *p_t)
{
	char c;

	while (*p_s != '\0' && *p_t != '\0')
	{
		c = *p_t++;

		if (c >= 'a' && c <= 'z')
			c -= 0x20;

		if (*p_s++ != c)
			return -1;
	}

	if (*p_t == '\0' && *p_s != '\0')
		return 1;
	else
		return 0;
}

UINT8 utl_itoa(UINT16 i, char *p_s)
{
	UINT16 j;
	UINT16 k;
	char *p = p_s;
	BOOLEAN fill = FALSE;

	if (i == 0)
	{
		*p++ = '0';
	}
	else
	{
		for (j = 10000; j > 0; j /= 10)
		{
			k = i / j;
			i = i % j;

			if (k > 0 || fill)
			{
				*p++ = k + '0';
				fill = TRUE;
			}
		}
	}

	*p = '\0';

	return (UINT8)(p - p_s);
}

void utl_freebuf(void **p)
{
	if (*p)
	{
		GKI_freebuf(*p);
		*p = NULL;
	}
}

BOOLEAN utl_set_device_class(tBTA_UTL_COD *p_cod, tUTL_SET_DEVICE_CLASS_CMD cmd)
{
	tBTM_STATUS btm_status;
	UINT8 *dev;
	UINT16 service;
	UINT8 minor;
	UINT8 major;
	DEV_CLASS dev_class;

	dev = BTM_ReadDeviceClass();
	BTM_GET_COD_SERVICE_CLASS(dev, &service);
	BTM_GET_COD_MINOR_CLASS(dev, &minor);
	BTM_GET_COD_MAJOR_CLASS(dev, &major);

	switch (cmd)
	{
	case BTA_UTL_SET_COD_MAJOR_MINOR:
		minor = p_cod->minor & BTM_COD_MINOR_CLASS_MASK;
		major = p_cod->major & BTM_COD_MAJOR_CLASS_MASK;
		break;

	case BTA_UTL_SET_COD_SERVICE_CLASS:
		p_cod->service &= BTM_COD_SERVICE_CLASS_MASK;
		service = service | p_cod->service;
		break;

	case BTA_UTL_CLR_COD_SERVICE_CLASS:
		p_cod->service &= BTM_COD_SERVICE_CLASS_MASK;
		service = service & ~p_cod->service;
		break;

	case BTA_UTL_SET_COD_ALL:
		minor = p_cod->minor & BTM_COD_MINOR_CLASS_MASK;
		major = p_cod->major & BTM_COD_MAJOR_CLASS_MASK;

		p_cod->service &= BTM_COD_SERVICE_CLASS_MASK;
		service = service | p_cod->service;
		break;

	case BTA_UTL_INIT_COD:
		minor = p_cod->minor & BTM_COD_MINOR_CLASS_MASK;
		major = p_cod->major & BTM_COD_MAJOR_CLASS_MASK;

		service = p_cod->service & BTM_COD_SERVICE_CLASS_MASK;
		break;

	default:
		return FALSE;
	}

	MAKE_COD_FROM_FIELDS(dev_class, minor, major, service);

	if ((btm_status = BTM_SetDeviceClass(dev_class)) == BTM_SUCCESS)
		return TRUE;
	else
		return FALSE;
}
