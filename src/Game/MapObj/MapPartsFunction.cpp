#include "Game/MapObj/MapPartsFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Spine.hpp"

MapPartsFunction::MapPartsFunction(LiveActor *pActor, const char *pName) : NameObj(pName != 0 ? pName : "マップパーツ機能") {
    mSpine = 0;
    mHost = pActor;
    _14 = 1;
}

void MapPartsFunction::sendMsgToHost(u32 msg) {
    LiveActor* host = mHost;
    host->receiveMessage(msg, host->getSensor("body"), host->getSensor("body"));
}

void MapPartsFunction::movement() {
    if (_14) {
        mSpine->update();
        control();
    }
}

void MapPartsFunction::initNerve(const Nerve *pNerve) {
    mSpine = new Spine(this, pNerve);
}

void MapPartsFunction::setNerve(const Nerve *pNerve) {
    mSpine->setNerve(pNerve);
}

s32 MapPartsFunction::getStep() const {
    return mSpine->mStep;
}

bool MapPartsFunction::isStep(s32 step) const {
    return step == mSpine->mStep;
}

bool MapPartsFunction::isNerve(const Nerve *pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

bool MapPartsFunction::isFirstStep() const {
    return mSpine->mStep == 0;
}
