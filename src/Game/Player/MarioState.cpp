#include "Game/Player/MarioState.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"

MarioState::MarioState(MarioActor* pActor, u32 statusId) : MarioModule(pActor), _8(), mStatusId(statusId), _10() {
}

bool MarioState::proc(u32 msg) {
    switch (msg) {
    case 0:
        return start();
    case 1:
        if (_10 != 0) {
            break;
        }
        _10 = 1;
        close();
        _10 = 0;
        break;
    case 2:
        return update();
    case 3:
        return notice();
    case 4:
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

        if (pState->proc(msg) == 0) {
            if (Mario::isStatusActive(pState->mStatusId) != 0) {
                closeStatus(pState);
            }
        } else if (msg == 2) {
            msg = 4;
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
    if (isStatusActive(pState->mStatusId)) {
        return;
    }
    _980 = pState;
    sendStateMsg(3);
    if (_97C) {
        pState->_8 = _97C;
    }
    _97C = pState;
    if (!pState->proc(0)) {
        closeStatus(pState);
        _97C = pState->_8;
    }
}

void Mario::closeStatus(MarioState* pState) {
    if (pState == nullptr) {
        while (_97C) {
            closeStatus(_97C);
        }
        return;
    }

    MarioState* pCurr = _97C;
    if (pCurr == pState) {
        _97C = pState->_8;
    } else {
        while (pCurr) {
            MarioState* pNext = pCurr->_8;
            if (pNext == pState) {
                break;
            }
            pCurr = pNext;
        }
        pCurr->_8 = pState->_8;
    }
    pState->_8 = nullptr;
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
    if (statusId == pState->mStatusId) {
        return true;
    }
    while (pState) {
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
