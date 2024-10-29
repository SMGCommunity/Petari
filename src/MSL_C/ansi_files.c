#include "file_struct.h"
#include "ansi_files.h"
#include "console_io.h"
#include <cstdlib>

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
