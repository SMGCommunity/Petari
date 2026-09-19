#include "Game/System/ShapePacketUserData.hpp"
#include "Game/System/WPad.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/J3DGraphBase/J3DFifo.hpp>
#include <JSystem/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3DGraphBase/J3DShapeMtx.hpp>
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JAudio2/JASAramStream.hpp>
#include <JSystem/JAudio2/JASDriverIF.hpp>
#include <JSystem/JAudio2/JASDvdThread.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JAudio2/JASTrack.hpp>
#include <JSystem/JAudio2/JASWaveArcLoader.hpp>
#include <JSystem/JAudio2/JAUInitializer.hpp>
#include <JSystem/JKernel/JKRAram.hpp>
#include <JSystem/JKernel/JKRAramPiece.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <JSystem/JKernel/JKRThread.hpp>
#include <JSystem/JKernel/JKRUnitHeap.hpp>
#include <JSystem/JParticle/JPABaseShape.hpp>
#include <JSystem/JParticle/JPAEmitter.hpp>
#include <JSystem/JParticle/JPAEmitterManager.hpp>
#include <JSystem/JParticle/JPAFieldBlock.hpp>
#include <JSystem/JParticle/JPAParticle.hpp>
#include <JSystem/JSupport/JSUInputStream.hpp>
#include <JSystem/JSupport/JSUOutputStream.hpp>
#include <JSystem/JUtility/JUTConsole.hpp>
#include <JSystem/JUtility/JUTException.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <JSystem/JAudio2/JASAudioThread.hpp>
#include <cstdarg>
#include <revolution/sc.h>

void Overwrite_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.0000038146973f;
    (void)0.5f;
    (void)-2607.5945f;
    (void)2607.5945f;
    (void)0.333333f;
    (void)0.57735f;
    (void)0.001f;
    (void)25.0f;
    (void)0.01f;
}

namespace {
    const u8 sUnitMask[] = {0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1};
}

void* JKRUnitHeap::do_alloc(u32 size, int alignment) {
    u32 bit;
    u8* pBat;
    s32 index;
    lock();
    void* pResult = nullptr;

    if (size <= mUnitSize) {
        index = find1FreeBlock(alignment);
        if (index >= 0) {
            setUnitUsed(index);
            mTotalFreeSize -= mUnitSize;
            pResult = indexToAddress(index);
        }
    } else {
        u32 count = (size + mUnitSize - 1) / mUnitSize;
        index = findFreeBlock(alignment, count);
        if (index >= 0) {
            pBat = mBat + index / 8;
            for (s32 i = index; i < index + count; i++) {
                bit = i & 7;
                if (bit == 0) {
                    pBat = mBat + i / 8;
                }

                *pBat |= sUnitMask[bit];
            }

            mTotalFreeSize -= mUnitSize * count;
            pResult = indexToAddress(index);
        }
    }

    unlock();
    return pResult;
}

void JKRUnitHeap::do_free(void* pMemory) {
    lock();
    s32 index = addressToIndex(pMemory);
    if (index >= 0) {
        u32 value = mBat[index / 8];
        u32 cleared = value & (sUnitMask[index & 7] ^ 0xFF);
        mBat[index / 8] = cleared;
        if (value != cleared) {
            mTotalFreeSize += mUnitSize;
        }
    }

    unlock();
}

void JKRSolidHeap::do_free(void* pMemory) {
}

s32 JKRExpHeap::adjustSize() {
    CMemBlock* pNext;
    JKRHeap* pParent = mChildTree.getParent()->getObject();
    if (pParent == nullptr) {
        return -1;
    }

    lock();
    u8* pEnd = mStart;
    for (CMemBlock* pBlock = mHeadUsedList; pBlock != nullptr; pBlock = pBlock->mNext) {
        u8* pBlockEnd = reinterpret_cast< u8* >(pBlock + 1) + pBlock->mSize;
        if (pBlockEnd > pEnd) {
            pEnd = pBlockEnd;
        }
    }

    if (pEnd == mEnd) {
        unlock();
        return -1;
    }

    if (pParent->getHeapType() != 'EXPH') {
        unlock();
        return -1;
    }

    for (CMemBlock* pBlock = mHeadFreeList; pBlock != nullptr; pBlock = pNext) {
        pNext = pBlock->mNext;
        if (reinterpret_cast< u8* >(pBlock) >= pEnd) {
            if (pNext != nullptr) {
                pNext = pNext->mNext;
            }

            removeFreeBlock(pBlock);
        }
    }

    if (mHeadFreeList == nullptr) {
        CMemBlock* pBlock = reinterpret_cast< CMemBlock* >(pEnd);
        pBlock->initiate(nullptr, nullptr, 0, 0, 0);
        mHeadFreeList = pBlock;
        mTailFreeList = pBlock;
        pEnd += sizeof(CMemBlock);
    }

    u32 size = pEnd - reinterpret_cast< u8* >(this);
    pParent->resize(this, size);
    mEnd = pEnd;
    mSize = pEnd - mStart;
    unlock();
    return size;
}

u32 JKRHeap::getMaxAllocatableSize(int alignment) {
    u32 address = reinterpret_cast< u32 >(getMaxFreeBlock());
    return ~(alignment - 1) & (getFreeSize() - ((alignment - 1) & (alignment - (address & 0xF))));
}

extern "C" void JUTWarningConsole_f_va(const char*, va_list);

extern "C" void JUTWarningConsole_f(const char* pFormat, ...) {
    va_list args;
    va_start(args, pFormat);
    JUTWarningConsole_f_va(pFormat, args);
    va_end(args);
}

extern "C" void JUTWarningConsole(const char* pMessage) {
    JUTWarningConsole_f("%s", pMessage);
}

void JUTTexture::captureDolTexture(void* pBuffer, int width, int height, int x, int y, bool mipmap, GXTexFmt format) {
    if (mipmap) {
        GXSetTexCopySrc(x, y, width * 2, height * 2);
    } else {
        GXSetTexCopySrc(x, y, width, height);
    }

    GXSetTexCopyDst(width, height, format, mipmap);
    GXCopyTex(pBuffer, GX_FALSE);
    GXPixModeSync();
}

JSUOutputStream::~JSUOutputStream() {
}

JSUInputStream::~JSUInputStream() {
}

