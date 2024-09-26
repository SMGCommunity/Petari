#include <wchar_io.h>
#include <types.h>

int fwide(FILE *pFile, int file_mode) {
    int orientation;
    int res;

    if ((pFile == NULL) || (pFile->mode.file == file_closed)) {
        return 0;
    }

    orientation = pFile->mode.file_orientation;

    switch (orientation) {
        case file_unoriented:
            if (file_mode > 0) {
                pFile->mode.file_orientation = file_wide_oriented;
            }
            else if (file_mode < 0) {
                pFile->mode.file_orientation = file_char_oriented;
            }

            res = file_mode;
            break;

        case file_wide_oriented:
            res = 1;
            break;

        case file_char_oriented:
            res = -1;
            break;
    }

    return res;
}