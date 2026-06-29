#include "Game/Player/MarioTalk.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Util/MathUtil.hpp"

void Mario::startTalk(const LiveActor* pActor) {
    stopWalk();

    mTalk->_14 = pActor;

    changeStatus(mTalk);
}

void Mario::endTalk() {
    closeStatus(mTalk);
}

MarioTalk::MarioTalk(MarioActor* pActor) : MarioState(pActor, 34), _14(), mIsUpdate() {
}

bool MarioTalk::update() {
    if (mIsUpdate) {
        TVec3f vec;
        MR::vecKillElement(_14->mPosition - getTrans(), getAirGravityVec(), &vec);

        if (!MR::normalizeOrZero(&vec)) {
            getPlayer()->setFrontVecKeepUp(vec, static_cast< u32 >(16));

            if (!getPlayer()->getMovementStates()._37) {
                getPlayer()->setLastNonFixMoveVec(vec);
            } else if (isPlayerModeTeresa()) {
                MR::vecKillElement(vec, getCamDirZ(), &vec);
                if (!MR::normalizeOrZero(&vec)) {
                    getPlayer()->setLastNonFixMoveVec(vec);
                }
            }
        }

        mIsUpdate = false;
    }

    return true;
}

bool MarioTalk::notice() {
    return true;
}

bool MarioTalk::close() {
    if (getPlayer()->getMovementStates()._37) {
        getPlayer()->setFrontVecKeepUp(getPlayer()->_334);
    }

    return true;
}

bool MarioTalk::start() {
    bool val = mActor->_B90;
    mActor->_B90 = false;

    if (mActor->_468 == 0) {
        stopAnimationUpper(nullptr, nullptr);
    }

    changeAnimationNonStop("見る");

    if (val) {
        mActor->_B90 = true;
    }

    mIsUpdate = true;

    return true;
}

void Mario::setLastNonFixMoveVec(const TVec3f& rVec) {
    _334 = rVec;
}
