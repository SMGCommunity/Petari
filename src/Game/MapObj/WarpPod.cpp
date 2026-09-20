#include "Game/MapObj/WarpPod.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/DirectDrawUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>
#include <cstdio>

void WarpPod_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)3.0f;
    (void)MR::pi();
    (void)2.0f;
    (void)200.0f;
    (void)(MR::pi() / 4.0f);
    (void)30.0f;
    (void)100.0f;
}

GXColor gGlowEffectEnvColor[] = {
    {0, 100, 200}, {44, 255, 42}, {255, 60, 60}, {196, 166, 0}, {0, 255, 0}, {255, 0, 255}, {255, 255, 0}, {255, 255, 255},
};

namespace {
    static f32 cSensorRadius0 = 120.0f;
    static f32 cSensorRadius1 = 15.0f;
};  // namespace

namespace MR {
    static u32 mDrawTimer;

    WarpPodMgr* getWarpPodManager() {
        if (!isExistSceneObj(SceneObj_WarpPodMgr)) {
            return nullptr;
        }

        return getSceneObj< WarpPodMgr >(SceneObj_WarpPodMgr);
    }
};  // namespace MR

WarpPodMgr::WarpPodMgr(const char* pName) : NameObj(pName) {
    _10 = new LiveActorGroup("ワープポッド群", 128);
    _C = nullptr;
    _14 = 0;

    MR::connectToScene(this, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_WarpPodPath);
}

WarpPod* WarpPodMgr::getPairPod(const LiveActor* pParam1) {
    if (static_cast< const WarpPod* >(pParam1)->mJMapIdInfo == nullptr) {
        return nullptr;
    }

    for (u32 i = 0; i < _10->getObjNum(); i++) {
        WarpPod* pWarpPod = static_cast< WarpPod* >(_10->getActor(i));

        if (pWarpPod == pParam1) {
            continue;
        }

        if (pWarpPod->mJMapIdInfo == nullptr) {
            continue;
        }

        if (*pWarpPod->mJMapIdInfo == *static_cast< const WarpPod* >(pParam1)->mJMapIdInfo) {
            return pWarpPod;
        }
    }

    return nullptr;
}

void WarpPodMgr::startEventCamera(const LiveActor* pWarpPod) {
    static_cast< const WarpPod* >(pWarpPod)->startEventCamera();
    _C = pWarpPod;
}

void WarpPodMgr::endEventCamera() {
    if (_C == nullptr) {
        return;
    }

    const_cast< WarpPod* >(static_cast< const WarpPod* >(_C))->endEventCamera();

    WarpPod* pPairPod = getPairPod(_C);
    pPairPod->mDelay = 60;
    MR::startBck(pPairPod, "Wait");
    MR::startBrk(pPairPod, "Wait");

    _C = nullptr;
}

void WarpPodMgr::notifyWarpEnd(WarpPod* pWarpPod) {
    if (pWarpPod == nullptr) {
        return;
    }

    WarpPod* pPairPod = getPairPod(pWarpPod);
    pPairPod->mDelay = 60;
    MR::startBck(pPairPod, "Wait");
    MR::startBrk(pPairPod, "Wait");

    _C = nullptr;
}

void WarpPodMgr::draw() const {
    for (u32 i = 0; i < _10->getObjNum(); i++) {
        static_cast< WarpPod* >(_10->getActor(i))->drawCylinder(MR::mDrawTimer);
    }

    MR::mDrawTimer++;
}

