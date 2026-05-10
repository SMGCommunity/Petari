#include "Game/MapObj/TrampleStar.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace {
    f32 debug1 = 0.003331f;
    f32 debug2 = -0.003331f;
    f32 debug3 = 1.0f;
    f32 debug4 = 0.5f;
    f32 debug5 = 0.5f;
}  // namespace

namespace {
    NEW_NERVE(TrampleStarNrvWait, TrampleStar, Wait);
    NEW_NERVE(TrampleStarNrvBindingCapture, TrampleStar, BindingCapture);
    NEW_NERVE(TrampleStarNrvBindingShoot, TrampleStar, BindingShoot);
}  // namespace

TrampleStarShare::TrampleStarShare(TrampleStar* pTrampleStar) : LiveActor("TrampleStarShare"), mTrampleStar(pTrampleStar) {
    makeActorDead();
}

void TrampleStarShare::copy(TrampleStar* pTrampleStar) const {
    pTrampleStar->mVtxs = mTrampleStar->mVtxs;
    pTrampleStar->mSurfaces = mTrampleStar->mSurfaces;
    pTrampleStar->mLodSurfaces = mTrampleStar->mLodSurfaces;
    pTrampleStar->mVtxOverlap = mTrampleStar->mVtxOverlap;
    pTrampleStar->mTexST = mTrampleStar->mTexST;
    pTrampleStar->mNumSurfaces = mTrampleStar->mNumSurfaces;
    pTrampleStar->mNumVtxs = mTrampleStar->mNumVtxs;
}

TrampleStar::TrampleStar(const char* pName) : LiveActor(pName) {
}

TrampleStar::~TrampleStar() {
}

