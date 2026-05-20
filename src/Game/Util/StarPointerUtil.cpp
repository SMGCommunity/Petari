#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Screen/StarPointerDirector.hpp"

namespace MR {

    void createStarPointerLayout() {
        StarPointerFunction::getStarPointerDirector()->createLayout();
    }

}  // namespace MR
