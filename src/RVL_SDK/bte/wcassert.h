/* Original source:
 * bluedroid <android.googlesource.com/platform/external/bluetooth/bluedroid>
 * stack/include/wcassert.h
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
 * anti-header guard
 */

#ifdef __MWERKS__
# pragma notonce
#endif

/*******************************************************************************
 * WCAssert macro family
 */

// CLEANUP: Removed parentheses around both "wc_assert"s

#undef WCAssert_FileLine /* <assert.h> semantics */

#if !defined(NDEBUG)
# define WCAssert_FileLine(file_, line_, expr_)					\
	do															\
	{															\
		if (!(expr_))											\
			wc_assert("ASSERT at %s line %d\n", file_, line_);	\
	} while (0)
#else
# define WCAssert_FileLine(file_, line_, expr_)
#endif

#define WCAssert_File(file_, expr_)	WCAssert_FileLine(  file_ , __LINE__, expr_)
#define WCAssert_Line(line_, expr_)	WCAssert_FileLine(__FILE__,   line_ , expr_)
#define WCAssert(expr_)				WCAssert_FileLine(__FILE__, __LINE__, expr_)

/*******************************************************************************
 * functions
 */

#ifndef BTE_WC_ASSERT_H
#define BTE_WC_ASSERT_H

#ifdef __cplusplus
	extern "C" {
#endif

/* WARNING: This function does not have a definition. */
void wc_assert(char const *message, char const *file, unsigned long int line);

#ifdef __cplusplus
	}
#endif

#endif // BTE_WC_ASSERT_H
