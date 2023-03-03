#ifndef LOCALE_H
#define LOCALE_H

#include "ansi_params.h"
#include "size_t.h"
#include "wchar_io.h"

typedef int (* __decode_mbyte) (wchar_t *, const char *,  __std(size_t));
typedef int (* __encode_mbyte) (char *, wchar_t);

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

#endif // LOCALE_H