void WarpPod::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_WarpPodMgr);

    MR::joinToGroup(this, "ワープポッド群");

    LiveActor::init(rIter);
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("WarpPod", nullptr, false);

    s32 groupId = -1;
    MR::getJMapInfoGroupID(rIter, &groupId);

    if (groupId >= 0) {
        mJMapIdInfo = new JMapIdInfo(groupId, rIter);
    }

    mGroupId = groupId;
    mVisibilityState = 1;
    mArg2 = -1;
    mArg3 = -1;
    mGrandstarReq = -1;
    mCameraTime = 120;
    mGlowColorIndex = 0;
    s32 arg7 = -1;

    if (MR::isValidInfo(rIter)) {
        MR::getJMapInfoArg1NoInit(rIter, &mVisibilityState);
        MR::getJMapInfoArg2NoInit(rIter, &mArg2);
        MR::getJMapInfoArg3NoInit(rIter, &mArg3);
        MR::getJMapInfoArg4NoInit(rIter, &mGrandstarReq);
        MR::getJMapInfoArg5NoInit(rIter, &mCameraTime);
        MR::getJMapInfoArg6NoInit(rIter, &mGlowColorIndex);
        MR::getJMapInfoArg7NoInit(rIter, &arg7);
    }

    if (arg7 == 1) {
        mArg7 = true;
    } else {
        mArg7 = false;
    }

    mCamInfo = new ActorCameraInfo(rIter);

    s32 arg0;
    MR::getJMapInfoArg0WithInit(rIter, &arg0);

    char eventCameraName[256];
    sprintf(eventCameraName, "ワープカメラ %d-%c", groupId, arg0 + 65);
    MR::declareEventCamera(mCamInfo, eventCameraName);

    mEventCameraName = new char[strlen(eventCameraName) + 1];
    strcpy(mEventCameraName, eventCameraName);

    if (mVisibilityState == 0) {
        MR::connectToScene(this, MR::MovementType_MapObj, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
    } else {
        MR::connectToScene(this, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, MR::DrawBufferType_MapObj, MR::DrawType_None);
    }

    initSound(4, false);
    initHitSensor(1);

    f32 scale = mScale.x;
    f32 radius = mVisibilityState == 0 ? scale * ::cSensorRadius0 : scale * ::cSensorRadius1;
    MR::addHitSensorEye(this, "eye", 8, radius, TVec3f(0.0f, 0.0f, 0.0f));

    mDelay = 0;
    _A2 = 0;
    _A6 = 0;
    _CD = false;

    initEffectKeeper(1, nullptr, false);

    MR::validateClipping(this);
    MR::setClippingFarMax(this);

    makeActorAppeared();

    _A4 = 0;

    if (mVisibilityState != 0) {
        MR::startBck(this, "Active");
        MR::startBrk(this, "Active");
    }

    bool isNonActive = false;
    if (MR::calcOpenedAstroDomeNum() < mGrandstarReq) {
        isNonActive = true;
    }

    if (mArg3 == 0) {
        s32 index = MR::getWarpPodManager()->_14++;
        mPathFlagIndex = index;
        if (MR::isOnWarpPodPathFlag(mPathFlagIndex)) {
            isNonActive = false;
        } else {
            isNonActive = true;
        }
    } else if (!isNonActive) {
        glowEffect();
    }

    if (isNonActive) {
        MR::startBck(this, "Wait");
        MR::startBrk(this, "Wait");

        mIsInactive = true;
    } else {
        mIsInactive = false;
        glowEffect();
    }

    _CC = false;

    mPairPod = MR::getWarpPodManager()->getPairPod(this);

    if (mPairPod != nullptr) {
        initPair();
        mPairPod->initPair();
    }

    MR::tryRegisterDemoCast(this, rIter);
}

void WarpPod::glowEffect() {
    if (mVisibilityState == 0) {
        return;
    }

    MR::emitEffect(this, "EndGlow");
    MR::setEffectEnvColor(this, "EndGlow", gGlowEffectEnvColor[mGlowColorIndex].r, gGlowEffectEnvColor[mGlowColorIndex].g,
                          gGlowEffectEnvColor[mGlowColorIndex].b);
}

void WarpPod::initPair() {
    mPairPod = MR::getWarpPodManager()->getPairPod(this);

    bool someBool;
    if (mPairPod->mPosition.x > mPosition.x) {
        someBool = true;
    } else if (mPairPod->mPosition.y > mPosition.y) {
        someBool = true;
    } else if (mPairPod->mPosition.z > mPosition.z) {
        someBool = true;
    } else {
        someBool = false;
    }

    if (mPairPod->mArg7 != true && mArg7 != true) {
        if (!mArg3) {
            mArg7 = false;
        } else if (!mPairPod->mArg3) {
            mArg7 = true;
        } else {
            mArg7 = someBool;
        }
    }

    initDraw();

    if (!mIsInactive) {
        return;
    }

    if (!mArg7 && mPairPod->mIsInactive) {
        return;
    }

    char buf[256];
    sprintf(buf, "wPod出現カメラ %d", mGroupId);

    _9C = new char[strlen(buf) + 1];
    strcpy(_9C, buf);

    MR::declareEventCamera(mCamInfo, _9C);
}

void WarpPod::appear() {
    if (mIsInactive && mPairPod->mIsInactive && !mArg7) {
        mPairPod->appear();
        mIsInactive = false;
    } else {
        _A6 = mCameraTime;

        MR::invalidateClipping(this);

        mIsInactive = false;

        MR::startSound(this, "SE_OJ_WARP_POD_PATH_APPEAR");
        MR::startBck(this, "Active");
        MR::startBrk(this, "Active");
        glowEffect();
    }
}

