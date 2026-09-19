#include "Game/Map/WaterPlant.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DAnimation.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace {
    f32 gxArray[] = {0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.2f, 1.0f, 0.2f, 0.0f, 0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f};
    static f32 sHeightTable[] = {150.0f, 200.0f, 300.0f, 250.0f};
    static TVec3f sDrawVec(1.0f, 0.0f, 0.0f);
    static f32 sSwingSpeed = 0.03f;
    static f32 sSwingWidth = 15.0f;

    static const s32 sDefaultPlantNum = 16;
    static const f32 sDefaultAreaRadius = 500.0f;
    static const s32 sSwingPosTableSize = 64;
    static const s32 sPlantAddTime = 10;
    static const f32 sSwingSpeedMin = 0.03f;
    static const f32 sSwingSpeedMax = 0.03f;
    static const f32 sSwingSpeedDown = 0.001f;
    static const f32 sSwingWidthMin = 20.0f;
    static const f32 sSwingWidthMax = 20.0f;
    static const f32 sSwingWidthDown = 0.5f;
    static const f32 sSpinWaveDistanceMax = 1500.0f;
    // static const s32 sSpinWaveTimeRate
    static const s32 sSpinWaveSwingTimeMax = 90;
    static const s32 sSpinWaveValidTime = 20;
    static const f32 sSpwinWaveSwingRate = 400.0f;
    // static const f32 sSwinWaveWidth
    // static const s32 sInterval
    static const f32 sDefaultHalfWidth = 20.0f;
    static const f32 sHeightRate0 = 0.5f;
    static const f32 sHeightRate1 = 0.8f;
    static const f32 sSpinSwingRate0 = 0.3f;
    static const f32 sSpinSwingRate1 = 0.6f;
    static const f32 sFogStartZ = 2000.0f;
    static const f32 sFogEndZ = 10000.0f;
};  // namespace

namespace {
    // function exists in debug map
    inline WaterPlantDrawInit* getWaterPlantDrawInit() {
        return MR::getSceneObj< WaterPlantDrawInit >(SceneObj_WaterPlantDrawInit);
    }

};  // namespace

WaterPlant::WaterPlant(const char* pName)
    : LiveActor(pName), mPlantNum(::sDefaultPlantNum), mPlantDataArray(), mAreaRadius(::sDefaultAreaRadius), mIsSpin(),
      mHalfWidth(::sDefaultHalfWidth), mHeight(), mType() {
}

void WaterPlant::init(const JMapInfoIter& rIter) {
    MR::createSceneObj(SceneObj_WaterPlantDrawInit);
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, MR::DrawType_WaterPlant);

    MR::initDefaultPos(this, rIter);

    MR::getJMapInfoArg0NoInit(rIter, &mPlantNum);
    MR::getJMapInfoArg1NoInit(rIter, &mAreaRadius);
    MR::getJMapInfoArg3NoInit(rIter, &mType);

    mHeight = ::sHeightTable[mType];
    initPlant();

    MR::setClippingTypeSphere(this, TVec2f(mAreaRadius, 2.0f * mHeight).length());

    makeActorAppeared();
}

void WaterPlant::initAfterPlacement() {
    initPlantAfterPlacement();
}

