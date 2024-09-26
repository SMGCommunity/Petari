#pragma once

#include "nw4r/ut/ResFontBase.hpp"
#include "nw4r/ut/binaryFileFormat.hpp"

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