#include "Game/Player/MarioState.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

MarioState::MarioState(MarioActor* actor, u32 statusId) : MarioModule(actor) {
    _8 = 0;
    mStatusId = statusId;
    _10 = 0;
}

bool MarioState::proc(u32 msg) {
    switch (msg) {
        case 0:
            start();
            break;
        case 1:
            if (_10 != 0) {
                break;
            }
            _10 = 1;
            notice();
            _10 = 0;
            break;
        case 2:
            return close();
        case 3:
            return update();
        case 4:
            return keep();
    }
    return true;
}

void Mario::sendStateMsg(u32 msg) {
    MarioState* pState = _97C;
    while (pState != 0) {
        MarioState* pNext = (MarioState*)pState->_8;
        if (isStatusActiveID(pState->mStatusId) == 0) {
            pState = pNext;
            continue;
        }
        if (pState->proc(msg) == 0) {
            if (isStatusActiveID(pState->mStatusId) != 0) {
                closeStatus(pState);
            }
        } else {
            if (msg == 2) {
                msg = 4;
            }
        }
        pState = pNext;
    }
}

bool Mario::updatePosture(MtxPtr mtx) {
    if (_97C != 0) {
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
    if (isStatusActiveID(pState->mStatusId)) {
        return;
    }
    _980 = pState;
    sendStateMsg(3);
    if (_97C) {
        pState->_8 = (u32)_97C;
    }
    _97C = pState;
    if (!pState->proc(0)) {
        closeStatus(pState);
        _97C = (MarioState*)pState->_8;
    }
}

void Mario::closeStatus(MarioState* pState) {
    if (!pState) {
        while (_97C) {
            closeStatus(_97C);
        }
        return;
    }
    
    MarioState* pCurr = _97C;
    if (pCurr == pState) {
        _97C = (MarioState*)pState->_8;
    } else {
        while (pCurr) {
            MarioState* pNext = (MarioState*)pCurr->_8;
            if (pNext == pState) {
                break;
            }
            pCurr = pNext;
        }
        pCurr->_8 = pState->_8;
    }
    pState->_8 = 0;
    pState->proc(1);
}

u32 Mario::getCurrentStatus() const {
    MarioState* pState = _97C;
    if (pState == 0) {
        return 0;
    }
    return pState->mStatusId;
}

bool Mario::isStatusActive(u32 statusId) const {
    MarioState* pState = _97C;
    if (!pState) {
        return false;
    }
    if (pState->mStatusId == statusId) {
        return true;
    }
    while (pState) {
        if (pState->mStatusId == statusId) {
            return true;
        }
        pState = (MarioState*)pState->_8;
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