void WaterPlant::movement() {
    if (mIsSpin) {
        s32 count = 0;
        for (int i = 0; i < mPlantNum; i++) {
            if (mPlantDataArray[i].mSpinWaveTimer >= 0) {
                count++;
                mPlantDataArray[i].mSpinWaveTimer--;

                if (mPlantDataArray[i].mSpinWaveTimer <= 0) {
                    mPlantDataArray[i].mSpinWaveSwingTimer = ::sSpinWaveSwingTimeMax;
                    mPlantDataArray[i].mSpinSwingDir.set(mPlantDataArray[i].mTempSpinSwingDir);
                }
            }

            if (mPlantDataArray[i].mSpinWaveSwingTimer >= 0) {
                count++;
                mPlantDataArray[i].mSpinWaveSwingTimer--;
                f32 flt = static_cast< f32 >(mPlantDataArray[i].mSpinWaveSwingTimer) / 90.0f;
                mPlantDataArray[i].mSpinWaveSwingVec1.set(mPlantDataArray[i].mSpinSwingDir);
                mPlantDataArray[i].mSpinWaveSwingVec2.set(mPlantDataArray[i].mSpinSwingDir);
                mPlantDataArray[i].mSpinWaveSwingVec3.set(mPlantDataArray[i].mSpinSwingDir);

                f32 spinSwingRate = mPlantDataArray[i].mSpinSwingStrength * (::sSpwinWaveSwingRate * (flt * MR::sinDegree(180.0f * (1.0f - flt))));
                mPlantDataArray[i].mSpinWaveSwingVec1.scale(::sSpinSwingRate0 * spinSwingRate);
                mPlantDataArray[i].mSpinWaveSwingVec2.scale(::sSpinSwingRate1 * spinSwingRate);
                mPlantDataArray[i].mSpinWaveSwingVec3.scale(spinSwingRate);
            }
        }

        if (count == 0) {
            mIsSpin = false;
        }
    }

    if (!MR::isCorePadSwing(WPAD_CHAN0)) {
        return;
    }

    f32 distToPlayer = MR::calcDistanceToPlayer(this);
    if (distToPlayer + mAreaRadius > ::sSpinWaveDistanceMax) {
        return;
    }

    for (int i = 0; i < mPlantNum; i++) {
        if (mPlantDataArray[i].mSpinWaveSwingTimer > ::sSpinWaveValidTime) {
            continue;
        }

        f32 distPlayerToPlant = MR::getPlayerPos()->distance(mPlantDataArray[i].mPosition);

        if (distPlayerToPlant > ::sSpinWaveDistanceMax) {
            continue;
        }

        TVec3f dirPlantToPlayer(mPlantDataArray[i].mPosition - *MR::getPlayerPos());
        MR::normalizeOrZero(&dirPlantToPlayer);

        mPlantDataArray[i].mTempSpinSwingDir.set(dirPlantToPlayer);

        s32 newSpinWaveTime = static_cast< s32 >(distPlayerToPlant * 0.1f);

        // inline needed to get the correct comparison "optimization" to appear
        if (mPlantDataArray[i].mSpinWaveTimer > 0 && mPlantDataArray[i].newSpinWaveTimerIsGreater(newSpinWaveTime)) {
            continue;
        }

        mPlantDataArray[i].mSpinWaveTimer = newSpinWaveTime;
        mPlantDataArray[i].mSpinSwingStrength =
            MR::getEaseInValue((::sSpinWaveDistanceMax - distPlayerToPlant) / ::sSpinWaveDistanceMax, 0.0f, 1.0f, 1.0f);
        mIsSpin = true;
    }
}

void WaterPlant::draw() const {
    if (!MR::isValidDraw(this)) {
        return;
    }

    getWaterPlantDrawInit()->loadTex(mType);
    drawDirect();
}

void WaterPlant::initPlant() {
    mPlantDataArray = new WaterPlant::PlantData[mPlantNum];
    MR::setClippingTypeSphere(this, mAreaRadius);
}

WaterPlant::PlantData::PlantData() {
}

