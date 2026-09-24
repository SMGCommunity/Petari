#include "Game/MapObj/CoinHolder.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/Coin.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

void CoinHolder_FORCE_MATCH() {
    TVec3f vector(0.0f, 0.0f, 0.0f);
    (void)-vector;
    (void)0.001f;
}

namespace {
    static const s32 sCoinNumMax = 512;
    static const s32 sCreateCoinNum = 32;
    const f32 sRandomizeVel = 4.0f;
    // sRandomizeVelSplash
    // sCoinVelHorizon
    const f32 sCoinVelVertical = 25.0f;
    // sCoinVelPlayer
    const f32 sCircleVelHRatio = 0.25f;
    const f32 sCircleVelLength = 30.0f;
};  // namespace

CoinHolder::CoinHolder(const char* pName) : DeriveActorGroup< Coin >(pName, ::sCoinNumMax), mHostInfoArr(), mHostInfoCount() {
    mHostInfoArr = new CoinHostInfo[::sCoinNumMax];
}

bool CoinHolder::hopCoin(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rDirection) {
    CoinHostInfo* hostInfo = findHostInfo(pObj);

    if (hostInfo->_8 >= hostInfo->_4) {
        return false;
    }

    Coin* coin = getDeadMember();

    if (coin != nullptr) {
        coin->setHostInfo(hostInfo);
        coin->appearHop(rPosition, rDirection);
        return true;
    }

    return false;
}

bool CoinHolder::appearCoinFix(const NameObj* pObj, const TVec3f& rPosition, s32 count) {
    TVec3f velocity(0.0f, 0.0f, 0.0f);

    return appearCoin(pObj, rPosition, velocity, count, -1, -1, count == 1 ? 0.0f : ::sRandomizeVel);
}

bool CoinHolder::appearCoinPop(const NameObj* pObj, const TVec3f& rPosition, s32 count) {
    TVec3f gravity;
    MR::calcGravityVector(this, rPosition, &gravity, nullptr, 0);
    TVec3f velocity = -gravity * ::sCoinVelVertical;

    return appearCoin(pObj, rPosition, velocity, count, -1, -1, count == 1 ? 0.0f : ::sRandomizeVel);
}

bool CoinHolder::appearCoinPopToDirection(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rDirection, s32 count) {
    TVec3f direction;
    MR::normalize(rDirection, &direction);

    return appearCoin(pObj, rPosition, direction * ::sCoinVelVertical, count, -1, -1, count == 1 ? 0.0f : ::sRandomizeVel);
}

bool CoinHolder::appearCoinToVelocity(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rVelocity, s32 count) {
    return appearCoin(pObj, rPosition, rVelocity, count, -1, -1, count == 1 ? 0.0f : ::sRandomizeVel);
}

bool CoinHolder::appearCoinCircle(const NameObj* pObj, const TVec3f& rPosition, s32 count) {
    if (count == 1) {
        return appearCoinPop(pObj, rPosition, count);
    }

    bool appeared = false;
    TVec3f gravity;
    MR::calcGravityVector(this, rPosition, &gravity, nullptr, 0);
    TVec3f axis;
    MR::makeAxisVerticalZX(&axis, gravity);

    for (s32 i = 0; i < count; i++) {
        f32 angle = 360.0f / count;

        TVec3f horizontal;
        MR::rotateVecDegree(&horizontal, axis, gravity, i * angle);
        horizontal.setLength(::sCircleVelHRatio);
        TVec3f velocity(horizontal - gravity);
        velocity.setLength(::sCircleVelLength);

        appeared |= appearCoin(pObj, rPosition, velocity, 1, -1, -1, 0.0f);
    }

    return appeared;
}

CoinHostInfo* CoinHolder::declare(const NameObj* pObj, s32 count) {
    if (count <= 0) {
        return nullptr;
    }

    CoinHostInfo* hostInfo = findHostInfo(pObj);
    if (hostInfo == nullptr) {
        hostInfo = &mHostInfoArr[mHostInfoCount];
        hostInfo->mHostActor = pObj;
        mHostInfoCount++;
    }

    hostInfo->_4 += count;
    return hostInfo;
}

s32 CoinHolder::getDeclareRemnantCoinCount(const NameObj* pObj) const {
    CoinHostInfo* hostInfo = findHostInfo(pObj);

    if (MR::isGalaxyDarkCometAppearInCurrentStage()) {
        return 0;
    }

    return hostInfo->_4 - hostInfo->_8;
}

CoinHostInfo* CoinHolder::findHostInfo(const NameObj* pObj) const {
    for (s32 i = 0; i < mHostInfoCount; i++) {
        if (mHostInfoArr[i].mHostActor == pObj) {
            return &mHostInfoArr[i];
        }
    }

    return nullptr;
}

void CoinHolder::init(const JMapInfoIter& rIter) {
    Coin* pCoin;

    for (int i = 0; i < ::sCreateCoinNum; i++) {
        pCoin = new Coin("コイン(共用)");
        pCoin->initWithoutIter();
        registerActor(pCoin);
    }
}

namespace MR {
    void createCoinHolder() {
        MR::createSceneObj(SceneObj_CoinRotater);
        MR::createSceneObj(SceneObj_CoinHolder);
    }

    CoinHolder* getCoinHolder() {
        return getSceneObj< CoinHolder >(SceneObj_CoinHolder);
    }

    void addToCoinHolder(const NameObj* pNameObj, Coin* pCoin) {
        getCoinHolder();
        pCoin->setHostInfo(getCoinHolder()->declare(pNameObj, 1));
    }
};  // namespace MR

bool CoinHolder::appearCoin(const NameObj* pObj, const TVec3f& rPosition, const TVec3f& rVelocity, s32 count, s32 life, s32 cannotTime,
                            f32 randomize) {
    CoinHostInfo* pHostInfo = findHostInfo(pObj);
    if (pHostInfo == nullptr) {
        return false;
    }

    bool appeared = false;
    for (s32 i = 0; i < count; i++) {
        if (pHostInfo->_8 >= pHostInfo->_4) {
            break;
        }

        Coin* pCoin = getDeadMember();
        if (pCoin == nullptr) {
            break;
        }

        TVec3f velocity(rVelocity);
        if (!MR::isNearZero(randomize)) {
            MR::addRandomVector(&velocity, velocity, randomize);
        }

        pCoin->setHostInfo(pHostInfo);
        pCoin->appearMove(rPosition, velocity, life, cannotTime);
        appeared = true;
    }

    if (!MR::isGalaxyDarkCometAppearInCurrentStage() && appeared) {
        if (MR::hasME()) {
            MR::startSystemME("ME_COIN_APPEAR_S");
        } else {
            MR::startSystemSE("SE_SY_COIN_APPEAR_S");
        }
    }

    return appeared;
}
