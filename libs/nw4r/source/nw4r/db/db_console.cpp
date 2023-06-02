#include "nw4r/db/console.h"
#include <cstdarg>

namespace nw4r {
    namespace db {
        static inline u16 GetRingUsedLines_(ConsoleHandle console) {
            s32 lines = console->printTop - console->ringTop;

            if (lines < 0) {
                lines += console->height;
            }

            return lines;
        }

        static inline u16 GetActiveLines_(ConsoleHandle console) {
            u16 lines = GetRingUsedLines_(console);

            if (console->printXPos > 0) {
                lines++;
            }

            return lines;
        }

        // this does nothing. Actual print call was optimized out
        void Console_Printf(ConsoleHandle console, const char *format, ...) {
            va_list vlist;
            va_start(vlist, format);
            va_end(vlist);
        }

        s32 Console_GetTotalLInes(ConsoleHandle console) {
            s32 count;
            BOOL enabled = OSDisableInterrupts();
            count = console->ringTopLineCnt + GetActiveLines_(console);
            OSRestoreInterrupts(enabled);
            return count;
        }
    };
};