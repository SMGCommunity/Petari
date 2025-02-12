#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class Font;
    };

    namespace lyt {
        typedef u32 ResType;

        class ResourceAccessor {
        public:
            ResourceAccessor();

            virtual ~ResourceAccessor();
            virtual void* GetResource(ResType, const char *, u32* ) = 0;
            virtual ut::Font* GetFont(const char *);
        };
    };
};
