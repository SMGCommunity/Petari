#ifndef FILE_STRUCT_H
#define FILE_STRUCT_H

enum file_kinds {
    file_closed,
    file_disk,
    file_console,
    file_unavailable
};

enum open_modes {
    must_exist,
    create_if_needed,
    create_or_truncate
};

enum file_orientation {
    file_unoriented,
    file_char_oriented,
    file_wide_oriented
};

typedef struct {
    unsigned int open : 2;
    unsigned int io : 3;
    unsigned int buffer : 2;
    unsigned int file : 3;
    unsigned int file_orientation : 2;
    unsigned int binary : 1;
} file_modes;

typedef unsigned long file_handle;
typedef struct _FILE FILE;

struct _FILE {
    file_handle handle;
    file_modes mode;
};

#endif // FILE_STRUCT_H