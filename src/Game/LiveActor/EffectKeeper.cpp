#include "Game/LiveActor/EffectKeeper.hpp"
#include "Game/Effect/AutoEffectInfo.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/Effect/MultiEmitter.hpp"
#include "Game/Effect/SyncBckEffectChecker.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/HashUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <algorithm>
#include <cstdio>

namespace {
    const char* cAttributeEffectTag = "Attr";
};  // namespace

namespace {
    const char* getMultiEmitterName(const MultiEmitter* pEmitter) {
        return pEmitter->_28->getName();
    }

    bool makeAttibuteEffectBaseName(char* pDst, u32 size, const char* pSrc) {
        const char* tag = strstr(pSrc, cAttributeEffectTag);

        if (tag == nullptr) {
            return false;
        }

        MR::extractString(pDst, pSrc, strlen(tag) - strlen(pSrc), size);

        return true;
    }
};  // namespace

EffectKeeper::EffectKeeper(const char* pParam1, ResourceHolder* pParam2, int param3, const char* pParam4) : _0(pParam1), _4(pParam2), _8(pParam4), _C(), _18(), _1C(), _20(), mBinder(), _28(), _2C(), _30() {
    if (_8 == nullptr && _4 != nullptr) {
        _8 = _4->getModelName();
    }

    _C.init(MR::Effect::getAutoEffectNum(_8) + param3);
}

void EffectKeeper::init(LiveActor* pActor) {
    if (_8 != nullptr) {
        MR::Effect::registerAutoEffectInfoGroup(this, pActor, _8);
    }
}

// EffectKeeper::initAfterPlacementForAttributeEffect(MtxPtr)

void EffectKeeper::setBinder(const Binder* pBinder) {
    mBinder = pBinder;

    if (mBinder != nullptr) {
        checkExistenceAttributeEffect();
    }
}

void EffectKeeper::addEffect(const char* pParam1, LiveActor* pActor) {
    MR::Effect::addAutoEffect(this, pActor, MR::Effect::createAutoEffect(_0, pParam1));
}

void EffectKeeper::registerEffect(const char* pParam1, const TVec3f* pParam2, const TVec3f* pParam3, const TVec3f* pParam4, const char* pParam5) {
    registMultiEmitter(new MultiEmitter(pParam1, pParam2, pParam3, pParam4, TVec3f(0.0f, 0.0f, 0.0f)), pParam1, pParam5);
}

void EffectKeeper::registerEffect(const char* pParam1, MtxPtr pParam2, const char* pParam3, const char*) {
    registMultiEmitter(new MultiEmitter(pParam1, pParam2, TVec3f(0.0f, 0.0f, 0.0f)), pParam1, pParam3);
}

void EffectKeeper::registerEffect(const char* pParam1, MtxPtr pParam2, const TVec3f* pParam3, const char* pParam4, const char*) {
    registMultiEmitter(new MultiEmitter(pParam1, pParam2, pParam3, TVec3f(0.0f, 0.0f, 0.0f)), pParam1, pParam4);
}

void EffectKeeper::registerEffectWithoutSRT(const char* pParam1, const char* pParam2) {
    registMultiEmitter(new MultiEmitter(pParam1), pParam1, pParam2);
}

void EffectKeeper::registerSyncBckEffect(XanimePlayer* pParam1, const char* pParam2, const char* pParam3, s32 param4, f32 param5, f32 param6, bool param7) {
    if (_20 == nullptr) {
        _20 = new SyncBckEffectChecker(pParam1);
    }

    MultiEmitter* emitter = getEmitter(pParam2);

    emitter->initSyncBck(pParam1, pParam3, param4, param5);
    emitter->onDeleteSyncBck(param7, param6);
}

MultiEmitter* EffectKeeper::createEmitter(const char* pParam1) {
    MultiEmitter* emitter = getEmitter(pParam1);

    emitter->createEmitter();

    return emitter;
}

void EffectKeeper::deleteEmitter(const char* pParam1) {
    getEmitter(pParam1)->deleteEmitter();
}

void EffectKeeper::forceDeleteEmitter(const char* pParam1) {
    getEmitter(pParam1)->forceDeleteEmitter();
}

void EffectKeeper::deleteEmitterAll() {
    std::for_each(_C.begin(), _C.end(), std::mem_func(&MultiEmitter::deleteEmitter));
}

void EffectKeeper::forceDeleteEmitterAll() {
    std::for_each(_C.begin(), _C.end(), std::mem_func(&MultiEmitter::forceDeleteEmitter));
}

MultiEmitter* EffectKeeper::getEmitter(const char* pParam1) const {
    char buf[256];

    if (_30 && !MR::isEqualSubString(pParam1, ::cAttributeEffectTag) && isTypeAttributeEffect(pParam1)) {
        snprintf(buf, sizeof(buf), "%s%s%s", pParam1, ::cAttributeEffectTag, MR::Effect::getEffectAttributeName(_28));

        if (!isRegisteredEmitter(buf)) {
            snprintf(buf, sizeof(buf), "%s%s%s", pParam1, ::cAttributeEffectTag, "Default");
        }

        pParam1 = buf;
    }

    if (_18 != nullptr && _18->mHasBeenSorted) {
        u32 result = 0;

        if (_18->search(pParam1, &result)) {
            return reinterpret_cast< MultiEmitter* >(result);
        }
    }

    u16 name = MR::getHashCode(pParam1);

    for (MultiEmitter* const* it = _C.begin(); it != _C.end(); it++) {
        if ((*it)->isEqualName(name)) {
            return *it;
        }
    }

    return nullptr;
}

