#pragma once

#include "nw4r/ut/ResFontBase.h"

namespace nw4r {
    namespace ut {
        struct BinaryFileHeader;

        class ResFont : public detail::ResFontBase {
        public:
            ResFont();
            virtual ~ResFont();

            bool SetResource(void* pBuffer);
            void RemoveResource();

            static FontInformation* Rebuild(BinaryFileHeader* pHeader);
        };
    };  // namespace ut
};  // namespace nw4r