void WaterPlant::initPlantAfterPlacement() {
    s32 index = 0;
    TPos3f mtx;
    mtx.identity();
    MR::makeMtxRotateY(mtx, 15.0f);
    TVec3f vec(0.0f, 0.0f, 1.0f);

    int i = 0;
    if (mPlantNum > 0) {
        for (; i < mPlantNum; i++) {
            f32 randomBounds = mAreaRadius;
            TVec3f* currentPlantPos = &mPlantDataArray[i].mPosition;
            for (int j = 0; j < ::sPlantAddTime; j++) {
                currentPlantPos->set< f32 >(MR::getRandom(-randomBounds, randomBounds) + mPosition.x, 500.0f + mPosition.y,
                                            MR::getRandom(-randomBounds, randomBounds) + mPosition.z);

                if (MR::calcMapGround(*currentPlantPos, currentPlantPos, 1000.0f)) {
                    break;
                }
            }

            mPlantDataArray[i].mRotation.set(vec);
            mPlantDataArray[i].mSpinSwingDir.zero();
            mPlantDataArray[i].mTempSpinSwingDir.zero();
            mPlantDataArray[i].mSpinWaveSwingVec1.zero();
            mPlantDataArray[i].mSpinWaveSwingVec2.zero();
            mPlantDataArray[i].mSpinWaveSwingVec3.zero();
            mPlantDataArray[i].mSpinWaveTimer = 0;
            mPlantDataArray[i].mSpinWaveSwingTimer = 0;
            mPlantDataArray[i].mSpinSwingStrength = 0.0f;

            f32 random = MR::getRandom(mHeight, 2.0f * mHeight);
            mPlantDataArray[i].mHeight1 = random * ::sHeightRate0;
            mPlantDataArray[i].mHeight2 = random * ::sHeightRate1;
            mPlantDataArray[i].mHeight3 = random;

            mPlantDataArray[i].mSwingStrength1 = &getWaterPlantDrawInit()->mSwingPosTable[index + 6];
            mPlantDataArray[i].mSwingStrength2 = &getWaterPlantDrawInit()->mSwingPosTable[index + 3];
            mPlantDataArray[i].mSwingStrength3 = &getWaterPlantDrawInit()->mSwingPosTable[index];

            index = (index + ::sSwingPosTableSize - 4) % (::sSwingPosTableSize - 6 - 1);

            mtx.mult(vec, vec);
        }
    }
}

void WaterPlant::drawDirect() const {
    for (int i = 0; i < mPlantNum; i++) {
        TVec3f* position(&mPlantDataArray[i].mPosition);
        TVec3f drawVec1(mPlantDataArray[i].mRotation);
        TVec3f drawVec2(drawVec1);
        TVec3f drawVec3(drawVec1);
        drawVec1.scale(*mPlantDataArray[i].mSwingStrength1);
        drawVec2.scale(*mPlantDataArray[i].mSwingStrength2);
        drawVec3.scale(*mPlantDataArray[i].mSwingStrength3);

        drawVec1.add(*position);
        drawVec2.add(*position);
        drawVec3.add(*position);

        if (mPlantDataArray[i].mSpinWaveSwingTimer > 0) {
            drawVec1.add(mPlantDataArray[i].mSpinWaveSwingVec1);
            drawVec2.add(mPlantDataArray[i].mSpinWaveSwingVec2);
            drawVec3.add(mPlantDataArray[i].mSpinWaveSwingVec3);
        }

        drawVec1.y += mPlantDataArray[i].mHeight1;
        drawVec2.y += mPlantDataArray[i].mHeight2;
        drawVec3.y += mPlantDataArray[i].mHeight3;

        f32 drawOffsetX = mHalfWidth * ::sDrawVec.x;
        f32 drawOffsetZ = mHalfWidth * ::sDrawVec.z;
        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 8);
        GXPosition3f32(drawVec3.x - drawOffsetX, drawVec3.y, drawVec3.z - drawOffsetZ);
        GXPosition1x8(0);
        GXPosition3f32(drawVec3.x + drawOffsetX, drawVec3.y, drawVec3.z + drawOffsetZ);
        GXPosition1x8(1);
        GXPosition3f32(drawVec2.x - drawOffsetX, drawVec2.y, drawVec2.z - drawOffsetZ);
        GXPosition1x8(2);
        GXPosition3f32(drawVec2.x + drawOffsetX, drawVec2.y, drawVec2.z + drawOffsetZ);
        GXPosition1x8(3);
        GXPosition3f32(drawVec1.x - drawOffsetX, drawVec1.y, drawVec1.z - drawOffsetZ);
        GXPosition1x8(4);
        GXPosition3f32(drawVec1.x + drawOffsetX, drawVec1.y, drawVec1.z + drawOffsetZ);
        GXPosition1x8(5);
        GXPosition3f32(position->x - drawOffsetX, position->y, position->z - drawOffsetZ);
        GXPosition1x8(6);
        GXPosition3f32(position->x + drawOffsetX, position->y, position->z + drawOffsetZ);
        GXPosition1x8(7);
        GXEnd();
    }
}

