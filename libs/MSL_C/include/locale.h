#ifndef LOCALE_H
#define LOCALE_H

#include "ansi_params.h"
#include "size_t.h"
#include "wchar_io.h"

typedef int (* __decode_mbyte) (wchar_t *, const char *,  __std(size_t));
typedef int (* __encode_mbyte) (char *, wchar_t);

struct lconv {
    char* decimal_point;
    char* thousands_sep;
    char* grouping;
    char* int_curr_symbol;
    char* currency_symbol;
    char* mon_decimal_point;
    char* mon_thousands_sep;
    char* mon_grouping;
    char* positive_sign;
    char* negative_sign;
    char int_frac_digits;
    char frac_digits;
    char p_cs_precedes;
    char p_sep_by_space;
    char n_cs_precedes;
    char p_sign_posn;
    char n_sign_posn;
    wchar_t* _W_decimal_point;
    wchar_t* _W_thousands_sep;
    wchar_t* _W_int_curr_symbol;
    wchar_t* _W_currency_symbol;
    wchar_t* _W_mon_decimal_point;
    wchar_t* _W_mon_thousands_sep;
    wchar_t* _W_positive_sign;
    wchar_t* _W_negative_sign;
};

struct _loc_coll_cmpt {
    char name[8];
    int char_start;
    int char_coll_tab_size;
    short char_spec;
    unsigned short* char_coll_table_ptr;
    unsigned short* wchar_coll_seq_ptr;
};

struct _loc_ctype_cmpt {
    char name[8];                               // _0
    const unsigned short* ctype_map_ptr;        // _8
    const unsigned char* upper_map_ptr;         // _C
    const unsigned char* lower_map_ptr;         // _10
    const unsigned short* wctype_map_ptr;       // _14
    const wchar_t* wupper_map_ptr;              // _18
    const wchar_t* wlower_map_ptr;              // _1C
    __decode_mbyte  decode_mb;
    __encode_mbyte  encode_wc;
};

struct __locale {
    struct __locale* next_locale;               // _0
    char name[0x30];                            // _4
    struct _loc_coll_cmpt* coll_cmpt_ptr;       // _34
    struct _loc_ctype_cmpt* ctype_cmpt_ptr;     // _38
};

extern struct __locale _current_locale;
extern struct lconv __lconv;

#endif // LOCALE_H