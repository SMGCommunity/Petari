#pragma once

#include "nw4r/ut/TextWriter.h"
#include <revolution.h>

namespace nw4r {
    namespace db {
        typedef enum {
            CONSOLE_OUTPUT_NONE = 0x00,
            CONSOLE_OUTPUT_DISPLAY = 0x01,
            CONSOLE_OUTPUT_TERMINAL = 0x02,
            CONSOLE_OUTPUT_ALL = (CONSOLE_OUTPUT_DISPLAY | CONSOLE_OUTPUT_TERMINAL)
        } ConsoleOutputType;

        enum {
            CONSOLE_ATTR_NOT_TURNOVER_LINE = 0x01,
            CONSOLE_ATTR_STOP_ON_BUFFER_FULL = 0x02,
            CONSOLE_ATTR_TAB_SIZE_2 = (0x00 << 2),
            CONSOLE_ATTR_TAB_SIZE_4 = (0x01 << 2),
            CONSOLE_ATTR_TAB_SIZE_8 = (0x02 << 2),
            CONSOLE_ATTR_TAB_SIZE_MASK = (0x03 << 2),
            CONSOLE_ATTR_DEFAULT = CONSOLE_ATTR_TAB_SIZE_4
        };

        namespace detail {
            typedef struct ConsoleHead ConsoleHead;
        };

        typedef nw4r::db::detail::ConsoleHead* ConsoleHandle;

        namespace detail {
            struct ConsoleHead {
                u8* textBuf;
                u16 width;
                u16 height;
                u16 priority;
                u16 attr;
                u16 printTop;
                u16 printXPos;
                u16 ringTop;
                s32 ringTopLineCnt;
                s32 viewTopLine;
                s16 viewPosX;
                s16 viewPosY;
                u16 viewLines;
                bool isVisible;
                u8 padding_[1];
                nw4r::ut::TextWriter* writer;
                ConsoleHandle next;
            };
        };  // namespace detail

        void Console_VFPrintf(ConsoleOutputType type, ConsoleHandle console, const char* format, va_list vlist);
        void Console_Printf(ConsoleHandle console, const char* format, ...);
        s32 Console_GetTotalLines(ConsoleHandle console);

        inline void Console_VPrintf(ConsoleHandle console, const char* format, va_list vlist) {
            Console_VFPrintf(CONSOLE_OUTPUT_ALL, console, format, vlist);
        }

        inline void Console_DrawDirect(ConsoleHandle console) {}

        inline bool Console_SetVisible(ConsoleHandle console, bool isVisible) {
            bool before;
            before = console->isVisible;
            console->isVisible = isVisible;
            return before;
        }

        inline u16 Console_GetViewHeight(ConsoleHandle console) {
            return console->viewLines;
        }

        inline s32 Console_SetViewBaseLine(ConsoleHandle console, s32 line) {
            s32 before;
            before = console->viewTopLine;
            console->viewTopLine = line;
            return before;
        }

        inline s32 Console_ShowLatestLine(ConsoleHandle console) {
            s32 baseLine = Console_GetTotalLines(console) - Console_GetViewHeight(console);

            if (baseLine < 0) {
                baseLine = 0;
            }
            (void)Console_SetViewBaseLine(console, baseLine);
            return baseLine;
        }

    };  // namespace db
};  // namespace nw4r