void TrampleStar::init(const JMapInfoIter& rIter) {
    // FIXME: JUTTexture inline, TVec2 array allocation, incorrect clear mask
    // https://decomp.me/scratch/4dRlF

    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, MR::MovementType_MapObj, MR::CalcAnimType_MapObj, -1, MR::DrawType_FlexibleSphere);

    // FIXME: inline
    mTexture = new JUTTexture(MR::loadTexFromArc("TrampleStar.arc", "TrampleStar.bti"), 0);

    f32 scale = mScale.x * 500.0f;
    initHitSensor(1);
    MR::addHitSensorPriorBinder(this, "body", 4, scale, TVec3f(0.0f, 0.0f, 0.0f));

    mNumSurfaces = 12;
    mNumVtxs = 24;
    mDeformCoeff = new f32[mNumVtxs * 256];
    mDeformSpeed = new f32[mNumVtxs * 256];

    bool isShareStar = false;

    for (u32 actorIndex = 0; actorIndex < MR::getAllLiveActorGroup()->mObjectCount; actorIndex++) {
        LiveActor* actor = MR::getAllLiveActorGroup()->getActor(actorIndex);

        if (strcmp(actor->mName, "TrampleStarShare") == 0) {
            isShareStar = true;
            reinterpret_cast< TrampleStarShare* >(actor)->copy(this);
            break;
        }
    }

    if (!isShareStar) {
        mVtxs = new TVec3f[mNumVtxs * 256];
        mSurfaces = new Surface[mNumSurfaces * 256];
        mLodSurfaces = new Surface[(u16)(mNumSurfaces * 64)];  // FIXME: incorrect masking in clrlslwi
        mVtxOverlap = new s16[mNumVtxs * 256];
        mTexST = new TVec2f[mNumVtxs * 256];  // FIXME: def ctor should not be called here

        mVtxs[0].set(-500.0f, -500.0f, -500.0f);
        mVtxs[1].set(-500.0f, -500.0f, 500.0f);
        mVtxs[2].set(500.0f, -500.0f, 500.0f);
        mVtxs[3].set(500.0f, -500.0f, -500.0f);
        mVtxs[4].set(-500.0f, 500.0f, -500.0f);
        mVtxs[5].set(-500.0f, 500.0f, 500.0f);
        mVtxs[6].set(500.0f, 500.0f, 500.0f);
        mVtxs[7].set(500.0f, 500.0f, -500.0f);

        mVtxs[8] = mVtxs[0];
        mVtxs[9] = mVtxs[3];
        mVtxs[10] = mVtxs[7];
        mVtxs[11] = mVtxs[4];

        mVtxs[12] = mVtxs[3];
        mVtxs[13] = mVtxs[2];
        mVtxs[14] = mVtxs[6];
        mVtxs[15] = mVtxs[7];

        mVtxs[16] = mVtxs[2];
        mVtxs[17] = mVtxs[1];
        mVtxs[18] = mVtxs[5];
        mVtxs[19] = mVtxs[6];

        mVtxs[20] = mVtxs[1];
        mVtxs[21] = mVtxs[0];
        mVtxs[22] = mVtxs[4];
        mVtxs[23] = mVtxs[5];

        mSurfaces[0].set(1, 0, 2);
        mSurfaces[1].set(2, 0, 3);
        mSurfaces[2].set(18, 17, 16);
        mSurfaces[3].set(18, 16, 19);
        mSurfaces[4].set(21, 23, 22);
        mSurfaces[5].set(21, 20, 23);
        mSurfaces[6].set(13, 12, 15);
        mSurfaces[7].set(14, 13, 15);
        mSurfaces[8].set(10, 9, 11);
        mSurfaces[9].set(11, 9, 8);
        mSurfaces[10].set(4, 5, 6);
        mSurfaces[11].set(4, 6, 7);

        for (u32 idx = 0; idx < 24; idx += 4) {
            mTexST[idx + 0].set(0.0f, 0.0f);
            mTexST[idx + 1].set(1.0f, 0.0f);
            mTexST[idx + 2].set(1.0f, 1.0f);
            mTexST[idx + 3].set(0.0f, 1.0f);
        }

        for (u32 idx = 0; idx < 3; idx++) {
            divide();
            if (idx == 1) {
                for (u32 surface = 0; surface < mNumSurfaces; surface++) {
                    mLodSurfaces[surface] = mSurfaces[surface];
                }
            }
        }

        for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
            mVtxOverlap[vtx] = -1;
        }

        for (u32 vtxA = 0; vtxA < mNumVtxs; vtxA++) {
            for (u32 vtxB = vtxA + 1; vtxB < mNumVtxs; vtxB++) {
                if ((mVtxs[vtxA] - mVtxs[vtxB]).length() < 1.0f) {
                    if (mVtxOverlap[vtxA] != -1) {
                        mVtxOverlap[vtxB] = mVtxOverlap[vtxA];
                        mVtxOverlap[vtxA] = vtxB;
                    } else {
                        mVtxOverlap[vtxA] = vtxB;
                        mVtxOverlap[vtxB] = vtxA;
                    }
                    break;
                }
            }

            if (mVtxOverlap[vtxA] == -1 && mTexST[vtxA].x != 0.0f && mTexST[vtxA].y != 0.0f && mTexST[vtxA].x != 1.0f) {
                // NOTE: this is a dummy to emit the above structure
                f32 f1 = 0.0f;
            }
        }

        toSphere(scale);

        TrampleStarShare* shareStar = new TrampleStarShare(this);
    }

    _A8 = new u8[mNumVtxs];
    mNumDrawVtxs = 0;
    mDrawPos = new TVec3f[mNumSurfaces * 3];
    mDrawNorm = new TVec3f[mNumSurfaces * 3];
    mDrawTex = new TVec2f[mNumSurfaces * 3];  // FIXME: def ctor should not be called here
    mDrawDeformCoeffs = new f32[mNumSurfaces * 3];

    for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
        mDeformCoeff[vtx] = 0.0f;
        mDrawDeformCoeffs[vtx] = -0.1f;
    }

    calcSurface(false);
    mIsLod = false;

    initNerve(&TrampleStarNrvWait::sInstance);
    MR::initLightCtrlNoDrawMapObj(this);
    makeActorAppeared();
    MR::setClippingTypeSphere(this, scale);
    MR::setClippingFarMax(this);
}

void TrampleStar::toSphere(f32 radius) {
    for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
        mVtxs[vtx].setLength(radius);
    }
}

void TrampleStar::initCheckFlag(u32 numFlags) {
    mCheckFlags = new (-0x4) u16[numFlags * numFlags];
    mNumFlags = numFlags;
    for (u32 idx = 0; idx < mNumFlags * mNumFlags; idx++) {
        mCheckFlags[idx] = 0;
    }
}