void WarpPod::appearWithDemo() {
    if (mIsInactive) {
        if (mArg3 == 0) {
            MR::setWarpPodPathFlag(mPathFlagIndex, true);
        }

        if (mPairPod->mIsInactive && !mArg7) {
            mPairPod->appearWithDemo();

            mIsInactive = false;

            return;
        }
    }

    _CD = true;

    MR::invalidateClipping(this);
}

void WarpPod::control() {
    if (!_CD) {
        return;
    }

    if (!MR::tryStartDemoWithoutCinemaFrame(this, "出現")) {
        return;
    }

    _CD = false;
    _A6 = mCameraTime;

    MR::startEventCameraNoTarget(mCamInfo, _9C, -1);
    MR::startSound(this, "SE_OJ_WARP_POD_PATH_APPEAR");

    MR::requestMovementOn(this);

    MR::pauseOffCameraDirector();

    mIsInactive = false;
    _CC = true;

    MR::startBck(this, "Active");
    MR::startBrk(this, "Active");

    glowEffect();
}

void WarpPod::movement() {
    if (_A6 != 0) {
        if (--_A6 == 0) {
            MR::validateClipping(this);

            if (_CC) {
                MR::endDemo(this, "出現");
            }

            mPairPod->glowEffect();
            MR::startBck(mPairPod, "Active");
            MR::startBrk(mPairPod, "Active");
        }
    } else {
        if (mDelay != 0) {
            if (mDelay == 1 && MR::calcDistanceToPlayer(mPosition) < 200.0f) {
                return;
            }

            if (--mDelay == 0) {
                MR::validateClipping(this);

                MR::startBck(this, "Active");
                MR::startBrk(this, "Active");

                if (mVisibilityState != 0) {
                    MR::startSound(this, "SE_OJ_WARP_POD_ACTIVE");
                }
            }
        }

        LiveActor::movement();

        _A4++;
    }
}

void WarpPod::startEventCamera() const {
    if (_CC) {
        return;
    }

    // FIXME: Should not optimize to beqlr instruction.
    if (!mPairPod->_CC) {
        return;
    }

    MR::startEventCameraNoTarget(mCamInfo, mEventCameraName, -1);
}

void WarpPod::endEventCamera() {
    if (_CC) {
        MR::endEventCamera(mCamInfo, _9C, true, -1);
        _CC = false;
    } else if (mPairPod->_CC) {
        mPairPod->endEventCamera();
    } else {
        MR::endEventCamera(mCamInfo, mEventCameraName, true, -1);
    }
}

void WarpPod::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mIsInactive) {
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (mPairPod->mIsInactive) {
        mPairPod->appearWithDemo();
        MR::invalidateClipping(this);
    } else if (mDelay != 0) {
        if (mDelay < 30) {
            mDelay = 30;
        }
    } else if (MR::sendArbitraryMsg(ACTMES_WARP, pReceiver, pSender)) {
        _A2 = 60;
    }
}

// FIXME: big tvec mess
void WarpPod::initDraw() {
    if (!mArg7) {
        return;
    }

    TVec3f dir(mPairPod->mPosition - mPosition);
    f32 distance = dir.length();

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f vecD4;
    vecD4.cross(dir, -upVec);
    MR::normalizeOrZero(&vecD4);

    TVec3f vec104(mPosition + dir * 0.5f);

    TVec3f vecE0;
    vecE0.cross(vecD4, dir);
    MR::normalizeOrZero(&vecE0);

    MR::sin(1.0f);

    _C4 = new TVec3f[60];
    _C8 = 60;

    for (s32 i = 0; i < 60; i++) {
        f32 val = (MR::sin((((60 - i) - 0.5f * 60) / 60) * MR::pi()) + 1.0f) * 0.5f;
        if (mVisibilityState == 2) {
            val = 1 - (i - 1) / 60;
        }

        TPos3f mtx;
        TVec3f vec110;
        PSMTXRotAxisRad(mtx, vec110, (1.0f - val) + val);

        TVec3f vec128;
        TVec3f vec134;
        PSMTXMultVecSR(mtx, vec128, vec134);

        TVec3f vec11C;
        _C4[i] = vec11C + vec134 + upVec * 200.0f;
    }

    _D4 = new JUTTexture(MR::getTexture(MR::getResourceHolder(this), "TestColor.bti"), 0);
    _D8 = new JUTTexture(MR::getTexture(MR::getResourceHolder(this), "TestMask.bti"), 0);
}

