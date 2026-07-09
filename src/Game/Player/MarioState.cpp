#include "Game/Player/MarioState.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

MarioState::MarioState(MarioActor* pActor, u32 statusId) : MarioModule(pActor), _8(), mStatusId(statusId), _10() {
}

bool MarioState::proc(u32 msg) {
    switch (msg) {
    case MarioStateMsg_Start:
        return start();
    case MarioStateMsg_Close:
        if (_10) {
            break;
        }

        _10 = true;
        close();
        _10 = false;
        break;
    case MarioStateMsg_Update:
        return update();
    case MarioStateMsg_Notice:
        return notice();
    case MarioStateMsg_Keep:
        return keep();
    }

    return true;
}

void Mario::sendStateMsg(u32 msg) {
    MarioState* pNext;

    for (MarioState* pState = _97C; pState != nullptr; ) {
        pNext = pState->_8;

        if (isStatusActive(pState->mStatusId) == 0) {
            pState = pNext;
            continue;
        }

        if (!pState->proc(msg)) {
            if (isStatusActive(pState->mStatusId) != 0) {
                closeStatus(pState);
            }
        } else if (msg == MarioStateMsg_Update) {
            msg = MarioStateMsg_Keep;
        }

        pState = pNext;
    }
}

bool Mario::updatePosture(MtxPtr mtx) {
    if (_97C != nullptr) {
        return _97C->postureCtrl(mtx);
    }

    postureCtrl(mtx);

    return false;
}

bool MarioState::postureCtrl(MtxPtr mtx) {
    getPlayer()->postureCtrl(mtx);

    return false;
}

void Mario::changeStatus(MarioState* pState) {
    if (isStatusActive(pState->mStatusId)) {
        return;
    }

    _980 = pState;

    sendStateMsg(MarioStateMsg_Notice);

    if (_97C) {
        pState->_8 = _97C;
    }

    _97C = pState;

    if (!pState->proc(MarioStateMsg_Start)) {
        closeStatus(pState);

        _97C = pState->_8;
    }
}

void Mario::closeStatus(MarioState* pState) {
    if (pState == nullptr) {
        while (_97C != nullptr) {
            closeStatus(_97C);
        }

        return;
    }

    MarioState* pCurr = _97C;

    if (pCurr == pState) {
        _97C = pState->_8;
    } else {
        while (pCurr != nullptr) {
            MarioState* pNext = pCurr->_8;

            if (pNext == pState) {
                break;
            }

            pCurr = pNext;
        }

        pCurr->_8 = pState->_8;
    }

    pState->_8 = nullptr;
    pState->proc(MarioStateMsg_Close);
}

u32 Mario::getCurrentStatus() const {
    MarioState* pState = _97C;

    if (pState == nullptr) {
        return 0;
    }

    return pState->mStatusId;
}

bool Mario::isStatusActive(u32 statusId) const {
    MarioState* pState = _97C;

    if (pState == nullptr) {
        return false;
    }

    if (statusId == pState->mStatusId) {
        return true;
    }

    while (pState != nullptr) {
        if (statusId == pState->mStatusId) {
            return true;
        }

        pState = pState->_8;
    }

    return false;
}

u32 MarioState::getNoticedStatus() const {
    return getPlayer()->_980->mStatusId;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
