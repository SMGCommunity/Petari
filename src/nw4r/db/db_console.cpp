#include "nw4r/db/console.h"
#include <va_list.h>

namespace nw4r {
    namespace db {
        // does nothing
        void Console_VFPrintf(ConsoleOutputType type, ConsoleHandle console, const char* format, va_list vlist) {}

        void Console_Printf(ConsoleHandle console, const char* format, ...) {
            va_list vlist;
            va_start(vlist, format);
            Console_VFPrintf(CONSOLE_OUTPUT_ALL, console, format, vlist);
            va_end(vlist);
        }

        static inline u16 GetRingUsedLines_(ConsoleHandle console) {
            s32 lines = console->printTop - console->ringTop;

            if (lines < 0) {
                lines += console->height;
            }
            return (u16)lines;
        }

        static inline u16 GetActiveLines_(ConsoleHandle console) {
            u16 lines = GetRingUsedLines_(console);

            if (console->printXPos > 0) {
                lines++;
            }
            return lines;
        }

        s32 Console_GetTotalLines(ConsoleHandle console) {
            s32 count;
            BOOL enabled = OSDisableInterrupts();
            count = console->ringTopLineCnt + GetActiveLines_(console);
            OSRestoreInterrupts(enabled);
            return count;
        }
    };  // namespace db
};  // namespace nw4r