extern "C" void PSMTXMultVecArraySR(const Mtx, const Vec*, Vec*, u32);

extern "C" void PSMTXRotTrig(Mtx, char, f32, f32);

extern "C" void PSMTXRotRad(Mtx pMtx, char axis, f32 radians) {
    f32 sin = JMASinRadian(radians);
    f32 cos = JMACosRadian(radians);
    PSMTXRotTrig(pMtx, axis, sin, cos);
}

bool JUTException::readPad(u32* pTrigger, u32* pHold) {
    OSTime startTime = OSGetTime();
    OSTime elapsed;

    do {
        elapsed = OSTicksToMilliseconds(OSGetTime() - startTime);
    } while (elapsed < 50);

    *pHold = 0;
    *pTrigger = 0;

    MR::getPadDataForExceptionNoInit(WPAD_CHAN0, pHold, pTrigger);
    MR::getPadDataForExceptionNoInit(WPAD_CHAN1, pHold, pTrigger);
    MR::getPadDataForExceptionNoInit(WPAD_CHAN2, pHold, pTrigger);
    MR::getPadDataForExceptionNoInit(WPAD_CHAN3, pHold, pTrigger);

    return true;
}

bool JUTException::queryMapAddress_single(char* pMap, u32 address, s32 section, u32* pAddress, u32* pSize, char* pLine, u32 length, bool print,
                                          bool newline) {
    return false;
}

void J3DShapeMtx::loadMtxIndx_PNGP(int slot, u16 index) const {
    J3DFifoLoadIndx(0x20, index, 0xB000 | static_cast< u16 >(slot * 12));
    J3DFifoLoadNrmMtxIndx3x3(index, slot * 3);
    ShapePacketUserData* pData = MR::getJ3DShapePacketUserData(j3dSys.getShapePacket());
    if (pData != nullptr) {
        pData->loadTexMtx(j3dSys.getShapePacket()->getShape()->getMaterial(), slot, index);
    }
}

namespace {
    static void noLoadPrj(JPAEmitterWorkData const* pWork, const Mtx pSrt) {
    }

    void loadPrj(JPAEmitterWorkData const* pWork, const Mtx pSrt) {
        Mtx mtx;
        PSMTXConcat(pWork->mPrjMtx, pSrt, mtx);
        GXLoadTexMtxImm(mtx, GX_TEXMTX0, GX_MTX3x4);
    }

    static void loadPrjAnm(JPAEmitterWorkData const* pWork, const Mtx pSrt) {
        f32 frame = pWork->mpEmtr->getAge();
        const JPABaseShape* pShape = pWork->mpRes->getBsp();
        f32 pivotS = 0.5f * (1.0f + pShape->getTilingS());
        f32 pivotT = 0.5f * (1.0f + pShape->getTilingT());
        f32 translationS = (frame * pShape->getIncTransX()) + pShape->getInitTransX();
        f32 translationT = (frame * pShape->getIncTransY()) + pShape->getInitTransY();
        f32 scaleS = (frame * pShape->getIncScaleX()) + pShape->getInitScaleX();
        f32 scaleT = (frame * pShape->getIncScaleY()) + pShape->getInitScaleY();
        s32 angle = (frame * pShape->getIncRot()) + pShape->getInitRot();
        f32 sin = JMASSin(angle);
        f32 cos = JMASCos(angle);
        Mtx matrix;
        matrix[0][0] = scaleS * cos;
        matrix[0][1] = -scaleS * sin;
        matrix[0][2] = (pivotS + (scaleS * ((sin * (pivotT + translationT)) - (cos * (pivotS + translationS)))));
        matrix[0][3] = 0.0f;
        matrix[1][0] = scaleT * sin;
        matrix[1][1] = scaleT * cos;
        matrix[1][2] = (pivotT + (-scaleT * ((sin * (pivotS + translationS)) + (cos * (pivotT + translationT)))));
        matrix[1][3] = 0.0f;
        matrix[2][0] = 0.0f;
        matrix[2][1] = 0.0f;
        matrix[2][2] = 1.0f;
        matrix[2][3] = 0.0f;
        PSMTXConcat(matrix, pWork->mPrjMtx, matrix);
        PSMTXConcat(matrix, pSrt, matrix);
        GXLoadTexMtxImm(matrix, 0x1e, GX_MTX3x4);
    }

