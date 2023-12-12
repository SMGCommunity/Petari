#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class Font;
    };

    namespace lyt {
        class ResourceAccessor {
        public:
            ResourceAccessor();

            virtual ~ResourceAccessor();

            virtual void* GetResource(u32, const char *, u32 *) = 0;
            virtual ut::Font* GetFont(const char *);
        };
    };
};