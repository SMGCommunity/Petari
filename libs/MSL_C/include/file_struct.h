#ifndef FILE_STRUCT_H
#define FILE_STRUCT_H

#include "ansi_params.h"
#include "size_t.h"
#include "wchar_t.h"

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

enum io_states {
    neutral,
    writing,
    reading,
    rereading
};

typedef struct
{
	unsigned int	io_state	: 3;
	unsigned int	free_buffer	: 1;
	unsigned char	eof;
	unsigned char	error;
} file_state;

typedef unsigned long file_handle;
typedef struct _FILE FILE;
typedef int (* __pos_proc)(unsigned long, long *, int, void *);
typedef int (* __io_proc)(unsigned long, unsigned char *, size_t *, void *);
typedef int (* __close_proc)(unsigned long);

struct _FILE {
    file_handle handle;
    file_modes mode;
    file_state state;
    unsigned char is_dyn_alloc;
    unsigned char char_buf;
    unsigned char char_buf_of;
    unsigned char unget_buffer[2];
    wchar_t ungetwc_buffer[2];
    unsigned long pos;
    unsigned char* buffer;
    unsigned long buffer_size;
    unsigned char* buffer_ptr;
    unsigned long buffer_len;
    unsigned long buffer_alignment;
    unsigned long unk;
    unsigned long buffer_pos;
    __pos_proc pos_proc;
    __io_proc read_proc;
    __io_proc write_proc;
    __close_proc close_proc;
    void* ref;
    struct _FILE* next_file;
};

extern FILE __files[];

#define stdout	(&__std(__files[1])) 

#endif // FILE_STRUCT_H