#include "Game/MapObj/SpiderThread.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/SpiderThreadHangInfo.hpp"
#include "Game/MapObj/SpiderThreadMainPoint.hpp"
#include "Game/MapObj/SpiderThreadPart.hpp"
#include "Game/MapObj/SpiderThreadRadialLine.hpp"
#include "Game/MapObj/SpiderThreadWindCtrl.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <revolution/gx/GXBump.h>

namespace {
    const CutPointEntry sCutPointNoTable[] = {{0, 3}, {1, 1}, {2, 0}, {4, 3}, {6, 1}, {6, 4}};
}

inline SpiderThread* getThreadObj() {
    return reinterpret_cast< SpiderThread* >(MR::getSceneObjHolder()->getObj(SceneObj_SpiderThread));
}

inline SpiderThreadPart* connectPoints(SpiderThreadPoint* pPointA, SpiderThreadPoint* pPointB) {
    SpiderThreadMainPoint* pointB = reinterpret_cast< SpiderThreadMainPoint* >(pPointB);
    SpiderThreadMainPoint* pointA = reinterpret_cast< SpiderThreadMainPoint* >(pPointA);

    SpiderThreadPart* part = new SpiderThreadPart(pointA, pointB, 100.0f);

    pointA->addNearMainPoint(pointB, part);
    pointB->addNearMainPoint(pointA, part);

    return part;
}

SpiderThread::SpiderThread(const char* pName)
    : LiveActor(pName), mMainPartsBufferSize(0), mMainPoint(nullptr), mHangInfos(nullptr), mNumHangInfos(0), mShootHangInfo(nullptr),
      mNumRadialLines(8), mRadialLines(nullptr), mAnchorPoints(nullptr), mNumMainParts(0), mMainParts(nullptr), mNumSubParts(0), mSubParts(nullptr),
      mCutPointsBufferSize(0), mNumCutPoints(0), mCutPoints(nullptr), mFront(0.0f, 0.0f, 1.0f), mWindCtrl(nullptr), mIsBloomOn(false),
      mThreadTexture(nullptr), mIndirectTexture(nullptr) {
    mMainPartsBufferSize = mNumRadialLines * 9;
    mSubPartsBufferSize = mNumRadialLines * 8;
    mMainParts = new SpiderThreadPart*[mMainPartsBufferSize];
    mSubParts = new SpiderThreadPart*[mSubPartsBufferSize];

    for (s32 idx = 0; idx < mMainPartsBufferSize; idx++) {
        mMainParts[idx] = nullptr;
    }

    for (s32 idx = 0; idx < mSubPartsBufferSize; idx++) {
        mSubParts[idx] = nullptr;
    }
}

void SpiderThread::init(const JMapInfoIter& rIter) {}

void SpiderThread::initThread(const TVec3f& rPos) {
    mPosition.set(rPos);

    MR::connectToScene(this, MR::MovementType_Ride, -1, -1, MR::DrawType_SpiderThread);

    mWindCtrl = new SpiderThreadWindCtrl();
    initMainThreads();
    MR::invalidateClipping(this);

    mThreadTexture = new JUTTexture(MR::loadTexFromArc("SpiderThread.arc", "SpiderThread.bti"), 0);
    mIndirectTexture = new JUTTexture(MR::loadTexFromArc("SpiderThread.arc", "Indirect.bti"), 0);

    mHangInfos = new SpiderThreadHangInfo*[64];
    for (s32 idx = 0; idx < 64; idx++) {
        mHangInfos[idx] = new SpiderThreadHangInfo();
    }

    MR::registerDemoSimpleCastAll(this);
    makeActorDead();
}

void SpiderThread::movement() {
    if (!MR::isValidMovement(this)) {
        return;
    }

    mWindCtrl->update();
    updatePointerInfo();
    updateHangedPoint();
    tryTouchPoint(WPAD_CHAN0);
    tryTouchPoint(WPAD_CHAN1);

    mMainPoint->updateVelocity();
    for (s32 idx = 0; idx < mNumRadialLines; idx++) {
        mRadialLines[idx]->updateVelocity();
    }
    mMainPoint->updatePos();
    mMainPoint->mBasePos.set(mMainPoint->mPosition);
    for (s32 idx = 0; idx < mNumRadialLines; idx++) {
        mRadialLines[idx]->updatePos();
    }

    for (s32 idx = 0; idx < mNumMainParts; idx++) {
        mMainParts[idx]->update();
    }

    for (s32 idx = 0; idx < mNumSubParts; idx++) {
        mSubParts[idx]->update();
    }

    for (s32 idx = 0; idx < mNumHangInfos; idx++) {
        if (!mHangInfos[idx]->mIsHanging && mHangInfos[idx]->mThreadPart->mIsStill) {
            mHangInfos[idx]->mThreadPart->updatePointPosAndBasePosToLine();
        }
    }
}

