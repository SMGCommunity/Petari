#include "Game/NPC/NPCDirector.hpp"
#include "Game/NPC/NPCParameter.hpp"
#include "Game/Util/ObjUtil.hpp"

NPCDirector::NPCDirector() : NameObj("NPC指揮") {
}

void NPCDirector::init(const JMapInfoIter& rIter) {
    mCapsParameterReader = new NPCCapsParameterReader("");
    mItemParameterReader = new NPCItemParameterReader("");
    mDataResourceHolder = MR::createAndAddResourceHolder("NPCData.arc");
}
