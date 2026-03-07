#include "Game/Enemy/KarikariDirector.hpp"
#include "Game/Enemy/Karikari.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/FixedPosition.hpp"

namespace {

    const ClingData cling_data[] = {{"FacePosition", {-5.86f, 35.0f, 17.0f}, {-18.63f, 8.09f, -23.03f}},
                                    {"ArmL2", {0.0f, 0.0f, 0.0f}, {0.0f, -27.42f, 0.0f}},
                                    {"Hip", {-19.04f, 5.86f, 5.86f}, {165.59f, 80.86f, 0.0f}},
                                    {"Spine1", {0.0f, 0.0f, 0.0f}, {0.0f, 113.55f, 0.0f}},
                                    {"ShoulderR", {19.04f, 0.0f, 0.0f}, {2.29f, -199.69f, 205.0f}}};

    static const s32 hKarikariHoldingMax = ARRAY_SIZE(cling_data);
    static const s32 sNoReleaseTime = 10;
    static const s32 sRumbleInterval = 120;

    KarikariDirector* getKarikariDirector() {
        return MR::getSceneObj< KarikariDirector >(SceneObj_KarikariDirector);
    }
}  // namespace

KarikariDirector::KarikariDirector(const char* pName)
    : LiveActorGroup(pName, 128), mClingNum(0), mClingJointMtxs(nullptr), mClingPositions(nullptr), mClingingKarikari(nullptr),
      mReleaseKarikari(nullptr), mReleaseVel(0.0f, 0.0f), mLowestReleaseDist(999999.0f), mNoReleaseTime(0) {
    MR::connectToSceneEnemyMovement(this);
    mClingJointMtxs = new TPos3f[5];
    mClingPositions = new FixedPosition*[hKarikariHoldingMax];
    mClingingKarikari = new Karikari*[hKarikariHoldingMax];

    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        mClingingKarikari[idx] = nullptr;
        mClingPositions[idx] = new FixedPosition(getClingMtx(idx), TVec3f(cling_data[idx].mPos.x, cling_data[idx].mPos.y, cling_data[idx].mPos.z),
                                                 TVec3f(cling_data[idx].mRotDegree.x, cling_data[idx].mRotDegree.y, cling_data[idx].mRotDegree.z));
    }
}

bool KarikariDirector::isMaxNumCling() const {
    return mClingNum == hKarikariHoldingMax;
}

void KarikariDirector::removeAllClingingKarikari() {
    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        Karikari* karikari = mClingingKarikari[idx];
        if (karikari != nullptr) {
            karikari->tryHipDropRelease();
            unregistCling(karikari);
        }
    }
}

void KarikariDirector::blowOutAllClingingKarikari(const TVec3f& rPos) {
    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        Karikari* karikari = mClingingKarikari[idx];
        if (karikari != nullptr) {
            karikari->tryBlowOut(rPos, false);
        }
    }
}

void KarikariDirector::electricKillAllClingingKarikari() {
    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        Karikari* karikari = mClingingKarikari[idx];
        if (karikari != nullptr) {
            karikari->tryElectricKill();
        }
    }
}

void KarikariDirector::movement() {
    if (mClingNum == 0) {
        mRumbleTime = 1;
        return;
    }

    if (MR::isPlayerSwingAction()) {
        blowOutAllClingingKarikari(*MR::getPlayerPos());
        clearScratchInfo();
    } else if (MR::isPlayerParalyzing()) {
        electricKillAllClingingKarikari();
        clearScratchInfo();
    } else if (MR::isPlayerInRush()) {
        removeAllClingingKarikari();
        clearScratchInfo();
    } else if (mReleaseKarikari != nullptr) {
        unregistCling(mReleaseKarikari);
        mReleaseKarikari->tryDPDRelease(mReleaseVel);
        clearScratchInfo();
    } else {
        mNoReleaseTime--;
    }

    mRumbleTime = (mRumbleTime + 1 + sRumbleInterval) % sRumbleInterval;

    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        MR::calcPlayerJointMtx(&mClingJointMtxs[idx], cling_data[idx].mJointName);
        mClingPositions[idx]->calc();
    }
}

void KarikariDirector::clearScratchInfo() {
    mReleaseKarikari = nullptr;
    mLowestReleaseDist = 100000000.0f;
    mReleaseVel.y = 0.0f;
    mReleaseVel.x = 0.0f;
    mNoReleaseTime = sNoReleaseTime;
}

bool KarikariDirector::requestRelease(Karikari* pReleaseKarikari, TVec2f releaseVel, f32 dist) {
    if (mNoReleaseTime > 0) {
        return false;
    }

    if (mLowestReleaseDist < dist) {
        return false;
    }

    mReleaseKarikari = pReleaseKarikari;
    mLowestReleaseDist = dist;
    mReleaseVel = releaseVel;

    return true;
}

bool KarikariDirector::registCling(Karikari* pKarikari) {
    if (mClingNum >= hKarikariHoldingMax) {
        return false;
    }

    mClingNum++;

    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        if (mClingingKarikari[idx] == nullptr) {
            mClingingKarikari[idx] = pKarikari;
            pKarikari->mClingPosition = mClingPositions[idx];
            return true;
        }
    }

    return false;
}

bool KarikariDirector::unregistCling(Karikari* pKarikari) {
    mClingNum--;

    for (s32 idx = 0; idx < hKarikariHoldingMax; idx++) {
        if (mClingingKarikari[idx] == pKarikari) {
            mClingingKarikari[idx] = nullptr;
            return true;
        }
    }

    return false;
}

s32 MR::getKarikariClingNum() {
    if (!MR::isExistSceneObj(SceneObj_KarikariDirector)) {
        return 0;
    }

    KarikariDirector* director = getKarikariDirector();
    return director->mClingNum;
}

s32 MR::getClingNumMax() {
    if (!MR::isExistSceneObj(SceneObj_KarikariDirector)) {
        return 0;
    }

    KarikariDirector* director = getKarikariDirector();
    return hKarikariHoldingMax;
}

void MR::removeAllClingingKarikari() {
    if (MR::isExistSceneObj(SceneObj_KarikariDirector)) {
        KarikariDirector* director = getKarikariDirector();
        director->removeAllClingingKarikari();
    }
}
