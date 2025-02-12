#include "nw4r/lyt/resourceAccessor.h"

namespace nw4r {
    namespace lyt {
        ResourceAccessor::~ResourceAccessor() {

        }

        ResourceAccessor::ResourceAccessor() {
            
        }

        ut::Font* ResourceAccessor::GetFont(const char*) {
            return nullptr;
        }
    };
};
