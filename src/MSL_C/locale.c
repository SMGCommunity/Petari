#include "locale.h"
#include "mbstring.h"
#include "ctype.h"
#include "wctype.h"

#pragma options align=native
#pragma warn_padding off
struct lconv __lconv = {
    ".",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    "",
    127,
    127,
    127,
    127,
    127,
    127,
    127,
    "",
    127,
    127,
    127,
    127,
    127,
    127,
    127
};
#pragma warn_padding reset
#pragma options align=reset

struct _loc_ctype_cmpt _loc_ctyp_C = {
    "C",
    &__ctype_mapC[0],
    &__upper_mapC[0],
    &__lower_mapC[0],
    &__wctype_mapC[0],
    &__wupper_mapC[0],
    &__wlower_mapC[0],
    __mbtowc_noconv,
    __wctomb_noconv
};


unsigned short char_coll_tableC[0x60] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
    0x17, 0x2b, 0x2d, 0x2f, 0x31, 0x33, 0x35, 0x37, 0x39, 0x3b, 0x3d, 0x3f, 0x41, 0x43, 0x45, 0x47,
    0x49, 0x4b, 0x4d, 0x4f, 0x51, 0x53, 0x55, 0x57, 0x59, 0x5b, 0x5d, 0x18, 0x19, 0x1a, 0x1b, 0x1c,
    0x00, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48,
    0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x1d, 0x1e, 0x1f, 0x20
};

struct _loc_coll_cmpt _loc_coll_C = {
    "C",
    0x20,
    0x6E,
    0,
    (unsigned short*)&char_coll_tableC[0],
    0
};

struct _loc_mon_cmpt _loc_mon_C = {
    "C",
    "",
    "",
    "",
    "",
    "",
    "",
    127,
    127,
    127,
    127,
    127,
    127,
    127,
    "",
    127,
    127,
    127,
    127,
    127,
    127,
    127
};

struct _loc_num_cmpt _loc_num_C = {
    "C",
    ".",
    "",
    ""
};

struct _loc_time_cmpt _loc_tim_C = {
    "C",
    "AM|PM",
    "%a %b %e %T %Y",
    "%I:%M:%S %p",
    "%m/%d/%y",
    "%T",
    "Sun|Sunday|Mon|Monday|Tue|Tuesday|Wed|Wednesday|Thu|Thursday|Fri|Friday|Sat|Saturday",
    "Jan|January|Feb|February|Mar|March|Apr|April|May|May|Jun|June|Jul|July|Aug|August|Sep|September|Oct|October|Nov|November|Dec|December",
    ""
};


struct __locale _current_locale = {
    0,
    "C",
    &_loc_coll_C,
    &_loc_ctyp_C,
    &_loc_mon_C,
    &_loc_num_C,
    &_loc_tim_C
};