u16 TrampleStar::readCheckFlag(u16 vtxA, u16 vtxB) {
    return mCheckFlags[vtxA + vtxB * mNumFlags];
}

void TrampleStar::writeCheckFlag(u16 vtxA, u16 vtxB, u16 value) {
    mCheckFlags[vtxA + vtxB * mNumFlags] = value;
    mCheckFlags[vtxB + vtxA * mNumFlags] = value;
}

void TrampleStar::divide() {
    // FIXME: TVec2f strikes again!
    // https://decomp.me/scratch/SCOGi

    initCheckFlag(mNumVtxs);
    u16 numSurfaces = mNumSurfaces;
    u16 numVtxs = mNumVtxs;

    u16 midpoints[3];
    for (u32 surface = 0; surface < mNumSurfaces; surface++) {
        for (u32 idx = 0; idx < 3; idx++) {
            u16 vtx1 = mSurfaces[surface].mVtxs[idx];
            u16 vtx2 = mSurfaces[surface].mVtxs[(idx + 1) % 3];

            if (readCheckFlag(vtx1, vtx2) != 0) {
                midpoints[idx] = readCheckFlag(vtx1, vtx2);
            } else {
                mVtxs[numVtxs] = mVtxs[vtx1].translate(mVtxs[vtx2]).scaleInline(0.5f);

                // FIXME: TVec2 shenanigans
                TVec2f midpoint = mTexST[vtx1].addInline(mTexST[vtx2]).scaleInline(0.5f);
                mTexST[numVtxs] = midpoint;

                writeCheckFlag(vtx1, vtx2, numVtxs);
                midpoints[idx] = numVtxs;
                numVtxs++;
            }
        }

        mSurfaces[numSurfaces].set(midpoints[0], mSurfaces[surface].mVtxs[1], midpoints[1]);
        numSurfaces++;
        mSurfaces[numSurfaces].set(midpoints[0], midpoints[1], midpoints[2]);
        numSurfaces++;
        mSurfaces[numSurfaces].set(midpoints[1], mSurfaces[surface].mVtxs[2], midpoints[2]);
        numSurfaces++;
        mSurfaces[surface].set(mSurfaces[surface].mVtxs[0], midpoints[0], midpoints[2]);
    }

    mNumSurfaces = numSurfaces;
    mNumVtxs = numVtxs;
    delete mCheckFlags;
}

void TrampleStar::moveVtx(f32 deformRate, u32 vtxIdx, u32 /* unused */, f32 /* unused */, u32 /* unused */) {
    TVec3f vtxPos(mVtxs[vtxIdx]);

    for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
        f32 dist = (mVtxs[vtx] - vtxPos).length();
        if (!(dist >= 400.0f)) {
            f32 t = (400.0f - dist) / 400.0f;
            mDeformCoeff[vtx] += deformRate * (t * t);
        }
    }
}

void TrampleStar::control() {
    if (MR::calcDistanceToPlayer(this) >= 5000.0f) {
        onLod();
    } else {
        offLod();
    }
}

void TrampleStar::calcAnim() {
    calcSurface(false);
    if (!isNerve(&TrampleStarNrvWait::sInstance)) {
        for (u32 idx = 0; idx < mNumVtxs; idx++) {
            s16 idx2 = mVtxOverlap[idx];
            if (idx2 != -1) {
                f32 coeff = (mDeformCoeff[idx] + mDeformCoeff[idx2]) * 0.5f;
                mDeformCoeff[idx] = coeff;
                mDeformCoeff[mVtxOverlap[idx]] = coeff;
                f32 speed = (mDeformSpeed[idx] + mDeformSpeed[mVtxOverlap[idx]]) * 0.5f;
                mDeformSpeed[idx] = speed;
                mDeformSpeed[mVtxOverlap[idx]] = speed;
            }
        }
    }
    LiveActor::calcAnim();
}

