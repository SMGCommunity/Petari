#include "Game/NPC/TrickRabbitUtil.hpp"
#include "Game/Util/FootPrint.hpp"
#include "Game/Util/LiveActorUtil.hpp"

void TrickRabbitUtil_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

namespace TrickRabbitUtil {
    FootPrint* createRabbitFootPrint(LiveActor* pActor) {
        FootPrint* footPrint;

        footPrint = new FootPrint("ウサギ足跡", 64);
        footPrint->mMinPrintDistance = 100.0f;
        footPrint->setTexture(MR::getTexFromArc("RabbitFootprint.bti", pActor));
        footPrint->_2C = 0.0f;
        footPrint->_30 = 30.0f;
        footPrint->_34 = 30.0f;

        return footPrint;
    }
};  // namespace TrickRabbitUtil
