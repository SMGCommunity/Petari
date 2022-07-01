#pragma once

#include <revolution.h>
#include "nw4r/ut/Rect.h"

namespace nw4r {
    namespace ut {
        template <typename T>
        class TextWriterBase;

        template<typename T>
        struct PrintContext {
            TextWriterBase<T>* writer;
            const T* str;
            const f32 xOrigin;
            const f32 yOrigin;
            f32 flags;
        };

        template<typename T>
        class TagProcessorBase {
        public:
            enum Operation
            {
                OPERATION_DEFAULT,
                OPERATION_NO_CHAR_SPACE,
                OPERATION_CHAR_SPACE,
                OPERATION_NEXT_LINE,
                OPERATION_END_DRAW,
                NUM_OF_OPERATION
            };

            typedef PrintContext<T> ContextType;

            TagProcessorBase();

            virtual ~TagProcessorBase();
            virtual Operation Process(u16, ContextType *);
            virtual Operation CalcRect(Rect *, u16, ContextType *);
        };
    };
};