void TrampleStar::calcSurface(bool calcDrawBuffers) {
    if (mIsLod) {
        return;
    }

    mNumDrawVtxs = 0;
    for (u32 surface = 0; surface < mNumSurfaces; surface++) {
        for (u32 idx = 0; idx < 3; idx++) {
            f32 currDeform = mDrawDeformCoeffs[mNumDrawVtxs];
            mDrawDeformCoeffs[mNumDrawVtxs] = mDeformCoeff[mSurfaces[surface].mVtxs[idx]];
            if (!calcDrawBuffers && currDeform == mDeformCoeff[mSurfaces[surface].mVtxs[idx]]) {
                mNumDrawVtxs++;
            } else {
                u32 vtx = mSurfaces[surface].mVtxs[idx];
                TVec3f pos = mVtxs[vtx];
                TVec3f up = pos;
                MR::normalizeOrZero(&up);
                TVec2f texST = mTexST[vtx];

                pos += mPosition - up.scaleInline(mDeformCoeff[vtx]);
                mDrawPos[mNumDrawVtxs] = pos;
                mDrawNorm[mNumDrawVtxs] = up;
                mDrawTex[mNumDrawVtxs] = texST;
                mNumDrawVtxs++;
            }
        }
    }

    DCStoreRange(mDrawPos, mNumDrawVtxs * 4 * 3);
    DCStoreRange(mDrawNorm, mNumDrawVtxs * 4 * 3);
    DCStoreRange(mDrawTex, mNumDrawVtxs * 4 * 2);
}

void TrampleStar::calcLodSurface() {
    mNumDrawVtxs = 0;

    for (u32 surface = 0; surface < mNumSurfaces / 4; surface++) {
        for (u32 idx = 0; idx < 3; idx++) {
            u32 vtx = mLodSurfaces[surface].mVtxs[idx];
            TVec3f pos = mVtxs[vtx];
            TVec3f up = pos;
            MR::normalizeOrZero(&up);
            TVec2f& texST = mTexST[vtx];

            pos += mPosition;
            mDrawPos[mNumDrawVtxs] = pos;
            mDrawNorm[mNumDrawVtxs] = up;
            mDrawTex[mNumDrawVtxs] = texST;
            mNumDrawVtxs++;
        }
    }

    DCStoreRange(mDrawPos, mNumDrawVtxs * 4 * 3);
    DCStoreRange(mDrawNorm, mNumDrawVtxs * 4 * 3);
    DCStoreRange(mDrawTex, mNumDrawVtxs * 4 * 2);
}

void TrampleStar::onLod() {
    if (!mIsLod) {
        calcLodSurface();
        mIsLod = true;
    }
}

void TrampleStar::offLod() {
    if (mIsLod) {
        mIsLod = false;

        for (u32 idx = 0; idx < mNumVtxs; idx++) {
            mDeformCoeff[idx] = 0.0f;
            mDrawDeformCoeffs[idx] = -0.1f;
        }

        calcSurface(true);
    }
}

void TrampleStar::exeWait() {
}

void TrampleStar::exeBindingCapture() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("Wait", static_cast< const char* >(nullptr));
        MR::startSoundPlayer("SE_OJ_TRAMPOLINE_LAND_L", -1);
    }

    TVec3f down = -mVtxs[mCaptureVtx];
    MR::normalizeOrZero(&down);
    moveVtx(mDeformRate, mCaptureVtx, 8, 0.8f, 0);
    mDeformRate = 0.85f * mDeformRate;

    if (getNerveStep() > mCaptureFrames) {
        setNerve(&TrampleStarNrvBindingShoot::sInstance);
    }
}