void WarpPod::drawCylinder(u32) const {
    f32 length = 30.0f;

    if (!mArg7) {
        return;
    }

    if (mPairPod->mIsInactive) {
        return;
    }

    if (mIsInactive) {
        return;
    }

    if (mVisibilityState != 1) {
        return;
    }

    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
    MR::ddSetVtxFormat(2);
    MR::ddLightingOff();
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ONE, GX_CC_TEXA, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_CC_APREV, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TB_ADDHALF, GX_TEVPREV);
    GXSetTevColor(GX_TEVREG0, ::gGlowEffectEnvColor[mGlowColorIndex]);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);

    _D4->load(GX_TEXMAP0);
    _D8->load(GX_TEXMAP1);

    s32 val1 = _A6;
    u32 val2 = _C8;
    s32 val3 = mCameraTime;
    if (val1 == 0) {
        val1 = mPairPod->_A6;
        val3 = mPairPod->mCameraTime;
    }

    if (val1 != 0) {
        val2 *= 1 - static_cast< f32 >(val1) / val3;
    }

    for (u32 i = 0; i < val2; i++) {
        TVec3f vec84, vec90, vec9C, vecA8, vecB4, vecC0, vecCC, vecD8, vecE4;
        TVec2f vecFC, vec104;

        if (i == 0) {
            u32 j;
            for (j = 1; j < val2; j++) {
                TVec3f vec11C;
                MR::vecKillElement(MR::getCamZdir(), _C4[j] - _C4[i], &vec11C);

                if (MR::normalizeOrZero(&vec11C)) {
                    continue;
                }

                TVec3f vec128;
                vec128.cross(vec11C, MR::getCamZdir());
                MR::normalizeOrZero(&vec128);

                TVec3f vec134;
                vec134.cross(vec128, vec11C);
                MR::normalizeOrZero(&vec134);

                vec128.setLength(length);
                vec134.setLength(length);

                vecA8 = _C4[i] + vec128;
                vec9C = _C4[i] - vec128;
                vec90 = _C4[i] + vec134;
                vec84 = _C4[i] - vec134;

                break;
            }

            if (j >= val2) {
                break;
            }

            vecE4 = _C4[0];

            vec104.set(0.0f, 1.0f);
            vecFC.set(1.0f, 1.0f);

            continue;
        }

        f32 val = 2.0f * (static_cast< f32 >(i + 1) / val2) - 1.0f;
        if (val < 0.0f) {
            val = -val;
        }

        TVec2f vecF4(0.0f, val);
        TVec2f vecEC(1.0f, val);

        TVec3f vec14C;
        MR::vecKillElement(MR::getCamZdir(), _C4[i] - vecE4, &vec14C);

        if (MR::normalizeOrZero(&vec14C)) {
            continue;
        }

        TVec3f vec158;
        vec158.cross(vec14C, MR::getCamZdir());
        MR::normalizeOrZero(&vec158);

        TVec3f vec164;
        vec164.cross(vec158, vec14C);
        MR::normalizeOrZero(&vec164);

        vec158.setLength(length);
        vec164.setLength(length);

        vecD8 = _C4[i] + vec158;
        vecCC = _C4[i] - vec158;
        vecC0 = _C4[i] + vec164;
        vecB4 = _C4[i] - vec164;

        GXBegin(GX_QUADS, GX_VTXFMT0, 8);

        MR::ddSendVtxData(vecA8, vec104);
        MR::ddSendVtxData(vecD8, vecFC);
        MR::ddSendVtxData(vecCC, vecEC);
        MR::ddSendVtxData(vec9C, vecFC);
        MR::ddSendVtxData(vec90, vec104);
        MR::ddSendVtxData(vecC0, vecF4);
        MR::ddSendVtxData(vecB4, vecEC);
        MR::ddSendVtxData(vec84, vecFC);

        vecA8 = vecD8;
        vec104 = vecF4;
        vec9C = vecCC;
        vecFC = vecEC;
        vec90 = vecC0;
        vec84 = vecB4;
        vecE4 = _C4[i];
    }
}

void WarpPod::draw() const {
    if (mVisibilityState == 0) {
        return;
    }

    if (!mArg7) {
        return;
    }

    if (mPairPod->mIsInactive) {
        return;
    }

    if (mIsInactive) {
        return;
    }

    TDDraw::setup(0, 1, 0);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);

    for (u32 i = 0; i < _C8; i++) {
        TDDraw::drawCylinder(_C4[i], _C4[i + 1] - _C4[i], 100.0f, 0x40406040, 0x40406040, 8);
    }
}