MultiEmitter* EffectKeeper::getEmitter(s32 index) const {
    return _C[index];
}

void EffectKeeper::changeEffectName(const char* pParam1, const char* pName) {
    if (_18 != nullptr) {
        _18->swap(pParam1, pName);
    }

    getEmitter(pParam1)->setName(pName);
}

void EffectKeeper::update() {
    updateSyncBckEffect();
    updateAttributeEffect();
}

void EffectKeeper::updateSyncBckEffect() {
    if (_20 == nullptr) {
        return;
    }

    _20->updateBefore();

    for (MultiEmitter* const* it = _C.begin(); it != _C.end(); it++) {
        syncEffectBck(*it);
    }

    _20->updateAfter();
}

void EffectKeeper::syncEffectBck(MultiEmitter* pEmitter) {
    SyncBckEffectInfo* pInfo = pEmitter->_24;

    if (pInfo == 0) {
        return;
    }

    bool b1 = _20->isCreate(pInfo, true);
    bool b2 = _20->isCreate(pInfo, false);

    if (b1 || b2) {
        if (MR::isEqualSubString(::getMultiEmitterName(pEmitter), ::cAttributeEffectTag)) {
            char buf[256];
            ::makeAttibuteEffectBaseName(buf, sizeof(buf), ::getMultiEmitterName(pEmitter));

            pEmitter = getEmitter(buf);
        }

        if (b1) {
            pEmitter->createOneTimeEmitter();
        }

        if (b1) {
            pEmitter->createForeverEmitter();
        }
    }

    if (_20->isDelete(pInfo)) {
        pEmitter->deleteEmitter();
    }
}

void EffectKeeper::updateAttributeEffect() {
    if (!_30) {
        return;
    }

    if (_2C != _28) {
        for (MultiEmitter* const* it = _C.begin(); it != _C.end(); it++) {
            if (!(*it)->isValid()) {
                continue;
            }

            if ((*it)->isExistOneTimeEmitter()) {
                continue;
            }

            if (!MR::isEqualSubString(::getMultiEmitterName(*it), ::cAttributeEffectTag)) {
                continue;
            }

            (*it)->deleteForeverEmitter();

            char buf[256];
            ::makeAttibuteEffectBaseName(buf, sizeof(buf), ::getMultiEmitterName(*it));

            getEmitter(buf)->createEmitter();
        }
    }

    updateFloorCode();
}

void EffectKeeper::stopEmitterOnClipped() {
    std::for_each(_C.begin(), _C.end(), std::mem_func(&MultiEmitter::stopEmitterOnClipped));
}

void EffectKeeper::playEmitterOffClipped() {
    std::for_each(_C.begin(), _C.end(), std::mem_func(&MultiEmitter::playEmitterOffClipped));
}

void EffectKeeper::clear() {
    std::for_each(_C.begin(), _C.end(), std::mem_func(&MultiEmitter::playCalcAndDeleteForeverEmitter));
}

void EffectKeeper::changeBck() {
    if (_20 != nullptr) {
        _20->reset();
    }
}

// EffectKeeper::onDraw
// EffectKeeper::offDraw

void EffectKeeper::enableSort() {
    _18 = new HashSortTable(_C.capacity());
}

void EffectKeeper::finalizeSort() {
    _18->sort();
}

bool EffectKeeper::isRegisteredEmitter(const char* pParam1) const {
    u16 name = MR::getHashCode(pParam1);

    for (MultiEmitter* const* it = _C.begin(); it != _C.end(); it++) {
        if ((*it)->isEqualName(name)) {
            return true;
        }
    }

    return false;
}

void EffectKeeper::checkExistenceAttributeEffect() {
    for (MultiEmitter* const* it = _C.begin(); it != _C.end(); it++) {
        if (MR::isEqualSubString(::getMultiEmitterName(*it), ::cAttributeEffectTag)) {
            _30 = true;
            break;
        }
    }
}

void EffectKeeper::registMultiEmitter(MultiEmitter* pEmitter, const char* pParam2, const char* pParam3) {
    if (pParam3 != nullptr) {
        pEmitter->setName(pParam3);

        if (_18 != nullptr) {
            _18->add(pParam3, reinterpret_cast< u32 >(pEmitter), 0);
        }
    } else {
        if (_18 != nullptr) {
            _18->add(pParam2, reinterpret_cast< u32 >(pEmitter), 0);
        }
    }

    _C.push_back(pEmitter);
}

// EffectKeeper::updateFloorCode

void EffectKeeper::updateFloorCode(const Triangle* pTriangle) {
    if (pTriangle == nullptr) {
        return;
    }

    _2C = _28;
    _28 = MR::getFloorCodeIndex(pTriangle->getAttributes());
}

bool EffectKeeper::isTypeAttributeEffect(const char* pParam1) const {
    char buf[256];
    snprintf(buf, sizeof(buf), "%s%s", pParam1, ::cAttributeEffectTag);

    for (MultiEmitter* const* it = _C.begin(); it != _C.end(); it++) {
        if ((*it)->_28 == nullptr) {
            continue;
        }

        if (MR::isEqualSubString((*it)->_28->getName(), buf)) {
            return true;
        }
    }

    return false;
}
