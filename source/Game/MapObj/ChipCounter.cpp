#include "Game/MapObj/ChipCounter.hpp"
#include "Game/NPC/TalkDirector.hpp"
#include "Game/Util.hpp"

ChipCounter::ChipCounter(const char *pName, s32 type) : LayoutActor(pName, true) {
    mCollectCounter = 0;
    mCount = 0;
    mType = type;
    _2C = -1;
    _30 = 1.0f;
}

void ChipCounter::init(const JMapInfoIter &rIter) {
    MR::connectToSceneLayout(this);

    switch(mType) {
        case 0:
            initLayoutManager("BlueChipCounter", 2);
            break;
        case 1:
            initLayoutManager("YellowChipCounter", 2);
            break;
    }

    initNerve(&NrvChipCounter::ChipCounterNrvHide::sInstance);

    for (s32 i = 0; i < sChipPainCount; i++) {
        MR::createAndAddPaneCtrl(this, sChipPainName[i], 2);
        MR::startPaneAnim(this, sChipPainName[i], "ChipGet", 0);
    }

    MR::startAnim(this, "ShowHide", 1);
    MR::setAnimFrameAndStop(this, _30 * 20.0f, 1);
    mCollectCounter = new CollectCounter("集め数字");
    mCollectCounter->initWithoutIter();
    kill();
}

void ChipCounter::control() {
    if (MR::isActiveTalkBalloonShort()) {
        _30 -= 0.050000001f;

        if (_30 < 0.0f) {
            _30 = 0.0f;
        }
    }
    else {
        _30 += 0.050000001f;

        if (_30 > 1.0f) {
            _30 = 1.0f;
        }
    }

    MR::setAnimFrameAndStop(this, (_30 * 20.0f), 1);
}

// ChipCounter::setCount

