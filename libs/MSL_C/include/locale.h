#ifndef LOCALE_H
#define LOCALE_H

#include "ansi_params.h"
#include "size_t.h"
#include "wchar_io.h"

typedef int (*__decode_mbyte)(wchar_t*, const char*, __std(size_t));
typedef int (*__encode_mbyte)(char*, wchar_t);

struct lconv {
    /* 0x00 */ char* decimal_point;
    /* 0x04 */ char* thousands_sep;
    /* 0x08 */ char* grouping;
    /* 0x0C */ char* mon_decimal_point;
    /* 0x10 */ char* mon_thousands_sep;
    /* 0x14 */ char* mon_grouping;
    /* 0x18 */ char* positive_sign;
    /* 0x1C */ char* negative_sign;
    /* 0x20 */ char* currency_symbol;
    /* 0x24 */ char frac_digits;
    /* 0x25 */ char p_cs_precedes;
    /* 0x26 */ char n_cs_precedes;
    /* 0x27 */ char p_sep_by_space;
    /* 0x28 */ char n_sep_by_space;
    /* 0x29 */ char p_sign_posn;
    /* 0x2A */ char n_sign_posn;
    /* 0x2C */ char* int_curr_symbol;
    /* 0x30 */ char int_frac_digits;
    /* 0x31 */ char int_p_cs_precedes;
    /* 0x32 */ char int_n_cs_precedes;
    /* 0x33 */ char int_p_sep_by_space;
    /* 0x34 */ char int_n_sep_by_space;
    /* 0x35 */ char int_p_sign_posn;
    /* 0x36 */ char int_n_sign_posn;
};

struct _loc_mon_cmpt {
    /* 0x00 */ char CmptName[8];
    /* 0x08 */ char* mon_decimal_point;
    /* 0x0C */ char* mon_thousands_sep;
    /* 0x10 */ char* mon_grouping;
    /* 0x14 */ char* positive_sign;
    /* 0x18 */ char* negative_sign;
    /* 0x1C */ char* currency_symbol;
    /* 0x20 */ char frac_digits;
    /* 0x21 */ char p_cs_precedes;
    /* 0x22 */ char n_cs_precedes;
    /* 0x23 */ char p_sep_by_space;
    /* 0x24 */ char n_sep_by_space;
    /* 0x25 */ char p_sign_posn;
    /* 0x26 */ char n_sign_posn;
    /* 0x28 */ char* int_curr_symbol;
    /* 0x29 */ char int_frac_digits;
    /* 0x30 */ char int_p_cs_precedes;
    /* 0x31 */ char int_n_cs_precedes;
    /* 0x32 */ char int_p_sep_by_space;
    /* 0x33 */ char int_n_sep_by_space;
    /* 0x34 */ char int_p_sign_posn;
    /* 0x35 */ char int_n_sign_posn;
};

struct _loc_num_cmpt {
    /* 0x00 */ char CmptName[8];
    /* 0x08 */ char* decimal_point;
    /* 0x0C */ char* thousands_sep;
    /* 0x10 */ char* grouping;
};

struct _loc_time_cmpt {
    /* 0x00 */ char CmptName[8];
    /* 0x08 */ const char* am_pm;
    /* 0x0C */ const char* DateTime_Format;
    /* 0x10 */ const char* Twelve_hr_format;
    /* 0x14 */ const char* Date_Format;
    /* 0x18 */ const char* Time_Format;
    /* 0x1C */ const char* Day_Names;
    /* 0x20 */ const char* MonthNames;
    /* 0x24 */ char* TimeZone;
};

struct _loc_coll_cmpt {
    /* 0x00 */ char name[8];
    /* 0x08 */ int char_start;
    /* 0x0C */ int char_coll_tab_size;
    /* 0x10 */ short char_spec;
    /* 0x14 */ unsigned short* char_coll_table_ptr;
    /* 0x18 */ unsigned short* wchar_coll_seq_ptr;
};

struct _loc_ctype_cmpt {
    /* 0x00 */ char name[8];
    /* 0x08 */ const unsigned short* ctype_map_ptr;
    /* 0x0C */ const unsigned char* upper_map_ptr;
    /* 0x10 */ const unsigned char* lower_map_ptr;
    /* 0x14 */ const unsigned short* wctype_map_ptr;
    /* 0x18 */ const wchar_t* wupper_map_ptr;
    /* 0x1C */ const wchar_t* wlower_map_ptr;
    /* 0x20 */ __decode_mbyte decode_mb;
    /* 0x24 */ __encode_mbyte encode_wc;
};

struct __locale {
    /* 0x00 */ struct __locale* next_locale;
    /* 0x04 */ char name[0x30];
    /* 0x34 */ struct _loc_coll_cmpt* coll_cmpt_ptr;
    /* 0x38 */ struct _loc_ctype_cmpt* ctype_cmpt_ptr;
    /* 0x3C */ struct _loc_mon_cmpt* mon_cmpt_ptr;
    /* 0x40 */ struct _loc_num_cmpt* num_cmpt_ptr;
    /* 0x44 */ struct _loc_time_cmpt* time_cmpt_ptr;
};

extern struct __locale _current_locale;
extern struct lconv __lconv;

#endif  // LOCALE_H
