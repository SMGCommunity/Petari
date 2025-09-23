#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/Mario.hpp"

MarioState::MarioState(MarioActor *actor, u32 statusId) : MarioModule(actor)
{
    mNextState = nullptr;
    mStatusId = statusId;
    _10 = 0;
}

// regswap
void Mario::sendStateMsg(u32 msg) {
    MarioState* pTemp = _97C;
    while (pTemp != nullptr) {
        MarioState* pNext = pTemp->mNextState;
        if (!isStatusActive(pTemp->mStatusId)) {
            pTemp = pNext;
            continue;
        }
        else {
            if (!pTemp->proc(msg)) {
                if (isStatusActive(pTemp->mStatusId)) {
                    closeStatus(pTemp);
                }
            }
            else if (msg == 2) {
                msg = 4;
            }
        }
        pTemp = pNext;
    }
}

bool Mario::updatePosture(MtxPtr mtx) {
    if (_97C != nullptr) {
        return _97C->postureCtrl(mtx);
    }
    else {
        postureCtrl(mtx);
        return false;
    }
}

void Mario::changeStatus(MarioState *pState) {
    if (!isStatusActive(pState->mStatusId)) {
        _980 = pState;
        sendStateMsg(3);
        if (_97C != nullptr) {
            pState->mNextState = _97C;
        }
        _97C = pState;
        if (!pState->proc(0)) {
            closeStatus(pState);
            _97C = pState->mNextState;
        }
    }
}

void Mario::closeStatus(MarioState *pState) {
    if (pState == nullptr) {
        while (_97C != nullptr) {
            closeStatus(_97C);
        }
    }
    else {
        if (_97C == pState) {
            _97C = pState->mNextState;
        }
        else {
            MarioState* pTemp = _97C;
            while (pTemp != nullptr) {
                if (pTemp->mNextState == pState) {
                    break;
                }
                pTemp = pTemp->mNextState;
            }
            pTemp->mNextState = pState->mNextState;
        }
        pState->mNextState = nullptr;
        pState->proc(1);
    }
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
        pState = pState->mNextState;
    }
    return false;

}

u32 Mario::getCurrentStatus() const {
    MarioState* pState = _97C;
    if (pState == nullptr) {
        return 0;
    }
    return pState->mStatusId;
}

bool MarioState::proc(u32 msg) {
    switch (msg) {
        case 0:
            return start();
        case 1:
            if (!_10) {
                _10 = 1;
                close();
                _10 = 0;
            }
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

bool MarioState::postureCtrl(MtxPtr mtx) {
    getPlayer()->postureCtrl(mtx);
    return false;
}

u32 MarioState::getNoticedStatus() const {
    return getPlayer()->_980->mStatusId;
}
