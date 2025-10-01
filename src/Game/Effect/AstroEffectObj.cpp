#include "Game/Effect/AstroEffectObj.hpp"
#include "Game/Effect/SimpleEffectObj.hpp"
#include "Game/MapObj/AstroMapObjFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"

AstroEffectObj::AstroEffectObj(const char* pName) : SimpleEffectObj(pName) {
    _9C = -1;
}

void AstroEffectObj::init(const JMapInfoIter& rIter) {
    SimpleEffectObj::init(rIter);
    _9C = AstroMapObjFunction::getDomeIdFromArg0(rIter);
    MR::registerDemoSimpleCastAll(this);
    selectNrvWait();
    makeActorAppeared();
}

bool AstroEffectObj::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    return false;
}

void AstroEffectObj::setStateMove() {
    if (AstroMapObjFunction::isEnableRevival(_8C, _9C)) {
        SimpleEffectObj::setStateMove();
    }
}

void AstroEffectObj::selectNrvWait() {
    if (AstroMapObjFunction::isAlreadyRevival(_8C, _9C)) {
        SimpleEffectObj::setStateMove();
        return;
    }
    SimpleEffectObj::setStateWait();
}

AstroEffectObj::~AstroEffectObj() {
    
}