WaterPlantDrawInit::WaterPlantDrawInit()
    : NameObj("水草の描画初期化"), mAngleOffset(), mSwingPosTable(), mPlantATex(), mPlantBTex(), mPlantCTex(), mPlantDTex() {
    MR::connectToScene(this, MR::MovementType_MapObj, -1, -1, -1);

    mPlantATex = new JUTTexture(MR::loadTexFromArc("WaterPlant.arc", "WaterPlantA.bti"), nullptr);
    mPlantBTex = new JUTTexture(MR::loadTexFromArc("WaterPlant.arc", "WaterPlantB.bti"), nullptr);
    mPlantCTex = new JUTTexture(MR::loadTexFromArc("WaterPlant.arc", "WaterPlantC.bti"), nullptr);
    mPlantDTex = new JUTTexture(MR::loadTexFromArc("WaterPlant.arc", "WaterPlantD.bti"), nullptr);

    MR::registerPreDrawFunction(MR::Functor(this, &WaterPlantDrawInit::initDraw), MR::DrawType_WaterPlant);

    mSwingPosTable = new f32[::sSwingPosTableSize];
    updateSwingPos();
}

void WaterPlantDrawInit::movement() {
    updateSwingPos();
    mAngleOffset += ::sSwingSpeed;
    if (mAngleOffset > TWO_PI) {
        mAngleOffset = 0.0f;
    }

    if (::sSwingSpeed > ::sSwingSpeedMax) {
        ::sSwingSpeed -= ::sSwingSpeedDown;
        if (::sSwingSpeed < ::sSwingSpeedMin) {
            ::sSwingSpeed = ::sSwingSpeedMin;
        }
    } else {
        ::sSwingSpeed = ::sSwingSpeedMax;
    }

    if (::sSwingWidth > ::sSwingWidthMax) {
        ::sSwingWidth -= ::sSwingWidthDown;
        if (::sSwingWidth < ::sSwingWidthMin) {
            ::sSwingWidth = ::sSwingWidthMin;
        }
    } else {
        ::sSwingWidth = ::sSwingWidthMax;
    }

    TPos3f inverseViewMtx;
    PSMTXInverse(MR::getCameraViewMtx(), inverseViewMtx);
    // normal getXDir call here reverses float load order
    ::sDrawVec.set< f32 >(inverseViewMtx[0][0], inverseViewMtx[1][0], inverseViewMtx[2][0]);
}

void WaterPlantDrawInit::loadTex(int tex) {
    if (tex == 1) {
        mPlantBTex->load(GX_TEXMAP0);
        return;
    }

    if (tex == 2) {
        mPlantCTex->load(GX_TEXMAP0);
        return;
    }

    if (tex == 3) {
        mPlantDTex->load(GX_TEXMAP0);
        return;
    }

    mPlantATex->load(GX_TEXMAP0);
}

void WaterPlantDrawInit::updateSwingPos() {
    f32 angle = 0.0f;
    for (int i = 0; i < ::sSwingPosTableSize; i++) {
        mSwingPosTable[i] = ::sSwingWidth * MR::sin(mAngleOffset + angle);
        angle += 0.2f;
    }
}

void WaterPlantDrawInit::initDraw() const {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_INDEX8);
    GXSetArray(GX_VA_TEX0, &::gxArray, sizeof(TVec2f));
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_VTX, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GXSetAlphaCompare(GX_GREATER, 50, GX_AOP_OR, GX_GREATER, 50);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetZCompLoc(GX_FALSE);
    GXSetCullMode(GX_CULL_NONE);

    GXSetFog(GX_FOG_LIN, ::sFogStartZ, ::sFogEndZ, MR::getNearZ(), MR::getFarZ(), (GXColor){0x32, 0x32, 0x96, 0xFF});
}