void TrampleStar::exeBindingShoot() {
    if (MR::isFirstStep(this)) {
        mDeformRate = 0.0f;
        mSpringRate = 0.1f;

        for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
            s16 vtx2 = mVtxOverlap[vtx];
            if (vtx2 != -1) {
                f32 coeff = (mDeformCoeff[vtx] + mDeformCoeff[vtx2]) * 0.5f;
                mDeformCoeff[vtx] = coeff;
                mDeformCoeff[mVtxOverlap[vtx]] = coeff;
            }
        }

        for (u32 idx = 0; idx < mNumVtxs; idx++) {
            mDeformSpeed[idx] = mSpringRate * mDeformCoeff[idx];
        }
    }

    for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
        f32 coeff = mDeformCoeff[vtx];
        f32 speed = mDeformSpeed[vtx];
        f32 springRate = mSpringRate;
        mDeformCoeff[vtx] = coeff - speed;
        mDeformSpeed[vtx] += coeff * springRate;
        mDeformCoeff[vtx] *= 0.8f;

        if (vtx == mCaptureVtx) {
            if (coeff * mDeformCoeff[vtx] <= 0.0f) {
                mSpringRate = 0.2f;

                if (MR::isPlayerInBind()) {
                    TVec3f up = mVtxs[mCaptureVtx];
                    MR::normalizeOrZero(&up);

                    if (MR::getPlayerLevelA()) {
                        mJumpVel *= 1.5f;
                        MR::startSoundPlayer("SE_OJ_TRAMPOLINE_BOUND_L", -1);
                        MR::startSoundPlayer("SE_PV_JUMP_JOY", -1);
                    } else {
                        MR::startSoundPlayer("SE_OJ_TRAMPOLINE_BOUND_S", -1);
                        MR::startSoundPlayer("SE_PV_JUMP_M", -1);
                    }

                    MR::endBindAndPlayerJump(this, up.scaleInline(mJumpVel), 0);
                    MR::startBckPlayer("Jump2", static_cast< const char* >(nullptr));
                    setNerve(&TrampleStarNrvWait::sInstance);
                }
            }

            if (speed * mDeformSpeed[vtx] <= 0.0f) {
                mSpringRate = 0.1f;
            }
        }
    }

    if (getNerveStep() == 120) {
        if (MR::isPlayerInBind()) {
            TVec3f up = mVtxs[mCaptureVtx];
            MR::normalizeOrZero(&up);

            if (MR::getPlayerLevelA()) {
                mJumpVel *= 1.5f;
            }

            MR::endBindAndPlayerJump(this, up.scaleInline(mJumpVel), 0);
        }

        setNerve(&TrampleStarNrvWait::sInstance);

        for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
            mDeformSpeed[vtx] = 0.0f;
        }

        for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
            mDeformCoeff[vtx] = 0.0f;
        }
    }
}

u32 TrampleStar::calcNearestVtxIndex(const TVec3f& rPos) {
    f32 nearestDistance = 5000.0f;
    u32 nearestIndex = 0;

    for (u32 vtx = 0; vtx < mNumVtxs; vtx++) {
        f32 dist = (mVtxs[vtx].translate(mPosition) - rPos).length();
        if (dist < nearestDistance) {
            nearestDistance = dist;
            nearestIndex = vtx;
        }
    }

    return nearestIndex;
}

void TrampleStar::draw() const {
    if (!MR::isDead(this)) {
        TDDraw::setup(0, 1, 0);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        GXInvalidateVtxCache();
        drawSelf();
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
        MR::reinitGX();
        GXSetAlphaUpdate(GX_TRUE);
        GXSetDstAlpha(GX_TRUE, 0);
    }
}

bool TrampleStar::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_AUTORUSH_BEGIN) {
        mCaptureFrames = 10;
        mDeformRate = MR::getPlayerVelocity()->length();
        if (mDeformRate < 30.0f) {
            mDeformRate = 30.0f;
        }
        if (mDeformRate > 50.0f) {
            mDeformRate = 50.0f;
            mCaptureFrames = 15;
        }

        mJumpVel = mDeformRate;
        if (mDeformRate < 20.0f) {
            mJumpVel = 20.0f;
        }

        mCaptureVtx = calcNearestVtxIndex(pSender->mPosition);
        if (MR::getPlayerVelocity()->dot(mVtxs[mCaptureVtx]) > 0.0f) {
            return false;
        }
        setNerve(&TrampleStarNrvBindingCapture::sInstance);
        return true;
    }

    if (msg == ACTMES_UPDATE_BASEMTX) {
        TVec3f side;
        MR::getPlayerSideVec(&side);
        TVec3f capturePos = mVtxs[mCaptureVtx];
        TVec3f down = -capturePos;
        down.normalizePS();
        TPos3f mtx;
        capturePos += mPosition.translate(down.scaleInline(mDeformCoeff[mCaptureVtx]));
        MR::makeMtxUpSidePos(&mtx, -down, side, capturePos);
        MR::setPlayerBaseMtx(mtx);
        return true;
    }

    return false;
}

