#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/HashUtil.hpp"

#include "Game/Animation/XanimeResource.hpp"

XanimeSwapTable luigiAnimeSwapTable[] = {
    {"Run", "LuigiRun"},
    {"Jump", "LuigiJump"},
    {"JumpRoll", "LuigiJumpRoll"},
    {"JumpBack", "LuigiJumpBack"},
    {"RunEnd", "LuigiRunEnd"},
    {"Spin", "LuigiSpin"},
    {"SpinGround", "LuigiSpinGround"},
    {"SpaceFlyShort", "LuigiSpaceFlyShort"},
    {"Wait", "LuigiWait"},
    {"WaitSlopeL", "LuigiWaitSlopeL"},
    {"WaitSlopeR", "LuigiWaitSlopeR"},
    {"WaitSlopeU", "LuigiWaitSlopeU"},
    {"WaitSlopeD", "LuigiWaitSlopeD"},
    {"", nullptr},
};

namespace {

struct MarioCallbackInfo {
    const char* mName;
    s32 mType;
    void (MarioAnimator::*mEntry)();
    void (MarioAnimator::*mUpdate)();
    void (MarioAnimator::*mClose)();
    u32 _2C;
};

const char* getSpinEffectName(s32 type) {
    if (type == 0) {
        return "スピンライト";
    }
    if (type == 1) {
        return "アイススピン";
    }
    if (type == 2) {
        return "ファイアスピン";
    }
    if (type == 3) {
        return gIsLuigi ? "ハチルイージスピン" : "ハチスピン";
    }
    return nullptr;
}

const char* getThrowEffectName(s32 type) {
    if (type == 0) {
        return "こうら投げ";
    }
    if (type == 1) {
        return "ファイアボール投げ";
    }
    return nullptr;
}

MarioCallbackInfo marioCallbackTable[] = {
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

}  // namespace

void MarioAnimator::initCallbackTable() {
    u32 count = 0;
    for (MarioCallbackInfo* info = marioCallbackTable; info->mName[0]; ++info) {
        ++count;
    }

    HashSortTable* table = new HashSortTable(count);
    _120 = table;

    for (u32 i = 0; i < count; ++i) {
        table->add(marioCallbackTable[i].mName, i, false);
    }
    table->sort();

    _11C = -1;
    _10F = 0;
}

void MarioAnimator::entryCallback(const char* pName) {
    _10F = 0;
    closeCallback();

    u32 idx = 0;
    if (_120->search(pName, &idx)) {
        _11C = idx;
        if (marioCallbackTable[idx].mEntry) {
            (this->*marioCallbackTable[idx].mEntry)();
        }
    }
}

void MarioAnimator::runningCallback() {
    if (_11C == -1) {
        return;
    }

    _10F = 1;
    if (isAnimationStop() || isAnimationTerminate(nullptr)) {
        closeCallback();
        return;
    }

    _10F = 0;

    MarioCallbackInfo& info = marioCallbackTable[_11C];
    if (!isAnimationRun(info.mName)) {
        closeCallback();
        return;
    }

    if (info.mUpdate) {
        (this->*info.mUpdate)();
    }
}

void MarioAnimator::closeCallback() {
    if (_11C == -1) {
        return;
    }

    MarioCallbackInfo& info = marioCallbackTable[_11C];
    if (info.mClose) {
        (this->*info.mClose)();
    }

    _11C = -1;
}

void MarioAnimator::spinEntry() {
    const MarioCallbackInfo& info = marioCallbackTable[_11C];
    const char* effect = getSpinEffectName(info.mType);
    if (effect) {
        playEffect(effect);
    }
}

void MarioAnimator::spinUpdate() {
    if (getFrame() > 30.0f) {
        stopEffect("スピンライト");
    }
}

void MarioAnimator::spinClose() {
    const MarioCallbackInfo& info = marioCallbackTable[_11C];
    const char* effect = getSpinEffectName(info.mType);
    if (effect) {
        stopEffect(effect);
    }
}

void MarioAnimator::stageInCheck() {
    s32 frame = getFrame();
    if (frame == 0x32) {
        Mario* mario = getPlayer();
        playEffectRT("属性ステージイン", *mario->getGravityVec(), getTrans());
    }
}

void MarioAnimator::throwCheck() {
    if (mActor->_38C) {
        return;
    }

    if (getStickP() == 0.0f) {
        return;
    }

    if (getPlayer()->mMovementStates.jumping) {
        return;
    }

    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
}

void MarioAnimator::throwEntry() {
    const MarioCallbackInfo& info = marioCallbackTable[_11C];
    const char* effect = getThrowEffectName(info.mType);
    if (effect) {
        playEffect(effect);
    }
}

void MarioAnimator::throwClose() {
    const MarioCallbackInfo& info = marioCallbackTable[_11C];
    const char* effect = getThrowEffectName(info.mType);
    if (effect) {
        stopEffect(effect);
    }
}

void MarioAnimator::squatSpinCheck() {
    if (getPlayer()->mMovementStates._A) {
        return;
    }

    if (getFrame() <= 40.0f) {
        stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
    }
}

void MarioAnimator::walkinClose() {
    if (!_10F) {
        return;
    }

    stopAnimation(static_cast<const char*>(nullptr), static_cast<const char*>(nullptr));
    getPlayer()->changeAnimationInterpoleFrame(0x10);
}
