#pragma once

#include "nw4r/ut/Rect.h"
#include <revolution.h>

namespace nw4r {
    namespace ut {

        template <typename T>
        class TextWriterBase;

        template<typename T>
        class PrintContext {
            TextWriterBase<T>* mWriter; // _0
            const T* mString;           // _4
            f32 mXOrigin;               // _8
            f32 mYOrigin;               // _C
            u32 mFlags;                 // _10
        };
    
        template<typename T>
        class TagProcessorBase {
        public:

            typedef PrintContext<T> ContextType;

            enum Operation
            {
                OPERATION_DEFAULT,
                OPERATION_NO_CHAR_SPACE,
                OPERATION_CHAR_SPACE,
                OPERATION_NEXT_LINE,
                OPERATION_END_DRAW,
                NUM_OF_OPERATION
            };

            TagProcessorBase();

            virtual ~TagProcessorBase();
            virtual Operation Process(u16, PrintContext<T> *);
            virtual Operation CalcRect(Rect *, u16, PrintContext<T> *);
        };
    };
};