void TrampleStar::drawSelf() const {
    if (mNumDrawVtxs == 0) {
        return;
    }

    MR::loadActorLight(this);

    if (mTexture == nullptr) {
        return;
    }
    if (mTexST == nullptr) {
        return;
    }

    MR::ddSetVtxFormat(3);
    mTexture->load(GX_TEXMAP0);
    GXSetZCompLoc(GX_TRUE);
    const_cast< JUTTexture* >(MR::getMarioShadowTexForLoad())->load(GX_TEXMAP1);
    TVec3f playerPos = *MR::getPlayerPos();
    TVec3f diff = mPosition - playerPos;
    TVec3f shadowPos = MR::getMarioShadowVec();
    TPos3f mtx3;
    TPos3f mtx;
    TRot3f rot;
    TPos3f mtx2;

    if (MR::vecKillElement(diff, shadowPos, &diff) > 0.0f) {
        PSMTXTrans(mtx, -playerPos.x, -playerPos.y, -playerPos.z);
    } else {
        PSMTXTrans(mtx, 1000000.0f, 1000000.0f, 1000000.0f);
    }

    MR::makeMtxRotate(rot, -*MR::getPlayerRotate());

    PSMTXIdentity(mtx2);

    TVec3f front;
    MR::getPlayerFrontVec(&front);
    if (MR::isNearZero(front)) {
        return;
    }
    if (MR::isNearZero(MR::getMarioShadowVec())) {
        return;
    }
    if (MR::isSameDirection(MR::getMarioShadowVec(), front, 0.01f) || MR::isOppositeDirection(MR::getMarioShadowVec(), front, 0.01f)) {
        return;
    }

    MR::makeMtxUpFront(&mtx2, -MR::getMarioShadowVec(), -front);
    PSMTXInverse(mtx2, mtx2);

    PSMTXConcat(mtx2, mtx, mtx3);
    TPos3f mtx4;
    for (u32 idx = 0; idx < 12; idx++) {
        reinterpret_cast< f32* >(&mtx4)[idx] = 0;
    }
    mtx4[0][0] = debug1;
    mtx4[1][2] = debug2;
    mtx4[2][3] = debug3;
    mtx4[0][3] = debug4;
    mtx4[1][3] = debug5;
    PSMTXConcat(mtx4, mtx3, mtx3);

    GXLoadTexMtxImm(mtx3, GX_TEXMTX1, GX_MTX3x4);
    GXSetNumTevStages(3);
    GXSetNumTexGens(2);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetChanCtrl(GX_COLOR0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_ALPHA0, GX_TRUE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT2, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_SPOT);
    GXSetChanMatColor(GX_COLOR0A0, (GXColor)Color8(0xCC, 0xCC, 0xCC, 0xFF));
    GXSetChanAmbColor(GX_COLOR0A0, (GXColor)Color8(0x7F, 0x7F, 0x93, 0xC3));
    GXSetTevKColor(GX_KCOLOR0, (GXColor)Color8(0x00, 0x00, 0x00, 0xB4));
    GXSetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
    GXSetTevColor(GX_TEVREG0, (GXColor)Color8(0xFF, 0xFF, 0xFF, 0x00));
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_C0, GX_CC_RASC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_TEXC, GX_CC_APREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    if (MR::getPlayerShadowHeight() < MR::calcDistanceToPlayer(this)) {
        GXSetNumTevStages(2);
        GXSetNumTexGens(1);
    } else {
        GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX1, GX_FALSE, GX_PTIDENTITY);
        GXSetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE2, GX_CC_CPREV, GX_CC_ZERO, GX_CC_TEXC, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_APREV);
        GXSetTevAlphaOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }

    GXBegin(GX_TRIANGLES, GX_VTXFMT0, mNumDrawVtxs);
    TVec3f* pos = mDrawPos;
    TVec3f* norm = mDrawNorm;
    TVec2f* tex = mDrawTex;
    for (u32 vtx = 0; vtx < mNumDrawVtxs; vtx++) {
        MR::ddSendVtxData(*pos, *norm, *tex);
        pos++;
        norm++;
        tex++;
    }
}
