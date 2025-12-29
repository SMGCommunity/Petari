#ifndef __STDC_STDDEF_H__
#define __STDC_STDDEF_H__

// IWYU pragma: begin_exports

#include <__internal/__NULL.h>
#include <__internal/__size_t.h>

#define offsetof(type, member_designator) ((size_t) & (((type*)0)->member_designator))

// IWYU pragma: end_exports

#endif  // __STDC_STDDEF_H__