void SpiderThread::draw() const {
    if (MR::isValidDraw(this)) {
        initDraw();

        for (s32 idx = 0; idx < mNumMainParts; idx++) {
            mMainParts[idx]->draw();
        }

        for (s32 idx = 0; idx < mNumSubParts; idx++) {
            mSubParts[idx]->draw();
        }
    }
}

void SpiderThread::startActorBind(LiveActor* pActor, const TVec3f** pPointPos, const TVec3f** pNeutralPos, const TVec3f** pUp, const TVec3f& rPos,
                                  const TVec3f& rVel, s32 a1) {
    s32 nearestIndex = 0;
    s32 pointNum = 0;
    findNearestPointPos(pPointPos, pNeutralPos, &nearestIndex, &pointNum, rPos);

    SpiderThreadHangInfo* hangInfo = mHangInfos[mNumHangInfos];
    hangInfo->startBind(pActor, &pActor->mPosition, mSubParts[nearestIndex], pointNum, rVel, a1);
    *pUp = &hangInfo->mThreadPart->mUp;
    mNumHangInfos++;
}

void SpiderThread::touchActor(const TVec3f& rPos, const TVec3f& rVel) {
    const TVec3f* pointPos = nullptr;
    const TVec3f* neutralPos = nullptr;
    s32 nearestIndex = 0;
    s32 pointNum = 0;
    findNearestPointPos(&pointPos, &neutralPos, &nearestIndex, &pointNum, rPos);

    mSubParts[nearestIndex]->touchActor(pointNum, rVel);
}

void SpiderThread::tryPush(const TVec3f& rPos, f32 radius) {
    mMainPoint->tryPush(rPos, radius);
    for (s32 radialIdx = 0; radialIdx < mNumRadialLines; radialIdx++) {
        for (s32 idx = 0; idx < mRadialLines[radialIdx]->mNumPoints; idx++) {
            mRadialLines[radialIdx]->mPoints[idx]->tryPush(rPos, radius);
        }
    }
}

void SpiderThread::stopAllPartsPoint() {
    for (s32 idx = 0; idx < mNumSubParts; idx++) {
        mSubParts[idx]->stopAllPoints();
    }
}

void SpiderThread::cutSpiderThread() {
    mMainPoint->cutNearPoints(&mNumCutPoints, mCutPoints);

    for (s32 idx = 0; idx < static_cast< s32 >(sizeof(sCutPointNoTable) / sizeof(*sCutPointNoTable)); idx++) {
        mRadialLines[sCutPointNoTable[idx].mRadialIdx]->mPoints[sCutPointNoTable[idx].mPointIdx]->cutNearPoints(&mNumCutPoints, mCutPoints);
    }
}

void SpiderThread::initMainThreads() {
    TRot3f rotMtx;
    MR::makeMtxRotate(rotMtx.toMtxPtr(), 0.0f, 0.0f, 360.0f / mNumRadialLines);
    TVec3f down(0.0f, -1.0f, 0.0f);

    mMainPoint = new SpiderThreadMainPoint(mPosition, mNumRadialLines + 1);

    mAnchorPoints = new SpiderThreadMainPoint*[mNumRadialLines];
    for (s32 idx = 0; idx < mNumRadialLines; idx++) {
        mAnchorPoints[idx] = nullptr;
    }

    mCutPointsBufferSize = mNumRadialLines + 24;
    mCutPoints = new SpiderThreadMainPoint*[mCutPointsBufferSize];
    for (s32 idx = 0; idx < mCutPointsBufferSize; idx++) {
        mCutPoints[idx] = new SpiderThreadMainPoint(mPosition, 5);
    }

    mRadialLines = new SpiderThreadRadialLine*[mNumRadialLines];
    for (s32 radialIdx = 0; radialIdx < mNumRadialLines; radialIdx++) {
        mRadialLines[radialIdx] = new SpiderThreadRadialLine(8);

        // main axial points
        for (s32 idx = 0; idx < 8; idx++) {
            TVec3f pos(down);
            pos.scale(175.0f * (idx + 1));
            pos.add(mMainPoint->mPosition);

            SpiderThreadMainPoint* point = new SpiderThreadMainPoint(pos, 5);
            mRadialLines[radialIdx]->addPoint(point);
            point->mRadialLine = mRadialLines[radialIdx];
        }

        // anchor point
        TVec3f pos(down);
        pos.scale(1800.0f);
        pos.add(mMainPoint->mPosition);
        mAnchorPoints[radialIdx] = new SpiderThreadMainPoint(pos, 5);
        mAnchorPoints[radialIdx]->mRadialLine = mRadialLines[radialIdx];

        // connect points
        SpiderThreadMainPoint* point = mRadialLines[radialIdx]->mPoints[0];
        SpiderThreadMainPoint* nextPoint;
        connectMainPoint(point, mMainPoint, true);
        for (s32 idx = 0; idx < 8 - 1; idx++) {
            nextPoint = mRadialLines[radialIdx]->mPoints[idx + 1];
            connectMainPoint(point, nextPoint, true);
            point = nextPoint;
        }
        connectMainPoint(nextPoint, mAnchorPoints[radialIdx], true);

        rotMtx.mult(down, down);
    }

    for (s32 radialIdx = 0; radialIdx < mNumRadialLines; radialIdx++) {
        for (s32 idx = 0; idx < 8; idx++) {
            connectMainPoint(mRadialLines[radialIdx]->mPoints[idx], mRadialLines[(radialIdx + mNumRadialLines + 1) % mNumRadialLines]->mPoints[idx],
                             false);
        }
    }
}

