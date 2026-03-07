#include "Game/MapObj/SeaBottomTriplePropeller.hpp"

namespace NrvSeaBottomTriplePropeller {
    NEW_NERVE(SeaBottomTriplePropellerNrvWait, SeaBottomTriplePropeller, Wait);
    NEW_NERVE(SeaBottomTriplePropellerNrvBreak, SeaBottomTriplePropeller, Break);
};  // namespace NrvSeaBottomTriplePropeller

// SeaBottomTriplePropeller::SeaBottomTriplePropeller()

// void SeaBottomTriplePropeller::init(const JMapInfoIter& rIter)

// void SeaBottomTriplePropeller::control()

void SeaBottomTriplePropeller::exeBreak() {
    kill();
}

// void SeaBottomTriplePropeller::calcAnim()