    static u8 jpa_dl[32] ATTRIBUTE_ALIGN(32) = {
        0x80, 0x00, 0x04, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    static u8 jpa_dl_x[32] ATTRIBUTE_ALIGN(32) = {
        0x80, 0x00, 0x08, 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x48, 0x00, 0x49, 0x01, 0x4A,
        0x02, 0x4B, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    static u8* p_dl[2] = {
        jpa_dl,
        jpa_dl_x,
    };

    typedef void (*projectionFunc)(JPAEmitterWorkData const*, const Mtx);

    static projectionFunc p_prj[3] = {
        noLoadPrj,
        loadPrj,
        loadPrjAnm,
    };

    JPANode< JPABaseParticle >* getNext(JPANode< JPABaseParticle >* pNode) {
        return pNode->getNext();
    }
    JPANode< JPABaseParticle >* getPrev(JPANode< JPABaseParticle >* pNode) {
        return pNode->getPrev();
    }
    void dirTypeVel(JPAEmitterWorkData const* pWork, JPABaseParticle const* pParticle, JGeometry::TVec3< f32 >* pDirection) {
        pParticle->getVelVec(*pDirection);
    }

    void dirTypePos(JPAEmitterWorkData const* pWork, JPABaseParticle const* pParticle, JGeometry::TVec3< f32 >* pDirection) {
        pParticle->getLocalPosition(*pDirection);
    }

    void dirTypePosInv(JPAEmitterWorkData const* pWork, JPABaseParticle const* pParticle, JGeometry::TVec3< f32 >* pDirection) {
        pParticle->getLocalPosition(*pDirection);
        pDirection->negate();
    }

    void dirTypeEmtrDir(JPAEmitterWorkData const* pWork, JPABaseParticle const* pParticle, JGeometry::TVec3< f32 >* pDirection) {
        pDirection->set(pWork->mGlobalEmtrDir);
    }

    void dirTypePrevPtcl(JPAEmitterWorkData const* pWork, JPABaseParticle const* pParticle, JGeometry::TVec3< f32 >* pDirection) {
        JGeometry::TVec3< f32 > position;
        pParticle->getGlobalPosition(position);
        JPANode< JPABaseParticle >* pEnd = pWork->mpAlivePtcl->getEnd();
        JPANode< JPABaseParticle >* pPrev = pWork->mpCurNode->getPrev();

        if (pEnd != pPrev) {
            JPABaseParticle* pParticle = pWork->mpCurNode->getPrev()->getObject();
            pParticle->getGlobalPosition(*pDirection);
        } else {
            pWork->mpEmtr->calcEmitterGlobalPosition(pDirection);
        }

        pDirection->sub(position);
    }

    static void rotTypeY(f32 sin, f32 cos, Mtx& rMtx) {
        rMtx[0][0] = cos;
        rMtx[0][1] = 0.0f;
        rMtx[0][2] = -sin;
        rMtx[0][3] = 0.0f;
        rMtx[1][0] = 0.0f;
        rMtx[1][1] = 1.0f;
        rMtx[1][2] = 0.0f;
        rMtx[1][3] = 0.0f;
        rMtx[2][0] = sin;
        rMtx[2][1] = 0.0f;
        rMtx[2][2] = cos;
        rMtx[2][3] = 0.0f;
    }

    static void rotTypeX(f32 sin, f32 cos, Mtx& rMtx) {
        rMtx[0][0] = 1.0f;
        rMtx[0][1] = 0.0f;
        rMtx[0][2] = 0.0f;
        rMtx[0][3] = 0.0f;
        rMtx[1][0] = 0.0f;
        rMtx[1][1] = cos;
        rMtx[1][2] = -sin;
        rMtx[1][3] = 0.0f;
        rMtx[2][0] = 0.0f;
        rMtx[2][1] = sin;
        rMtx[2][2] = cos;
        rMtx[2][3] = 0.0f;
    }

    static void rotTypeZ(f32 sin, f32 cos, Mtx& rMtx) {
        rMtx[0][0] = cos;
        rMtx[0][1] = -sin;
        rMtx[0][2] = 0.0f;
        rMtx[0][3] = 0.0f;
        rMtx[1][0] = sin;
        rMtx[1][1] = cos;
        rMtx[1][2] = 0.0f;
        rMtx[1][3] = 0.0f;
        rMtx[2][0] = 0.0f;
        rMtx[2][1] = 0.0f;
        rMtx[2][2] = 1.0f;
        rMtx[2][3] = 0.0f;
    }

    static void rotTypeXYZ(f32 sin, f32 cos, Mtx& rMtx) {
        f32 third = 0.333333f * (1.0f - cos);
        f32 scaledSin = 0.57735f * sin;
        f32 plus = third + scaledSin;
        rMtx[0][0] = (third + cos);
        rMtx[0][1] = (third - scaledSin);
        rMtx[0][2] = plus;
        rMtx[0][3] = 0.0f;
        rMtx[1][0] = plus;
        rMtx[1][1] = (third + cos);
        rMtx[1][2] = (third - scaledSin);
        rMtx[1][3] = 0.0f;
        rMtx[2][0] = (third - scaledSin);
        rMtx[2][1] = plus;
        rMtx[2][2] = (third + cos);
        rMtx[2][3] = 0.0f;
    }

    static void basePlaneTypeXY(MtxPtr pMtx, f32 scaleX, f32 scaleY) {
        pMtx[0][0] *= scaleX;
        pMtx[1][0] *= scaleX;
        pMtx[2][0] *= scaleX;
        pMtx[0][1] *= scaleY;
        pMtx[1][1] *= scaleY;
        pMtx[2][1] *= scaleY;
    }

    static void basePlaneTypeXZ(MtxPtr pMtx, f32 scaleX, f32 scaleY) {
        pMtx[0][0] *= scaleX;
        pMtx[1][0] *= scaleX;
        pMtx[2][0] *= scaleX;
        pMtx[0][2] *= scaleY;
        pMtx[1][2] *= scaleY;
        pMtx[2][2] *= scaleY;
    }

    static void basePlaneTypeX(MtxPtr pMtx, f32 scaleX, f32 scaleY) {
        pMtx[0][0] *= scaleX;
        pMtx[1][0] *= scaleX;
        pMtx[2][0] *= scaleX;
        pMtx[0][1] *= scaleY;
        pMtx[1][1] *= scaleY;
        pMtx[2][1] *= scaleY;
        pMtx[0][2] *= scaleX;
        pMtx[1][2] *= scaleX;
        pMtx[2][2] *= scaleX;
    }

    typedef void (*dirTypeFunc)(JPAEmitterWorkData const*, JPABaseParticle const*, JGeometry::TVec3< f32 >*);
    static dirTypeFunc p_direction[5] = {
        dirTypeVel, dirTypePos, dirTypePosInv, dirTypeEmtrDir, dirTypePrevPtcl,
    };

    typedef void (*rotTypeFunc)(f32, f32, Mtx&);
    static rotTypeFunc p_rot[5] = {
        rotTypeY, rotTypeX, rotTypeZ, rotTypeXYZ, rotTypeY,
    };

    typedef void (*planeFunc)(MtxPtr, f32, f32);

    static planeFunc p_plane[3] = {
        basePlaneTypeXY,
        basePlaneTypeXZ,
        basePlaneTypeX,
    };

}  // namespace

void JPABaseEmitter::init(JPAEmitterManager* pManager, JPAResource* pResource) {
    mpEmtrMgr = pManager;
    mpRes = pResource;
    const JPADynamicsBlockData* pData = mpRes->getDyn()->mpData;
    mLocalScl.set< f32 >(pData->mEmitterScl.x, pData->mEmitterScl.y, pData->mEmitterScl.z);
    pData = mpRes->getDyn()->mpData;
    mLocalTrs.set< f32 >(pData->mEmitterTrs.x, pData->mEmitterTrs.y, pData->mEmitterTrs.z);
    pData = mpRes->getDyn()->mpData;
    mLocalDir.set< f32 >(pData->mEmitterDir.x, pData->mEmitterDir.y, pData->mEmitterDir.z);
    if (!MR::isNearZero(mLocalDir)) {
        MR::normalize(&mLocalDir);
    }

    const JGeometry::TVec3< s16 >& rRotation = mpRes->getDyn()->mpData->mEmitterRot;
    const s16 x = rRotation.x;
    const s16 z = rRotation.z;
    const s16 y = rRotation.y;
    mLocalRot.set(x, y, z);
    mMaxFrame = mpRes->getDyn()->getMaxFrame();
    mLifeTime = mpRes->getDyn()->getLifetime();
    mVolumeSize = mpRes->getDyn()->getVolumeSize();
    mRate = mpRes->getDyn()->getRate();
    mRateStep = mpRes->getDyn()->getRateStep();
    mVolumeSweep = mpRes->getDyn()->getVolumeSweep();
    mVolumeMinRad = mpRes->getDyn()->getVolumeMinRad();
    mAwayFromCenterSpeed = mpRes->getDyn()->getInitVelOmni();
    mAwayFromAxisSpeed = mpRes->getDyn()->getInitVelAxis();
    mDirSpeed = mpRes->getDyn()->getInitVelDir();
    mSpread = mpRes->getDyn()->getInitVelDirSp();
    mRndmDirSpeed = mpRes->getDyn()->getInitVelRndm();
    mAirResist = mpRes->getDyn()->getAirRes();
    mRndm.set_seed(mpEmtrMgr->pWd->mRndm.get_rndm_u());
    PSMTXIdentity(mGlobalRot);
    mGlobalScl.set(1.0f, 1.0f, 1.0f);
    mGlobalTrs.zero();
    mGlobalPScl.set(1.0f, 1.0f);
    mGlobalPrmClr.r = mGlobalPrmClr.g = mGlobalPrmClr.b = mGlobalPrmClr.a = mGlobalEnvClr.r = mGlobalEnvClr.g = mGlobalEnvClr.b = mGlobalEnvClr.a =
        255;
    pResource->getBsp()->getPrmClr(&mPrmClr);
    pResource->getBsp()->getEnvClr(&mEnvClr);
    mpUserWork = 0;
    mScaleOut = 1.0f;
    mEmitCount = 0.0f;
    mStatus = 0x30;
    mDrawTimes = 1;
    mTick = 0;
    mWaitTime = 0;
    mRateStepTimer = 0;
    mTexAnmIdx = 0;
}

void JPADrawDirection(JPAEmitterWorkData* pWork, JPABaseParticle* pParticle) {
    if (pParticle->checkStatus(8)) {
        return;
    }

    TVec3f direction;
    TVec3f side;
    p_direction[pWork->mDirType](pWork, pParticle, &direction);
    if (MR::isNearZero(direction)) {
        return;
    }

    MR::normalize(&direction);
    side.cross(pParticle->mBaseAxis, direction);
    if (MR::isNearZero(side)) {
        return;
    }

    MR::normalize(&side);
    pParticle->mBaseAxis.cross(direction, side);
    MR::normalize(&pParticle->mBaseAxis);
    f32 scaleX = pWork->mGlobalPtclScl.x * pParticle->mParticleScaleX;
    f32 scaleY = pWork->mGlobalPtclScl.y * pParticle->mParticleScaleY;
    Mtx matrix;
    matrix[0][0] = pParticle->mBaseAxis.x;
    matrix[0][1] = direction.x;
    matrix[0][2] = side.x;
    matrix[0][3] = pParticle->mPosition.x;
    matrix[1][0] = pParticle->mBaseAxis.y;
    matrix[1][1] = direction.y;
    matrix[1][2] = side.y;
    matrix[1][3] = pParticle->mPosition.y;
    matrix[2][0] = pParticle->mBaseAxis.z;
    matrix[2][1] = direction.z;
    matrix[2][2] = side.z;
    matrix[2][3] = pParticle->mPosition.z;
    p_plane[pWork->mPlaneType](matrix, scaleX, scaleY);
    PSMTXConcat(pWork->mPosCamMtx, matrix, matrix);
    GXLoadPosMtxImm(matrix, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, matrix);
    GXCallDisplayList(p_dl[pWork->mDLType], 32);
}

void JPADrawRotDirection(JPAEmitterWorkData* pWork, JPABaseParticle* pParticle) {
    if (pParticle->checkStatus(8)) {
        return;
    }

    f32 sin = JMASSin(pParticle->mRotateAngle);
    f32 cos = JMath::sSinCosTable.cosShort(pParticle->mRotateAngle);
    TVec3f direction;
    TVec3f side;
    p_direction[pWork->mDirType](pWork, pParticle, &direction);
    if (MR::isNearZero(direction)) {
        return;
    }

    MR::normalize(&direction);
    side.cross(pParticle->mBaseAxis, direction);
    if (MR::isNearZero(side)) {
        return;
    }

    MR::normalize(&side);
    pParticle->mBaseAxis.cross(direction, side);
    MR::normalize(&pParticle->mBaseAxis);
    f32 scaleX = pWork->mGlobalPtclScl.x * pParticle->mParticleScaleX;
    f32 scaleY = pWork->mGlobalPtclScl.y * pParticle->mParticleScaleY;
    Mtx rotation;
    Mtx matrix;
    p_rot[pWork->mRotType](sin, cos, rotation);
    p_plane[pWork->mPlaneType](rotation, scaleX, scaleY);
    matrix[0][0] = pParticle->mBaseAxis.x;
    matrix[0][1] = direction.x;
    matrix[0][2] = side.x;
    matrix[0][3] = pParticle->mPosition.x;
    matrix[1][0] = pParticle->mBaseAxis.y;
    matrix[1][1] = direction.y;
    matrix[1][2] = side.y;
    matrix[1][3] = pParticle->mPosition.y;
    matrix[2][0] = pParticle->mBaseAxis.z;
    matrix[2][1] = direction.z;
    matrix[2][2] = side.z;
    matrix[2][3] = pParticle->mPosition.z;
    PSMTXConcat(matrix, rotation, rotation);
    PSMTXConcat(pWork->mPosCamMtx, rotation, matrix);
    GXLoadPosMtxImm(matrix, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, matrix);
    GXCallDisplayList(p_dl[pWork->mDLType], 32);
}

void JPADrawDBillboard(JPAEmitterWorkData* pWork, JPABaseParticle* pParticle) {
    if (pParticle->checkStatus(8)) {
        return;
    }

    TVec3f direction;
    p_direction[pWork->mDirType](pWork, pParticle, &direction);
    TVec3f camera(pWork->mPosCamMtx[2][0], pWork->mPosCamMtx[2][1], pWork->mPosCamMtx[2][2]);
    PSVECCrossProduct(&direction, &camera, &direction);
    if (MR::isNearZero(direction)) {
        return;
    }

    MR::normalize(&direction);
    PSMTXMultVecSR(pWork->mPosCamMtx, &direction, &direction);
    TVec3f position;
    PSMTXMultVec(pWork->mPosCamMtx, &pParticle->mPosition, &position);
    f32 scaleX = pWork->mGlobalPtclScl.x * pParticle->mParticleScaleX;
    f32 scaleY = pWork->mGlobalPtclScl.y * pParticle->mParticleScaleY;
    Mtx matrix;
    matrix[0][0] = direction.x * scaleX;
    matrix[0][1] = -direction.y * scaleY;
    matrix[0][3] = position.x;
    matrix[1][0] = direction.y * scaleX;
    matrix[1][1] = direction.x * scaleY;
    matrix[1][3] = position.y;
    matrix[2][2] = 1.0f;
    matrix[2][3] = position.z;
    matrix[0][2] = matrix[1][2] = matrix[2][0] = matrix[2][1] = 0.0f;
    GXLoadPosMtxImm(matrix, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, matrix);
    GXCallDisplayList(jpa_dl, 32);
}

void JPADrawLine(JPAEmitterWorkData* pWork, JPABaseParticle* pParticle) {
    if (pParticle->checkStatus(8)) {
        return;
    }

    TVec3f position(pParticle->mPosition);
    TVec3f end;
    end.set< f32 >(pParticle->mVelocity);
    if (MR::isNearZero(end)) {
        return;
    }

    end.setLength(pWork->mGlobalPtclScl.y * (25.0f * pParticle->mParticleScaleY));
    end.sub(position, end);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXBegin(GX_LINES, GX_VTXFMT1, 2);
    GXPosition3f32(position.x, position.y, position.z);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(end.x, end.y, end.z);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

void JPADrawStripe(JPAEmitterWorkData* pWork) {
    const u32 count = pWork->mpAlivePtcl->getNum();
    JPABaseShape* pShape = pWork->mpRes->getBsp();
    if (count < 2) {
        return;
    }

    f32 texCoord = 0.0f;
    f32 texStep = 1.0f / (count - 1.0f);
    f32 leftWidth = (1.0f + pWork->mPivot.x) * (25.0f * pWork->mGlobalPtclScl.x);
    f32 rightWidth = (1.0f - pWork->mPivot.x) * (25.0f * pWork->mGlobalPtclScl.x);
    JPANode< JPABaseParticle >* pFirst;
    JPANode< JPABaseParticle >* (*pNext)(JPANode< JPABaseParticle >*);
    if (pShape->isDrawFwdAhead()) {
        texCoord = 1.0f;
        texStep = -texStep;
        pFirst = pWork->mpAlivePtcl->getLast();
        pNext = getPrev;
    } else {
        pFirst = pWork->mpAlivePtcl->getFirst();
        pNext = getNext;
    }

    GXLoadPosMtxImm(pWork->mPosCamMtx, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, pWork->mPosCamMtx);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    TVec3f position;
    TVec3f direction;
    TVec3f side;
    TVec3f vertices[2];
    Mtx matrix;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, count * 2);
    for (JPANode< JPABaseParticle >*pNode = pFirst, *pEnd = pWork->mpAlivePtcl->getEnd(); pNode != pEnd;) {
        JPABaseParticle* pParticle = pNode->getObject();
        pWork->mpCurNode = pNode;
        position.set< f32 >(pParticle->mPosition);
        f32 sin = JMASSin(pParticle->mRotateAngle);
        f32 cos = JMath::sSinCosTable.cosShort(pParticle->mRotateAngle);
        vertices[0].set(-pParticle->mParticleScaleX * leftWidth, 0.0f, 0.0f);
        vertices[0].set< f32 >(vertices[0].x * cos, 0.0f, vertices[0].x * sin);
        vertices[1].set< f32 >(pParticle->mParticleScaleX * rightWidth, 0.0f, 0.0f);
        vertices[1].set< f32 >(vertices[1].x * cos, 0.0f, vertices[1].x * sin);
        p_direction[pWork->mDirType](pWork, pParticle, &direction);
        if (MR::isNearZero(direction)) {
            direction.set< f32 >(0.0f, 1.0f, 0.0f);
        } else {
            MR::normalize(&direction);
        }

        side.cross(pParticle->mBaseAxis, direction);
        if (MR::isNearZero(side)) {
            side.set< f32 >(1.0f, 0.0f, 0.0f);
        } else {
            MR::normalize(&side);
        }

        pParticle->mBaseAxis.cross(direction, side);
        MR::normalize(&pParticle->mBaseAxis);
        matrix[0][0] = side.x;
        matrix[0][1] = direction.x;
        matrix[0][2] = pParticle->mBaseAxis.x;
        matrix[0][3] = 0.0f;
        matrix[1][0] = side.y;
        matrix[1][1] = direction.y;
        matrix[1][2] = pParticle->mBaseAxis.y;
        matrix[1][3] = 0.0f;
        matrix[2][0] = side.z;
        matrix[2][1] = direction.z;
        matrix[2][2] = pParticle->mBaseAxis.z;
        matrix[2][3] = 0.0f;
        PSMTXMultVecArraySR(matrix, vertices, vertices, 2);
        GXPosition3f32(vertices[0].x + position.x, vertices[0].y + position.y, vertices[0].z + position.z);
        GXTexCoord2f32(0.0f, texCoord);
        GXPosition3f32(vertices[1].x + position.x, vertices[1].y + position.y, vertices[1].z + position.z);
        GXTexCoord2f32(1.0f, texCoord);
        pNode = pNext(pNode);
        texCoord += texStep;
    }

    GXEnd();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

void JPADrawStripeX(JPAEmitterWorkData* pWork) {
    const u32 count = pWork->mpAlivePtcl->getNum();
    JPABaseShape* pShape = pWork->mpRes->getBsp();
    if (count < 2) {
        return;
    }

    f32 startTexCoord = 0.0f;
    f32 texCoord = 0.0f;
    f32 texStep = 1.0f / (count - 1.0f);
    f32 leftWidth = (1.0f + pWork->mPivot.x) * (25.0f * pWork->mGlobalPtclScl.x);
    f32 rightWidth = (1.0f - pWork->mPivot.x) * (25.0f * pWork->mGlobalPtclScl.x);
    f32 topWidth = (1.0f + pWork->mPivot.y) * (25.0f * pWork->mGlobalPtclScl.y);
    f32 bottomWidth = (1.0f - pWork->mPivot.y) * (25.0f * pWork->mGlobalPtclScl.y);
    JPANode< JPABaseParticle >* pFirst;
    JPANode< JPABaseParticle >* (*pNext)(JPANode< JPABaseParticle >*);
    if (pShape->isDrawFwdAhead()) {
        texCoord = 1.0f;
        startTexCoord = 1.0f;
        texStep = -texStep;
        pFirst = pWork->mpAlivePtcl->getLast();
        pNext = getPrev;
    } else {
        pFirst = pWork->mpAlivePtcl->getFirst();
        pNext = getNext;
    }

    GXLoadPosMtxImm(pWork->mPosCamMtx, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, pWork->mPosCamMtx);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    TVec3f position;
    TVec3f direction;
    TVec3f side;
    TVec3f vertices[2];
    Mtx matrix;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, count * 2);
    for (JPANode< JPABaseParticle >*pNode = pFirst, *pEnd = pWork->mpAlivePtcl->getEnd(); pNode != pEnd;) {
        JPABaseParticle* pParticle = pNode->getObject();
        pWork->mpCurNode = pNode;
        position.set< f32 >(pParticle->mPosition);
        f32 sin = JMASSin(pParticle->mRotateAngle);
        f32 cos = JMath::sSinCosTable.cosShort(pParticle->mRotateAngle);
        vertices[0].set(-pParticle->mParticleScaleX * leftWidth, 0.0f, 0.0f);
        vertices[0].set< f32 >(vertices[0].x * cos, 0.0f, vertices[0].x * sin);
        vertices[1].set< f32 >(pParticle->mParticleScaleX * rightWidth, 0.0f, 0.0f);
        vertices[1].set< f32 >(vertices[1].x * cos, 0.0f, vertices[1].x * sin);
        p_direction[pWork->mDirType](pWork, pParticle, &direction);
        if (MR::isNearZero(direction)) {
            direction.set< f32 >(0.0f, 1.0f, 0.0f);
        } else {
            MR::normalize(&direction);
        }

        side.cross(pParticle->mBaseAxis, direction);
        if (MR::isNearZero(side)) {
            side.set< f32 >(1.0f, 0.0f, 0.0f);
        } else {
            MR::normalize(&side);
        }

        if (MR::isSameDirection(direction, side, 0.01f)) {
            side.set< f32 >(0.0f, 1.0f, 0.0f);
        }

        pParticle->mBaseAxis.cross(direction, side);
        MR::normalize(&pParticle->mBaseAxis);
        matrix[0][0] = side.x;
        matrix[0][1] = direction.x;
        matrix[0][2] = pParticle->mBaseAxis.x;
        matrix[0][3] = 0.0f;
        matrix[1][0] = side.y;
        matrix[1][1] = direction.y;
        matrix[1][2] = pParticle->mBaseAxis.y;
        matrix[1][3] = 0.0f;
        matrix[2][0] = side.z;
        matrix[2][1] = direction.z;
        matrix[2][2] = pParticle->mBaseAxis.z;
        matrix[2][3] = 0.0f;
        PSMTXMultVecArraySR(matrix, vertices, vertices, 2);
        GXPosition3f32(vertices[0].x + position.x, vertices[0].y + position.y, vertices[0].z + position.z);
        GXTexCoord2f32(0.0f, texCoord);
        GXPosition3f32(vertices[1].x + position.x, vertices[1].y + position.y, vertices[1].z + position.z);
        GXTexCoord2f32(1.0f, texCoord);
        pNode = pNext(pNode);
        texCoord += texStep;
    }

    GXEnd();
    texCoord = startTexCoord;
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT1, count * 2);
    for (JPANode< JPABaseParticle >*pNode = pFirst, *pEnd = pWork->mpAlivePtcl->getEnd(); pNode != pEnd;) {
        JPABaseParticle* pParticle = pNode->getObject();
        pWork->mpCurNode = pNode;
        position.set< f32 >(pParticle->mPosition);
        f32 sin = -JMASSin(pParticle->mRotateAngle);
        f32 cos = JMath::sSinCosTable.cosShort(pParticle->mRotateAngle);
        vertices[0].set(-pParticle->mParticleScaleY * topWidth, 0.0f, 0.0f);
        vertices[0].set< f32 >(vertices[0].x * sin, 0.0f, vertices[0].x * cos);
        vertices[1].set< f32 >(pParticle->mParticleScaleY * bottomWidth, 0.0f, 0.0f);
        vertices[1].set< f32 >(vertices[1].x * sin, 0.0f, vertices[1].x * cos);
        p_direction[pWork->mDirType](pWork, pParticle, &direction);
        if (MR::isNearZero(direction)) {
            direction.set< f32 >(0.0f, 1.0f, 0.0f);
        } else {
            MR::normalize(&direction);
        }

        side.cross(pParticle->mBaseAxis, direction);
        if (MR::isNearZero(side)) {
            side.set< f32 >(1.0f, 0.0f, 0.0f);
        } else {
            MR::normalize(&side);
        }

        if (MR::isSameDirection(direction, side, 0.01f)) {
            side.set< f32 >(0.0f, 1.0f, 0.0f);
        }

        pParticle->mBaseAxis.cross(direction, side);
        MR::normalize(&pParticle->mBaseAxis);
        matrix[0][0] = side.x;
        matrix[0][1] = direction.x;
        matrix[0][2] = pParticle->mBaseAxis.x;
        matrix[0][3] = 0.0f;
        matrix[1][0] = side.y;
        matrix[1][1] = direction.y;
        matrix[1][2] = pParticle->mBaseAxis.y;
        matrix[1][3] = 0.0f;
        matrix[2][0] = side.z;
        matrix[2][1] = direction.z;
        matrix[2][2] = pParticle->mBaseAxis.z;
        matrix[2][3] = 0.0f;
        PSMTXMultVecArraySR(matrix, vertices, vertices, 2);
        GXPosition3f32(vertices[0].x + position.x, vertices[0].y + position.y, vertices[0].z + position.z);
        GXTexCoord2f32(0.0f, texCoord);
        GXPosition3f32(vertices[1].x + position.x, vertices[1].y + position.y, vertices[1].z + position.z);
        GXTexCoord2f32(1.0f, texCoord);
        pNode = pNext(pNode);
        texCoord += texStep;
    }

    GXEnd();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
}

void JPAFieldAir::prepare(JPAEmitterWorkData* pWork, JPAFieldBlock* pBlock) {
    TVec3f direction(pBlock->getDir());
    MR::normalizeOrZero(&direction);
    if (pBlock->checkStatus(2)) {
        mAccel.scale(pBlock->getMag(), direction);
    } else {
        PSMTXMultVecSR(pWork->mRotationMtx, &direction, &mAccel);
        mAccel.scale(pBlock->getMag());
    }
}

void JPAFieldVortex::prepare(JPAEmitterWorkData* pWork, JPAFieldBlock* pBlock) {
    PSMTXMultVecSR(pWork->mGlobalRot, &pBlock->getDir(), &field_0x10);
    MR::normalizeOrZero(&field_0x10);
    field_0x1c = pBlock->getPos().z * pBlock->getPos().z;
    field_0x20 = 1.0f / field_0x1c;
}

void JPAFieldVortex::calc(JPAEmitterWorkData*, JPAFieldBlock* pBlock, JPABaseParticle* pParticle) {
    TVec3f radial;
    radial.scale(field_0x10.dot(pParticle->mLocalPosition), field_0x10);
    radial.sub(pParticle->mLocalPosition, radial);
    f32 distanceSquared = radial.squared();
    f32 magnitude;
    if (distanceSquared > field_0x1c) {
        magnitude = pBlock->getMagRndm();
    } else {
        f32 rate = distanceSquared * field_0x20;
        magnitude = (1.0f - rate) * pBlock->getMag() + rate * pBlock->getMagRndm();
    }

    MR::normalizeOrZero(&radial);
    mAccel.cross(radial, field_0x10);
    mAccel.scale(magnitude);
    calcAffect(pBlock, pParticle);
}

void JPAFieldConvection::prepare(JPAEmitterWorkData* pWork, JPAFieldBlock* pBlock) {
    TVec3f radial;
    TVec3f tangent;
    tangent.cross(pBlock->getPos(), pBlock->getDir());
    radial.cross(pBlock->getDir(), tangent);
    PSMTXMultVecSR(pWork->mGlobalRot, &radial, &field_0x10);
    PSMTXMultVecSR(pWork->mGlobalRot, &pBlock->getDir(), &field_0x1c);
    PSMTXMultVecSR(pWork->mGlobalRot, &tangent, &field_0x28);
    MR::normalizeOrZero(&field_0x10);
    MR::normalizeOrZero(&field_0x1c);
    MR::normalizeOrZero(&field_0x28);
}

void JPAFieldSpin::prepare(JPAEmitterWorkData* pWork, JPAFieldBlock* pBlock) {
    TVec3f axis;
    Mtx rotation;
    PSMTXMultVecSR(pWork->mGlobalRot, &pBlock->getDir(), &axis);
    MR::normalizeOrZero(&axis);
    PSMTXRotAxisRad(rotation, &axis, pBlock->getMag());
    field_0x10.set< f32 >(rotation[0][0], rotation[1][0], rotation[2][0]);
    field_0x1c.set< f32 >(rotation[0][1], rotation[1][1], rotation[2][1]);
    field_0x28.set< f32 >(rotation[0][2], rotation[1][2], rotation[2][2]);
}

void JPAEmitterManager::calcYBBCam() {
    TVec3f axis(0.0f, pWd->mPosCamMtx[1][1], pWd->mPosCamMtx[2][1]);
    if (!MR::isNearZero(axis)) {
        MR::normalize(&axis);
        pWd->mYBBCamMtx[0][0] = 1.0f;
        pWd->mYBBCamMtx[0][1] = 0.0f;
        pWd->mYBBCamMtx[0][2] = 0.0f;
        pWd->mYBBCamMtx[0][3] = pWd->mPosCamMtx[0][3];
        pWd->mYBBCamMtx[1][0] = 0.0f;
        pWd->mYBBCamMtx[1][1] = axis.y;
        pWd->mYBBCamMtx[1][2] = -axis.z;
        pWd->mYBBCamMtx[1][3] = pWd->mPosCamMtx[1][3];
        pWd->mYBBCamMtx[2][0] = 0.0f;
        pWd->mYBBCamMtx[2][1] = axis.z;
        pWd->mYBBCamMtx[2][2] = axis.y;
        pWd->mYBBCamMtx[2][3] = pWd->mPosCamMtx[2][3];
    }
}

void JPADrawYBillboard(JPAEmitterWorkData* pWork, JPABaseParticle* pParticle) {
    if (pParticle->checkStatus(8)) {
        return;
    }

    TVec3f axis(0.0f, pWork->mPosCamMtx[1][1], pWork->mPosCamMtx[2][1]);
    if (MR::isNearZero(axis)) {
        return;
    }

    TVec3f position;
    PSMTXMultVec(pWork->mPosCamMtx, &pParticle->mPosition, &position);
    f32 scaleX = pWork->mGlobalPtclScl.x * pParticle->mParticleScaleX;
    f32 scaleY = pWork->mGlobalPtclScl.y * pParticle->mParticleScaleY;
    Mtx matrix;
    matrix[0][0] = scaleX;
    matrix[0][3] = position.x;
    matrix[1][1] = pWork->mYBBCamMtx[1][1] * scaleY;
    matrix[1][2] = pWork->mYBBCamMtx[1][2];
    matrix[1][3] = position.y;
    matrix[2][1] = pWork->mYBBCamMtx[2][1] * scaleY;
    matrix[2][2] = pWork->mYBBCamMtx[2][2];
    matrix[2][3] = position.z;
    matrix[0][1] = matrix[0][2] = matrix[1][0] = matrix[2][0] = 0.0f;
    GXLoadPosMtxImm(matrix, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, matrix);
    GXCallDisplayList(jpa_dl, 32);
}

void JPADrawRotYBillboard(JPAEmitterWorkData* pWork, JPABaseParticle* pParticle) {
    if (pParticle->checkStatus(8)) {
        return;
    }

    TVec3f axis(0.0f, pWork->mPosCamMtx[1][1], pWork->mPosCamMtx[2][1]);
    if (MR::isNearZero(axis)) {
        return;
    }

    TVec3f position;
    PSMTXMultVec(pWork->mPosCamMtx, &pParticle->mPosition, &position);
    f32 scaleX = pWork->mGlobalPtclScl.x * pParticle->mParticleScaleX;
    f32 scaleY = pWork->mGlobalPtclScl.y * pParticle->mParticleScaleY;
    Mtx matrix;
    f32 sin = JMASSin(pParticle->mRotateAngle);
    f32 cos = JMath::sSinCosTable.cosShort(pParticle->mRotateAngle);
    f32 sinX = sin * scaleX;
    f32 cosY = cos * scaleY;
    f32 y = pWork->mYBBCamMtx[1][1];
    f32 z = pWork->mYBBCamMtx[2][1];
    matrix[0][0] = (cos * scaleX);
    matrix[0][1] = (-sin * scaleY);
    matrix[0][2] = 0.0f;
    matrix[0][3] = position.x;
    matrix[1][0] = sinX * y;
    matrix[1][1] = cosY * y;
    matrix[1][2] = -z;
    matrix[1][3] = position.y;
    matrix[2][0] = sinX * z;
    matrix[2][1] = cosY * z;
    matrix[2][2] = y;
    matrix[2][3] = position.z;
    GXLoadPosMtxImm(matrix, GX_PNMTX0);
    p_prj[pWork->mPrjType](pWork, matrix);
    GXCallDisplayList(jpa_dl, 32);
}

void JKRAramPiece::startDMA(JKRAMCommand* pCommand) {
    if (pCommand->mSrc < 0x80000000) {
        doneDMA(reinterpret_cast< u32 >(pCommand));
        return;
    }

    if (pCommand->mDst >= 0x04000000) {
        doneDMA(reinterpret_cast< u32 >(pCommand));
        return;
    }

    if (pCommand->mDataLength > 0x00E00000) {
        doneDMA(reinterpret_cast< u32 >(pCommand));
        return;
    }

    ARStartDMA(pCommand->mTransferDirection, pCommand->mSrc, pCommand->mDst, pCommand->mDataLength);
    doneDMA(reinterpret_cast< u32 >(pCommand));
}

void JAU_JASInitializer::initJASystem(JKRSolidHeap* pHeap) {
    if (JASAudioThread::getThreadPointer() == nullptr && JASDvd::getThreadPointer() == nullptr) {
        JASKernel::setupRootHeap(pHeap, heapSize_);
        if (audioMemory_ == 0) {
            audioMemory_ = JKRAram::getManager()->getAudioMemory();
        }

        if (audioMemSize_ == 0) {
            audioMemSize_ = JKRAram::getManager()->getAudioMemSize();
        }

        JASKernel::setupAramHeap(audioMemory_, audioMemSize_);
        JASTrack::newMemPool(field_0x1c);
        if (field_0x20 > 0) {
            JASTrack::TChannelMgr::newMemPool(field_0x20);
        }

        JASDvd::createThread(dvdThreadPriority_, 0x100, 0x8000);
        JASAudioThread::create(audioThreadPriority_);
        JKRThreadSwitch* pThreadSwitch = JKRThreadSwitch::getManager();
        if (pThreadSwitch != nullptr) {
            if (dvdThreadId_ >= 0) {
                pThreadSwitch->enter(JASDvd::getThreadPointer(), dvdThreadId_);
            }

            if (audioThreadId_ >= 0) {
                pThreadSwitch->enter(JASAudioThread::getThreadPointer(), audioThreadId_);
            }
        }

        JASDriver::setDSPLevel(dspLevel_);
        JASAramStream::initSystem(aramBlockSize_, aramChannelNum_);
    }

    if (waveArcDir_ != nullptr) {
        JASWaveArcLoader::setCurrentDir(waveArcDir_);
    }

    switch (SCGetSoundMode()) {
    case 0:
        JASDriver::setOutputMode(0);
        break;
    case 1:
        JASDriver::setOutputMode(1);
        break;
    case 2:
        JASDriver::setOutputMode(2);
        break;
    }
}

namespace {
    s32 sAudioThreadMsgSize = 0x20;
    u32 sAudioThreadStackSize = 0x8000;
    u32 sAramThreadStackSize = 0xC000;
    s32 sAramThreadMsgSize = 0x20;
}  // namespace

JASAudioThread::JASAudioThread(int priority, int, u32)
    : JKRThread(JASDram, sAudioThreadStackSize, sAudioThreadMsgSize, priority), JASGlobalInstance< JASAudioThread >(true), sbPauseFlag() {
    OSInitThreadQueue(&sThreadQueue);
}

JKRAram::JKRAram(u32 audioSize, u32 graphSize, s32 priority) : JKRThread(sAramThreadStackSize, sAramThreadMsgSize, priority) {
    u32 reserved = ARInit(mStackArray, 3);
    ARQInit();
    u32 total = ARGetSize();
    mAudioMemorySize = audioSize;
    if (graphSize == 0xFFFFFFFF) {
        mGraphMemorySize = total - audioSize - reserved;
        mAramMemorySize = 0;
    } else {
        mGraphMemorySize = graphSize;
        mAramMemorySize = total - (audioSize + graphSize) - reserved;
    }

    mAudioMemoryPtr = ARAlloc(mAudioMemorySize);
    mGraphMemoryPtr = ARAlloc(mGraphMemorySize);
    if (mAramMemorySize != 0) {
        mAramMemoryPtr = ARAlloc(mAramMemorySize);
    } else {
        mAramMemoryPtr = 0;
    }

    mAramHeap = new (JKRGetSystemHeap(), 0) JKRAramHeap(mGraphMemoryPtr, mGraphMemorySize);
}
