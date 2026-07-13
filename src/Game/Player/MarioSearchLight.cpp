#include "Game/Player/MarioSearchLight.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/DrawAdaptor.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXTev.h>
#include <revolution/mtx.h>

MarioSearchLight::MarioSearchLight(LiveActor* pActor)
    : PartsModel(pActor, "サーチライト", "SearchLightCone", nullptr, -1, 0), _9C(0.0f), _A0(0), _A4(nullptr) {
}

void MarioSearchLight::init(const JMapInfoIter& rParam1) {
    PartsModel::init(rParam1);
    initHitSensor(1);
    _9C = 0.0f;
    _A4 = nullptr;
    _A0 = 0;
    MR::addHitSensorCallback(this, "eye", ATYPE_EYE, 4, 100.0f);
}

void MarioSearchLight::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (MR::sendArbitraryMsg(ACTMES_SEARCHLIGHT_ATTACK, pReceiver, pSender)) {
        return;
    }

    if (!MR::isExistInAttributeGroupSearchTurtle(pReceiver->mHost)) {
        return;
    }

    if (pReceiver != _A4) {
        MR::tryRumblePadWeak(this, 0);
    }
    _A4 = pReceiver;
    _A0 = 30;
}

void MarioSearchLight::updateHitSensor(HitSensor* pSensor) {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);

    TVec3f frontPosition = mPosition + frontVec * _9C;

    pSensor->mPosition.x = frontPosition.x;
    pSensor->mPosition.y = mRotation.x;
    pSensor->mPosition.z = mRotation.y;

    _9C += 80.0f;
    if (_9C > 2000.0f) {
        _9C = 0.0f;
    }

    if (_A4 != nullptr) {
        if (!_A4->mValidByHost) {
            _A4 = nullptr;
        }
    }

    if (_A0 != 0) {
        _A0--;
    }

    if (_A0 == 0) {
        _A4 = nullptr;
    }
}

void MarioSearchLight::calcAndSetBaseMtx() {
    Mtx baseMtx;
    PSMTXCopy(getBaseMtx(), baseMtx);

    PartsModel::calcAndSetBaseMtx();
    MR::blendMtxRotateSlerp(baseMtx, getBaseMtx(), 0.1f, baseMtx);

    TVec3f trans;
    MR::extractMtxTrans(getBaseMtx(), &trans);
    MR::setMtxTrans(baseMtx, trans.x, trans.y, trans.z);
    MR::setBaseTRMtx(this, baseMtx);
}

void MarioActor::initThrowing() {
    mSearchLightThrowPos = new FixedPosition(this, "HandR", TVec3f(13.19f, 41.57f, 19.02f), TVec3f(-3.63f, -41.69f, 15.76f));
}

void MarioActor::updateSearchLight() {
    if (mSearchLight == nullptr) {
        return;
    }

    if (mMario->isStatusActive(MarioStatus_Swim) && (mMario->mSwim->mJetTimer != 0)) {
        if (MR::isDead(mSearchLight)) {
            mSearchLight->appear();
            MR::hideModel(mSearchLight);
            MR::emitEffect(mSearchLight, "Glow");
        }
    } else {
        mSearchLight->kill();
    }
}

void MarioActor::initSearchLight() {
    mSearchLight = new MarioSearchLight(this);
    mSearchLight->initWithoutIter();
    mSearchLight->makeActorDead();
    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(mSearchLight));
    model->_DD = 16;

    s32 drawMtxNum = model->getModelData()->getDrawMtxNum();
    for (u32 i = 0; i < model->_DD; i++) {
        model->mExtraMtxBuffer[i] = new (0x20) Mtx[drawMtxNum];
    }

    mDrawSearchLight = new DrawAdaptor(MR::Functor_Inline(this, &MarioActor::drawSearchLight), MR::DrawType_0x33);
}

void MarioActor::updateThrowing() {
    if (mSearchLightThrowPos == nullptr) {
        return;
    }

    mSearchLightThrowPos->calc();

    if (mSearchLight == nullptr) {
        return;
    }

    TVec3f trans;
    mSearchLightThrowPos->mMtx.getTrans(trans);

    // TODO: Probably missing an inline?
    MarioSearchLight* searchLight = mSearchLight;
    searchLight->mPosition = trans + mVelocity;

    mSearchLightThrowPos->copyRotate(&mSearchLight->mRotation);

    MR::calcAnimDirect(mSearchLight);
}

void MarioActor::calcViewSearchLight() {
    if (mSearchLight == nullptr) {
        return;
    }

    if (MR::isDead(mSearchLight)) {
        J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(mSearchLight));

        // The search light is has three scaled cone "shells" to fake a blur effect.
        Mtx modelMdx;
        PSMTXConcat(J3DSys::mCurrentMtx, model->getBaseTRMtx(), modelMdx);

        Mtx invBaseMtx;
        PSMTXInverse(model->getBaseTRMtx(), invBaseMtx);

        model->viewCalc3(0, nullptr);

        for (u32 i = 0; i < ARRAY_SIZE(mConst->getTable()->mSearchLightBlurScale); i++) {
            Mtx scaledMtx;
            f32 scale = mConst->getTable()->mSearchLightBlurScale[i];
            PSMTXScale(scaledMtx, scale, scale, scale);

            PSMTXConcat(modelMdx, scaledMtx, scaledMtx);
            PSMTXConcat(scaledMtx, invBaseMtx, J3DSys::mCurrentMtx);

            model->viewCalc3(i + 1, nullptr);
        }

        MR::loadViewMtx();
        mSearchLightTimer++;
    } else {
        mSearchLightTimer = 0;
    }
}

