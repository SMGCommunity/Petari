#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Util/HashUtil.hpp"

struct MarioAnimationCallback {
    const char* mAnimation;
    s32 mType;
    void (MarioAnimator::*mEntry)();
    void (MarioAnimator::*mUpdate)();
    void (MarioAnimator::*mClose)();
    u32 _2C;
};

MarioAnimationCallback marioCallbackTable[] = {
    {"空中ひねり", 0, &MarioAnimator::spinEntry, &MarioAnimator::spinUpdate, &MarioAnimator::spinClose, 0},
    {"地上ひねり", 0, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"アイスひねり", 1, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"アイスひねり静止", 1, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"ファイアスピン", 2, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"ファイアスピン空中", 2, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"ハチスピン", 3, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"ハチスピン空中", 3, &MarioAnimator::spinEntry, nullptr, &MarioAnimator::spinClose, 0},
    {"ステージインA", 0, nullptr, &MarioAnimator::stageInCheck, nullptr, 0},
    {"投げ", 0, &MarioAnimator::throwEntry, &MarioAnimator::throwCheck, &MarioAnimator::throwClose, 0},
    {"ファイア投げ", 1, &MarioAnimator::throwEntry, nullptr, &MarioAnimator::throwClose, 0},
    {"サマーソルト", 0, nullptr, &MarioAnimator::squatSpinCheck, nullptr, 0},
    {"ウォークイン", 0, nullptr, nullptr, &MarioAnimator::walkinClose, 0},
    {"見る", 0, nullptr, nullptr, &MarioAnimator::walkinClose, 0},
    {"ResultWait", 0, nullptr, nullptr, &MarioAnimator::walkinClose, 0},
    {"ResultWaitGrandStar", 0, nullptr, nullptr, &MarioAnimator::walkinClose, 0},
    {"WatchUpMore", 0, nullptr, nullptr, &MarioAnimator::walkinClose, 0},
    {"", 0, nullptr, nullptr, nullptr, 0},
};

void MarioAnimator::initCallbackTable() {
    u32 count = 0;
    MarioAnimationCallback* callback = marioCallbackTable;
    for (;; count++, callback++) {
        if (!*callback->mAnimation) {
            break;
        }
    }
    mCallbackTable = new HashSortTable(count);
    callback = marioCallbackTable;
    for (u32 i = 0; i < count; callback++, i++) {
        mCallbackTable->add(callback->mAnimation, i, false);
    }
    mCallbackTable->sort();
    mCallbackId = -1;
    mCallbackEnded = false;
}

void MarioAnimator::entryCallback(const char* name) {
    mCallbackEnded = false;
    closeCallback();
    u32 index;
    if (mCallbackTable->search(name, &index)) {
        mCallbackId = index;
        if (marioCallbackTable[mCallbackId].mEntry != nullptr) {
            (this->*marioCallbackTable[mCallbackId].mEntry)();
        }
    }
}

void MarioAnimator::runningCallback() {
    if (mCallbackId == -1) {
        return;
    }
    mCallbackEnded = true;
    if (isAnimationStop() || isAnimationTerminate(nullptr)) {
        closeCallback();
        return;
    }
    mCallbackEnded = false;
    if (!isAnimationRun(marioCallbackTable[mCallbackId].mAnimation)) {
        closeCallback();
        return;
    }
    if (marioCallbackTable[mCallbackId].mUpdate != nullptr) {
        (this->*marioCallbackTable[mCallbackId].mUpdate)();
    }
}

void MarioAnimator::closeCallback() {
    if (mCallbackId != -1 && marioCallbackTable[mCallbackId].mClose != nullptr) {
        (this->*marioCallbackTable[mCallbackId].mClose)();
    }
    mCallbackId = -1;
}

void MarioAnimator::spinEntry() {
    switch (marioCallbackTable[mCallbackId].mType) {
    case 0:
        playEffect("スピンライト");
        break;
    case 1:
        playEffect("アイススピン");
        break;
    case 2:
        playEffect("ファイアスピン");
        break;
    case 3:
        if (gIsLuigi) {
            playEffect("ハチルイージスピン");
        } else {
            playEffect("ハチスピン");
        }
        break;
    }
}

void MarioAnimator::spinUpdate() {
    if (getFrame() > 30.0f) {
        stopEffect("スピンライト");
    }
}

void MarioAnimator::spinClose() {
    switch (marioCallbackTable[mCallbackId].mType) {
    case 0:
        stopEffect("スピンライト");
        break;
    case 1:
        stopEffect("アイススピン");
        break;
    case 2:
        stopEffect("ファイアスピン");
        break;
    case 3:
        if (gIsLuigi) {
            stopEffect("ハチルイージスピン");
        } else {
            stopEffect("ハチスピン");
        }
        break;
    }
}

void MarioAnimator::stageInCheck() {
    if (static_cast< s32 >(getFrame()) == 50) {
        Mario* player = getPlayer();
        playEffectRT("属性ステージイン", player->_368, getTrans());
    }
}

void MarioAnimator::throwCheck() {
    if (mActor->_38C == 0 && getStickP() != 0.0f && !getPlayer()->mMovementStates.jumping) {
        stopAnimation(nullptr);
    }
}

void MarioAnimator::throwEntry() {
    switch (marioCallbackTable[mCallbackId].mType) {
    case 0:
        playEffect("こうら投げ");
        break;
    case 1:
        playEffect("ファイアボール投げ");
        break;
    }
}

void MarioAnimator::throwClose() {
    switch (marioCallbackTable[mCallbackId].mType) {
    case 0:
        stopEffect("こうら投げ");
        break;
    case 1:
        stopEffect("ファイアボール投げ");
        break;
    }
}

void MarioAnimator::squatSpinCheck() {
    if (!getPlayer()->mMovementStates._A && getFrame() >= 40.0f) {
        stopAnimation(nullptr);
    }
}

void MarioAnimator::walkinClose() {
    if (mCallbackEnded) {
        stopAnimation(nullptr);
        getPlayer()->changeAnimationInterpoleFrame(16);
    }
}