void SpiderThread::connectMainPoint(SpiderThreadPoint* pPointA, SpiderThreadPoint* pPointB, bool isMainPart) {
    SpiderThreadPart* part = connectPoints(pPointA, pPointB);

    if (isMainPart) {
        mMainParts[mNumMainParts] = part;
        mNumMainParts++;
    } else {
        mSubParts[mNumSubParts] = part;
        mNumSubParts++;
    }
}

bool SpiderThread::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_SLING_SHOOT_ACTOR_HANG_START) {
        SpiderThreadHangInfo* hangInfo = findHangInfo(pSender->mHost);
        if (mShootHangInfo != nullptr) {
            if (mShootHangInfo->_14 < hangInfo->_14) {
                mShootHangInfo->endHang(true);
            } else {
                return false;
            }
        }
        mShootHangInfo = hangInfo;
        mShootHangInfo->startHang();
        return true;
    }

    if (msg == ACTMES_SLING_SHOOT_ACTOR_HANG_END) {
        mShootHangInfo->endHang(false);
        mShootHangInfo = nullptr;
        return true;
    }

    if (msg == ACTMES_SLING_SHOOT_ACTOR_APART) {
        SpiderThreadHangInfo* hangInfo = findHangInfo(pSender->mHost);
        hangInfo->endBind();
        if (mNumHangInfos > 1) {
            s32 lastInfoIndex = mNumHangInfos - 1;
            hangInfo->copyInfo(mHangInfos[lastInfoIndex]);
        }
        mShootHangInfo = nullptr;
        mNumHangInfos--;
        return true;
    }

    return false;
}

void SpiderThread::updatePointerInfo() {
    for (s32 idx = 0; idx < mNumSubParts; idx++) {
        mSubParts[idx]->updatePointerPos();
    }
}

void SpiderThread::updateHangedPoint() {
    for (s32 idx = 0; idx < mNumHangInfos; idx++) {
        if (mHangInfos[idx]->mIsHanging) {
            mHangInfos[idx]->updateHang();
            return;
        }
    }
}

void SpiderThread::tryTouchPoint(s32 padChannel) {
    if (!MR::isStarPointerInScreen(padChannel) || MR::getStarPointerScreenSpeed(padChannel) < 5.0f) {
        return;
    }

    TVec3f vel(0.0f, 0.0f, 0.0f);
    MR::calcStarPointerWorldVelocityDirectionOnPlane(&vel, mMainPoint->mPosition, mFront, padChannel);

    f32 speed = MR::getStarPointerScreenSpeed(padChannel) * 2.0f;
    speed = speed >= 50.0f ? 50.0f : speed;

    vel.scale(speed);

    for (s32 idx = 0; idx < mNumSubParts; idx++) {
        mSubParts[idx]->tryTouch(100.0f, vel, padChannel);
    }
}

