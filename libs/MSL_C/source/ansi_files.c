#include "file_struct.h"
#include "ansi_files.h"
#include "console_io.h"
#include <cstdlib>

FILE __files[4] =
{
    {
        0,
        {
            must_exist,
            1,
            1,
            file_console,
            file_unoriented,
            0
        },

        {
            neutral,
            0,
            0,
            0
        },

        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        stdin_buff,
        0x100,
        stdin_buff,
        0,
        0,
        0,
        0,
        0,
        __read_console,
        __write_console,
        __close_console,
        0,
        &__files[1]
    },

    {
        1,
        {	
            must_exist,
            2,
            1,
            file_console,
            file_unoriented,
            0
        },
    
        {
            neutral,
            0,
            0,
            0
        },

        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        stdout_buff,
        0x100,
        stdout_buff,
        0,
        0,
        0,
        0,
        0,
        __read_console,
        __write_console,
        __close_console,
        0,
        &__files[2]
    },

    {
        2,
        {
            must_exist,
            2,	
            0,
            file_console,
            file_unoriented,
            0
        },

        {
            neutral,
            0,
            0,
            0
        },

        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        stderr_buff,
        0x100,
        stderr_buff,
        0,
        0,
        0,
        0,
        0,
        __read_console,
        __write_console,
        __close_console,
        0,
        &__files[3]
    },

    {
        0,
        {	
            0,
            0,
            0,
            0,
            0,
            0
        },
    
        {
            0,
            0,
            0,
            0
        },

        0,
        0,
        0,
        {0, 0},
        {0, 0},
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
    }
};

void __close_all(void) {
    FILE* file = &__files[0];
    FILE* lastFile;

    while (file != 0) {
        if (file->mode.file != file_closed) {
            fclose(file);
        }

        lastFile = file;
        file = file->next_file;

        if (lastFile->is_dyn_alloc) {
            free(lastFile);
        }
        else {
            lastFile->mode.file = file_unavailable;

            if ((file != 0) && (file->is_dyn_alloc)) {
                lastFile->next_file = 0;
            }
        }
    }
}

int __flush_all(void) {
    int res = 0;
    FILE* file = &__files[0];

    while (file != 0) {
        if (file->mode.file != file_closed) {
            if (fflush(file)) {
                res = -1;
            }
        }

        file = file->next_file;
    }

    return res;
}