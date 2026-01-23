#include "Game/Player/MarioMagic.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioModule.hpp"

extern const char lbl_805CD078[];
extern const char lbl_805CD0C8[];
extern const char lbl_805CD0D5[];

#pragma force_active on
__declspec(section ".data") const char gap_07_805CD130_data[0xA0] = { 0 };
#pragma force_active off

void Mario::stopPunch() {
    if (isStatusActive(17)) {
        closeStatus(mMagic);
    }

    MarioActor* actor = mActor;

    if (!actor->_944) {
        actor->_945 = 0;
        actor->_974 = 0;
    }

    actor->_944 = 0;
}

void Mario::startMagic() {
    if (!mMovementStates.jumping) {
        if (!mActor->_468) {
            if (!mMovementStates._23) {
                if (!isStatusActive(7)) {
                    if (isSkatableFloor()) {
                        doSkate();
                    } else {
                        clearSlope();
                        changeStatus(mMagic);
                        stopAnimationUpper(static_cast< const char* >(nullptr), static_cast< const char* >(nullptr));
                        _10._1 = 1;
                    }
                }
            }
        }
    }
}

MarioMagic::MarioMagic(MarioActor* pActor) : MarioState(pActor, 0x11) {
    _12 = 0;
}

bool MarioMagic::close() {
    stopEffect(lbl_805CD0C8);

    if (_12 < 0x1A) {
        playEffect(lbl_805CD0D5);
    }

    return true;
}

bool MarioMagic::start() {
    register const char* base = lbl_805CD078;
    const char* baseCopy = base;
    const char* punchBlurLeft = base + 0x0B;
    const char* punchBlurRight = base + 0x1A;
    const char* commonGroundSpin = base + 0x29;
    const char* voiceSpin = base + 0x38;
    const char* spinJump = base + 0x41;

    changeAnimation(baseCopy, static_cast< const char* >(nullptr));
    stopEffect(punchBlurLeft);
    stopEffect(punchBlurRight);
    playEffect(commonGroundSpin);
    playSound(voiceSpin, -1);
    playSound(spinJump, -1);
    startPadVib(2);
    _12 = 0;
    return true;
}

bool MarioMagic::update() {
    register const char* base = lbl_805CD078;
    const char* baseCopy = base;
    const char* groundTwist = baseCopy + 0x0;

    if (mActor->isRequestJump()) {
        getPlayer()->tryJump();
        return false;
    } else if (!isAnimationRun(groundTwist)) {
        return false;
    }

    _12++;

    if (_12 == 25) {
        stopEffect(base + 0x50);
        playEffect(base + 0x5D);
    }

    if (getPlayer()->mMovementStates.jumping) {
        getPlayer()->procJump(false);
    } else {
        getPlayer()->mainMove();
    }

    getPlayer()->updateWalkSpeed();
    return true;
}
