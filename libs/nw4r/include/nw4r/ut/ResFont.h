#pragma once

#include "nw4r/ut/ResFontBase.h"
#include "nw4r/ut/binaryFileFormat.h"

namespace nw4r {
    namespace ut {
        class ResFont : public detail::ResFontBase {
        public:
            ResFont();

            virtual ~ResFont();

            bool SetResource(void *);
            void* RemoveResource();

            static FontInformation* Rebuild(BinaryFileHeader *);
        };
    };
};