void MarioActor::drawSearchLight() const {
    if (mSearchLight == nullptr) {
        return;
    }

    if (!mMario->isStatusActive(MarioStatus_Swim)) {
        return;
    }

    if (mMario->mSwim->mJetTimer == 0) {
        return;
    }

    if (MR::isSystemTalking()) {
        return;
    }

    if (mMario->mSwim->mJetCooldownTimer > 0) {
        return;
    }

    TVec3f throwVec;
    getThrowVec(&throwVec);

    TVec2f screenPos1;
    TDDraw::project2D(&screenPos1, mPosition);

    TVec2f screenPos2;
    TDDraw::project2D(&screenPos2, mPosition + throwVec * 10000.0f);

    TBox2f drawRect;
    if (screenPos1.x >= screenPos2.x) {
        drawRect.i.x = screenPos2.x;
        drawRect.f.x = screenPos1.x;
    } else {
        drawRect.i.x = screenPos1.x;
        drawRect.f.x = screenPos2.x;
    }

    if (screenPos1.y >= screenPos2.y) {
        drawRect.i.y = screenPos2.y;
        drawRect.f.y = screenPos1.y;
    } else {
        drawRect.i.y = screenPos1.y;
        drawRect.f.y = screenPos2.y;
    }

    drawRect.i = drawRect.i + TVec2f(-10.0f, -10.0f);
    drawRect.f = drawRect.f + TVec2f(10.0f, 10.0f);

    TDDraw::setup(0, 0, 2);

    GXSetCullMode(GX_CULL_NONE);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_NOOP);
    GXSetDstAlpha(GX_TRUE, 0);

    TDDraw::drawFillBox(drawRect.i, drawRect.f, 0);
    TDDraw::setup(0, 1, 1);

    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(mSearchLight));
    J3DMaterial* material = model->getModelData()->getMaterialNodePointer(0);
    if (model->simpleDrawSetup(material)) {
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetColorUpdate(GX_FALSE);
        GXSetDstAlpha(GX_FALSE, 0);

        // Gradually fade in the search light.
        f32 lightBrightness = 1.0f;
        if (mSearchLightTimer < 30) {
            lightBrightness = 0.0f;
        } else if (mSearchLightTimer < 180) {
            lightBrightness = (mSearchLightTimer - 30) / 150.0f;
        }

        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

        GXColorS10 color = {255, 255, 255, (u8)(lightBrightness * mConst->getTable()->mSearchLightColor[3])};

        GXSetTevColorS10(GX_TEVREG0, color);

        // Render intersection of light cone with the scene by subtracting
        // visible back faces from front faces.
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
        GXSetCullMode(GX_CULL_BACK);
        model->setDrawView(0);
        model->simpleDrawShape(material);

        GXSetCullMode(GX_CULL_FRONT);
        GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ONE, GX_BL_ONE, GX_LO_NOOP);
        model->simpleDrawShape(material);

        TDDraw::setup(0, 1, 2);
        GXSetCullMode(GX_CULL_NONE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetColorUpdate(GX_TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_NOOP);
        GXSetDstAlpha(GX_TRUE, 0);

        u8* col = mConst->getTable()->mSearchLightColor;
        TDDraw::drawFillBox(drawRect.i, drawRect.f, col[0] << 24 | col[1] << 16 | col[2] << 8);

        GXSetDstAlpha(GX_FALSE, 0);
        GXSetAlphaUpdate(GX_FALSE);

        TDDraw::setup(0, 1, 1);
        GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
        GXSetAlphaCompare(GX_GREATER, 4u, GX_AOP_AND, GX_ALWAYS, 0);
        bool res = model->simpleDrawSetup(material);

        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);

        // Colour of the light cone.
        GXColorS10 searchLightCol = {255, 255, 255, (u8)(lightBrightness * mConst->getTable()->mSearchLightAlphaLevel)};
        GXSetTevColorS10(GX_TEVREG0, searchLightCol);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_A0, GX_CA_RASA);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_SUB, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

        if (res) {
            // Draw central light cone.
            model->simpleDrawShape(material);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_RASA, GX_CA_ZERO);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_DIVIDE_2, GX_TRUE, GX_TEVPREV);

            // Draw soft falloff outside light cone.
            for (u32 i = 0; i < ARRAY_SIZE(mConst->getTable()->mSearchLightBlurAlpha); i++) {
                model->setDrawView(i + 1);

                // Cone falloff colour.
                GXColorS10 color3 = {255, 255, 255, (u8)(lightBrightness * mConst->getTable()->mSearchLightBlurAlpha[i])};
                GXSetTevColorS10(GX_TEVREG0, color3);

                // End cap falloff colour.
                GXColorS10 endCapCol = {255, 255, 255, 4};
                GXSetTevColorS10(GX_TEVREG1, endCapCol);

                model->simpleDrawShape(material);
            }
        }
    }
}

void MarioActor::getThrowVec(TVec3f* pVec) const {
    TVec3f rotate;
    mSearchLightThrowPos->copyRotate(&rotate);

    TVec3f dirZ(0.0f, 0.0f, 1.0f);

    Mtx trMtx;
    MR::makeMtxTR(trMtx, 0.0f, 0.0f, 0.0f, rotate.x, rotate.y, rotate.z);

    PSMTXMultVec(trMtx, dirZ, pVec);
}
