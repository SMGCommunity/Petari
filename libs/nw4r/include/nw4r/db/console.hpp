#pragma once

#include <revolution.h>

namespace nw4r {
    namespace db {
        typedef enum {
            None        = 0,
            Display     = 1,
            Terminal    = 2,
            All         = 3
        } ConsoleOutputType;

        namespace detail {
            typedef struct ConsoleHead ConsoleHead;
        }

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
                s32 viewTopLIne;
                s16 viewPosX;
                s16 viewPosY;
                u16 viewLines;
                bool isVisible;
                u16 pad;
                void* writer;
                ConsoleHandle next;
            };
        };

        void Console_Printf(ConsoleHandle, const char *, ...);
        s32 Console_GetTotalLines(ConsoleHandle);
        
        inline u16 Console_GetViewHeight(ConsoleHandle console) {
            return console->viewLines;
        }

        inline bool Console_SetVisible(ConsoleHandle console, bool isVisible) {
            bool before = console->isVisible;
            console->isVisible = isVisible;
            return before;
        }

        inline s32 Console_SetViewBaseLine(ConsoleHandle console, s32 line) {
            s32 before = console->viewTopLIne;
            console->viewTopLIne = line;
            return before;
        }

        inline s32 Console_ShowLatestLine(ConsoleHandle console) {
            s32 baseLine = Console_GetTotalLines(console) - Console_GetViewHeight(console);

            if (baseLine < 0) {
                baseLine = 0;
            }

            Console_SetViewBaseLine(console, baseLine);
            return baseLine;
        }
    };
};