void SpiderThread::findNearestPointPos(const TVec3f** pPointPos, const TVec3f** pNeutralPos, s32* nearestIndex, s32* pointNum,
                                       const TVec3f& rPos) const {
    TVec3f pos(rPos);
    f32 minDistance = 1800.0f;
    *pPointPos = nullptr;
    *pNeutralPos = nullptr;

    for (s32 idx = 0; idx < mNumSubParts; idx++) {
        const TVec3f* pointPos = nullptr;
        const TVec3f* neutralPos = nullptr;
        s32 pointNo = -1;
        f32 dist = mSubParts[idx]->calcNearestPointInfo(&pointPos, &neutralPos, &pointNo, pos);

        if (dist < minDistance) {
            *pPointPos = pointPos;
            *pNeutralPos = neutralPos;
            *nearestIndex = idx;
            *pointNum = pointNo;
            minDistance = dist;
        }
    }
}

SpiderThreadHangInfo* SpiderThread::findHangInfo(LiveActor* pActor) const {
    for (s32 idx = 0; idx < mNumHangInfos; idx++) {
        if (mHangInfos[idx]->mBindedActor == pActor) {
            return mHangInfos[idx];
        }
    }

    return mHangInfos[0];
}

void SpiderThread::initDraw() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_POS_XYZ, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_POS_XYZ, GX_F32, 0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);

    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);

    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetNumTexGens(2);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, 60, GX_FALSE, 125);
    mThreadTexture->load(GX_TEXMAP0);
    mIndirectTexture->load(GX_TEXMAP1);

    GXSetNumIndStages(1);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD1, GX_TEXMAP1);
    GXSetTevIndWarp(GX_TEVSTAGE0, GX_INDTEXSTAGE0, GX_TRUE, GX_FALSE, GX_ITM_0);

    Mtx23 mtx;
    mtx[0][0] = 0.035f;
    mtx[0][1] = 0.0;
    mtx[0][2] = 0.0;
    mtx[1][0] = 0.0;
    mtx[1][1] = 0.03f;
    mtx[1][2] = 0.0;
    GXSetIndTexMtx(GX_ITM_0, mtx, 0);

    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_FALSE, GX_TEVPREV);

    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);

    GXSetZMode(GX_ENABLE, GX_LEQUAL, GX_DISABLE);
    GXSetZCompLoc(GX_DISABLE);
    GXSetCullMode(GX_CULL_NONE);
}

void MR::initSpiderThread(const TVec3f& rPos) {
    getThreadObj()->initThread(rPos);
}

void MR::appearSpiderThread() {
    getThreadObj()->appear();
}

void MR::startSpiderThreadBattle() {
    getThreadObj()->stopAllPartsPoint();
    getThreadObj()->mWindCtrl->startWindBattle();
}

void MR::startSpiderThreadChance() {
    getThreadObj()->stopAllPartsPoint();
    getThreadObj()->mWindCtrl->startWindChance();
}

void MR::startSpiderThreadBattleEnd() {
    getThreadObj()->stopAllPartsPoint();
    getThreadObj()->cutSpiderThread();
    getThreadObj()->mWindCtrl->startWindBattleEnd();
}

void MR::pauseOffSpiderThread() {
    MR::requestMovementOn(getThreadObj());
}

f32 MR::getSpiderThreadPosZ() {
    if (!MR::isExistSceneObj(SceneObj_SpiderThread)) {
        return 0.0f;
    }

    return getThreadObj()->mPosition.z;
}

void MR::startActorBindToSpiderThread(LiveActor* pActor, const TVec3f** pPointPos, const TVec3f** pNeutralPos, const TVec3f** pUp, const TVec3f& rPos,
                                      const TVec3f& rVel) {
    getThreadObj()->startActorBind(pActor, pPointPos, pNeutralPos, pUp, rPos, rVel, 1);
}

void MR::sendMsgToSpiderThread(u32 msg, HitSensor* pSender) {
    getThreadObj()->receiveMessage(msg, pSender, MR::getMessageSensor());
}

void MR::touchActorToSpiderThread(const TVec3f& rPos, const TVec3f& rVel) {
    getThreadObj()->touchActor(rPos, rVel);
}

void MR::tryPushSpiderThread(const TVec3f& rPos, f32 radius) {
    getThreadObj()->tryPush(rPos, radius);
}

void MR::onSpiderThreadBloom() {
    getThreadObj()->mIsBloomOn = true;
}

void MR::drawSpiderThreadBloom() {
    if (!MR::isExistSceneObj(SceneObj_SpiderThread)) {
        return;
    }

    if (getThreadObj()->mIsBloomOn) {
        getThreadObj()->draw();
        CategoryList::drawOpa(MR::DrawBufferType_Ride);
    }
}
