#ifndef LOCALE_H
#define LOCALE_H

struct _loc_coll_cmpt {
    char name[8];
    int char_start;
    int char_coll_tab_size;
    short char_spec;
    unsigned short* char_coll_table_ptr;
    unsigned short* wchar_coll_seq_ptr;
};

struct _loc_ctype_cmpt {
    char name[8];
    const unsigned short* ctype_map_ptr;
    const unsigned char* upper_map_ptr;
    const unsigned char* lower_map_ptr;
};

struct __locale {
    struct __locale* next_locale;               // _0
    char name[0x30];                            // _4
    struct _loc_coll_cmpt* coll_cmpt_ptr;       // _34
    struct _loc_ctype_cmpt* ctype_cmpt_ptr;     // _38
};

extern struct __locale _current_locale;

#endif // LOCALE_H