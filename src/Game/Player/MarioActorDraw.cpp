#include "Game/LiveActor/DisplayListMaker.hpp"
#include "Game/LiveActor/ModelManager.hpp"
#include "Game/MapObj/CollectCounter.hpp"
#include "Game/MapObj/IceStep.hpp"
#include "Game/Player/DLchanger.hpp"
#include "Game/Player/DrawAdaptor.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Player/TornadoMario.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/FootPrint.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DJoint.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include "JSystem/J3DGraphBase/J3DTexture.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include <cstring>
#include <revolution/gd/GDBase.h>

namespace MR {
    void showMaterial(J3DModel*, const char*);
    template < class T >
    FunctorV0M< const T*, void (T::*)() const > Functor(const T*, void (T::*)() const) NO_INLINE;
};  // namespace MR

extern "C" {
void GDSetTexImgPtr(GXTexMapID, void*);
}

class JetTurtleShadow : public LiveActor {
public:
    void drawType0() const;
};

void MarioActor::initDrawAndModel() {
    _218 = new DrawAdaptor(MR::Functor(this, &MarioActor::drawShadow), MR::DrawType_AlphaShadow);
    _21C = new DrawAdaptor(MR::Functor(this, &MarioActor::drawSilhouette), MR::DrawType_0x28);
    _220 = new DrawAdaptor(MR::Functor(this, &MarioActor::drawPreWipe), MR::DrawType_CometScreenFilter);
    _228 = new DrawAdaptor(MR::Functor(this, &MarioActor::drawScreenBlend), MR::DrawType_CenterScreenBlur);
    _22C = new DrawAdaptor(MR::Functor(this, &MarioActor::drawIndirect), MR::DrawType_0x24);

    if (gIsLuigi) {
        initModelManagerWithAnm("Luigi", "MarioAnime", true);
    } else {
        initModelManagerWithAnm("Mario", "MarioAnime", true);
    }

    J3DModelX* baseModel = static_cast< J3DModelX* >(MR::getJ3DModel(this));
    baseModel->_DD = 8;

    for (u32 i = 0; i < baseModel->_DD; i++) {
        baseModel->mExtraMtxBuffer[i] = new (0x20) Mtx[0xC00 / sizeof(Mtx)];
    }

    MR::initDLMakerFog(this, true);
    MR::newDifferedDLBuffer(this);

    mModels[0] = baseModel;
    _9E4 = nullptr;
    _9C0 = nullptr;
    _B4C = nullptr;
    _9C8 = nullptr;
    _A00 = nullptr;
    _A04 = nullptr;
    mTornadoMario = nullptr;
    initFireBall();

    _3D2 |= 0x4;

    initBeeMario();
    initIceMario();
    initInvincibleMario();
    initHopperMario();
    initTeresaMario();
    _3D2 |= 0x80;
    initBoneMario();
    initTornadoMario();
    initBlur();

    mCurrModel = 0;
    _A0B = 0;
    initShadow();
    initHand();

    initFace();

    mDL[0] = new (0x20) u8[0x100];
    mDL[1] = new (0x20) u8[0x100];
    mCurrDL = 0;

    mDLchanger = new DLchanger(2, 0x100);
    _1A4 = 0.0f;

    swapTextureInit();
    createRainbowDL();

    {
        MR::CurrentHeapRestorer restorer(static_cast< JKRHeap* >(MR::getSceneHeapGDDR3()));
        _B7C = new JUTTexture(0x80, 0x40, GX_TF_RGB565);

        for (u32 i = 0; i < 2; i++) {
            mMaskTextures[i] = new JUTTexture(8, 8, GX_TF_IA4);
            mMaskTextures[i]->mWrapS = 1;
            mMaskTextures[i]->mWrapT = 1;
            mMaskTextures[i]->mMinType = 0;
            mMaskTextures[i]->mMagType = 0;
            mMaskTextures[i]->init();
        }

        _B88 = 0;
        updateRandomTexture(1000.0f);
        updateRandomTexture(1000.0f);
    }

    initScreenBox();
    MR::startBtp(this, "ElementEnd");
}

void MarioActor::initBeeMario() {
    const char* modelName;
    const char* archiveName;
    if (gIsLuigi) {
        modelName = "BeeLuigi";
        archiveName = "BeeLuigi.arc";
    } else {
        modelName = "BeeMario";
        archiveName = "BeeMario.arc";
    }
    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    if (!MR::isLoadedObjectOrLayoutArchive("BeeMarioWing")) {
        return;
    }

    MR::createAndAddResourceHolder(archiveName);

    _9E4 = new ModelHolder(modelName, true);
    _9E4->initWithoutIter();

    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(_9E4));
    model->copyExtraMtxBuffer(mModels[0]);

    MR::initDLMakerFog(_9E4, true);
    MR::newDifferedDLBuffer(_9E4);

    mModels[2] = model;
    _9E4->kill();
    MR::hideModel(_9E4);
    _9EC = MR::initFurPlayer(_9E4);
    MR::hideJoint(_9E4, "Face0");

    _9E8 = new MarioParts(this, "B-WING", "BeeMarioWing", false, nullptr, nullptr);
    _9E8->initWithoutIter();

    _9E8->initFixedPosition(_DDC.toMtxPtr(), TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));
    _9E8->makeActorDead();

    _9F0 = 0;
    _3D2 |= 0x10;
}

void MarioActor::initHopperMario() {
    const char* modelName;
    const char* archiveName;
    if (gIsLuigi) {
        modelName = "HopperLuigi";
        archiveName = "HopperLuigi.arc";
    } else {
        modelName = "HopperMario";
        archiveName = "HopperMario.arc";
    }

    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    MR::createAndAddResourceHolder(archiveName);

    _A00 = new ModelHolder(modelName, true);
    _A00->initWithoutIter();

    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(_A00));
    model->copyExtraMtxBuffer(mModels[0]);

    MR::initDLMakerFog(_A00, true);
    MR::newDifferedDLBuffer(_A00);

    mModels[5] = model;
    _A00->kill();
    MR::hideModel(_A00);
    MR::hideJoint(_A00, "Face0");
    _3D2 |= 0x20;
}

void MarioActor::initIceMario() {
    const char* modelName;
    const char* archiveName;
    if (gIsLuigi) {
        modelName = "IceLuigi";
        archiveName = "IceLuigi.arc";
    } else {
        modelName = "IceMario";
        archiveName = "IceMario.arc";
    }

    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    MR::createAndAddResourceHolder(archiveName);

    _9C0 = new ModelHolder(modelName, true);
    _9C0->initWithoutIter();

    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(_9C0));
    model->copyExtraMtxBuffer(mModels[0]);
    model->copyAnmMtxBuffer(mModels[0]);

    MR::initDLMakerFog(_9C0, true);
    MR::newDifferedDLBuffer(_9C0);

    mModels[3] = model;
    _9C0->kill();
    MR::hideModel(_9C0);

    _B4C = new IceStep*[20];
    for (u32 i = 0; i < 20; i++) {
        _B4C[i] = new IceStep("IceStep");
        _B4C[i]->initWithoutIter();
        _B4C[i]->kill();
    }

    _B50 = 0;
    _3D2 |= 0x8;
}

void MarioActor::initInvincibleMario() {
    const char* modelName;
    const char* archiveName;
    if (gIsLuigi) {
        modelName = "InvincibleLuigi";
        archiveName = "InvincibleLuigi.arc";
    } else {
        modelName = "InvincibleMario";
        archiveName = "InvincibleMario.arc";
    }

    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    MR::createAndAddResourceHolder(archiveName);

    _9C8 = new ModelHolder(modelName, true);
    _9C8->initWithoutIter();

    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(_9C8));
    model->copyExtraMtxBuffer(mModels[0]);
    model->copyAnmMtxBuffer(mModels[0]);

    MR::initDLMakerFog(_9C8, true);
    MR::newDifferedDLBuffer(_9C8);

    mModels[4] = model;
    _9C8->kill();
    MR::hideModel(_9C8);
    MR::startBrk(_9C8, "InvincibleMario");
    _3D2 |= 0x2;
}

void MarioActor::initTeresaMario() {
    const char* modelName = gIsLuigi ? "TeresaLuigi" : "TeresaMario";
    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    _9A4 = new MarioParts(this, "テレサの皮", modelName, nullptr);
    if (gIsLuigi) {
        _9A4->_9C = "TeresaMario";
    }
    _9A4->initWithoutIter();
    _9A4->kill();

    _9A4->initFixedPosition(TVec3f(0.0f, 0.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);

    _9A8 = 0.0f;
    _9AC = 0.0f;
    _9B4 = 0;

    MR::initShadowVolumeSphere(_9A4, 90.0f);
    _3D2 |= 0x40;
    initTeresaMarioAnimation();
}

void MarioActor::initTornadoMario() {
    const char* modelName = gIsLuigi ? "LuigiTornado" : "MarioTornado";
    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    mTornadoMario = new TornadoMario(modelName);
    mTornadoMario->initWithoutIter();
    _990 = 0;
    _3D2 |= 0x200;
}

void MarioActor::initBoneMario() {
    const char* modelName = gIsLuigi ? "BoneLuigi" : "BoneMario";

    _A04 = new ModelHolder(modelName, true);
    _A04->initWithoutIter();

    J3DModelX* model = static_cast< J3DModelX* >(MR::getJ3DModel(_A04));
    model->copyExtraMtxBuffer(mModels[0]);

    MR::initDLMakerFog(_A04, true);
    MR::newDifferedDLBuffer(_A04);

    mModels[1] = model;
    _A04->kill();
    MR::hideModel(_A04);
    _3D2 |= 0x100;
}

void MarioActor::changeDisplayMode(u8 mode) {
    mCurrModel = mode;
    mMarioAnim->mXanimePlayer->setModel(mModels[mCurrModel]);
    mMarioAnim->mXanimePlayerUpper->setModel(mModels[mCurrModel]);

    mModelManager->mDisplayListMaker->mModel = mModels[mCurrModel];

    if (_494) {
        MtxPtr handMtx = MR::getJointMtx(this, "HandR");
        _494->setBaseMtx(handMtx);
    }

    MtxPtr handMtx = MR::getJointMtx(this, "HandR");
    mSearchLightThrowPos->setBaseMtx(handMtx);
}

void MarioActor::calcViewAndEntry() {
    decideShadowMode();

    J3DModelX* model = getJ3DModel();
    model->mFlags._1C = false;
    model->swapDrawBuffer(4);
    getSimpleModel()->swapDrawBuffer(4);

    calcViewMainModel();
    calcViewSilhouetteModel();
    calcViewWallShadowModel();
    model->setDrawView(0);
    calcViewBlurModel();

    if (_B48) {
        calcViewFootPrint();
    }

    if (mMario->isStatusActive(MarioStatus_FpView)) {
        calc1stPersonView();
    } else {
        calcFogLighting();
    }

    switch (mCurrModel) {
    case 0:
        swapTexture("EyeLid_v", static_cast< u8 >(_B6A));
        break;
    case 2:
        swapTexture("EyeLid_v", static_cast< u8 >(_B6A + 4));
        break;
    case 5:
        swapTexture("EyeLid_v", static_cast< u8 >(_B6A + 8));
        break;
    default:
        break;
    }

    if (_A08 > 1) {
        calcViewReflectionModel();
    } else {
        _A24 = 0;
        _A25 = 0;
    }

    calcViewSearchLight();

    if (_A40) {
        _A40->updateMtx(_C8C.toMtxPtr());
    }

    if (_A44) {
        _A44->updateMtx(_CBC.toMtxPtr());
    }

    _A48->updateMtx(_C8C.toMtxPtr());

    _A4C->updateMtx(_CBC.toMtxPtr());

    if (_A50) {
        _A50->updateMtx(_C8C.toMtxPtr());
    }

    if (_A54) {
        _A54->updateMtx(_CBC.toMtxPtr());
    }

    _A5C->updateMtx(_CEC.toMtxPtr());

    updateDarkMask(0x96);
    updateDarkMask(0x96);

    if (MR::isDemoActive()) {
        _1B8->kill();
    }
}

void MarioActor::drawMarioModel() const {
    if (isAllHidden()) {
        return;
    }

    if (mTornadoMario) {
        if (mMario->mMovementStates._F) {
            if (mMario->_544 > 2) {
                return;
            }
        }

        if (!MR::isDead(mTornadoMario) &&
            (MR::isBckPlaying(mTornadoMario, "MarioTornadoEnd") || MR::isBckPlaying(mTornadoMario, "MarioTornadoLoop"))) {
            return;
        }
    }

    switch (mCurrModel) {
    case 3:
        return;
    default:
        break;
        return;
    }

    drawSpinInhibit();
    drawSphereMask();
    bool res = drawDarkMask();

    J3DModelX* model = mModels[mCurrModel];

    if (res) {
        model->mFlags._10 = true;
    }

    if (!mFlag.mIsHiddenModel) {
        model->mFlags._1C = false;
        if (mMario->isStatusActive(MarioStatus_FpView)) {
            if (_1A1) {
                return;
            }

            mDLchanger->addDL(model);

            MR::showJoint(model, "HandR0");
            MR::showJoint(model, "HandL0");
            MR::showJoint(model, "Face0");
        } else {
            if (mCurrModel == 4) {
                model->setDynamicDL(nullptr, 0);
            } else {
                model->setDynamicDL(mDL[mCurrDL], mDLSize);
            }
        }

        if (mBeeWallWalk != 0) {
            GXSetAlphaUpdate(GX_TRUE);
            GXSetDstAlpha(GX_TRUE, 0);
        }
        model->setDrawView(0);
        model->directDraw(nullptr);
        model->mFlags.clear();
    }

    if (mMario->isStatusActive(MarioStatus_FpView)) {
        MR::hideJoint(model, "HandR0");
        MR::hideJoint(model, "HandL0");
        MR::hideJoint(model, "Face0");
    }

    drawHand();

    if (!MR::isHiddenModel(_A5C)) {
        J3DModelX* cool = static_cast< J3DModelX* >(MR::getJ3DModel(_A5C));
        cool->setDynamicDL(mDL[mCurrDL], mDLSize);
        cool->directDraw(nullptr);
    }

    if (mBeeWallWalk != 0) {
        GXSetAlphaUpdate(GX_FALSE);
        GXSetDstAlpha(GX_FALSE, 0);
    }
}

bool MarioActor::isAllHidden() const {
    bool hidden = false;
    if (_482 || _483 || _481) {
        hidden = true;
    }

    return hidden;
}

void MarioActor::swapTextureInit() {
    J3DModelData* actorData = MR::getJ3DModelData(this);
    const u16 textureCount = actorData->mMaterialTable.mTexture->getNum();
    _B60 = textureCount;
    _B64 = new ResTIMG*[textureCount];
    _B6A = 0;

    for (u32 i = 0; i < textureCount; i++) {
        _B64[i] = MR::getResTIMG(this, i);
    }

    _B6C = new DLholder[12];

    u16 texNo = 0;
    for (u16 i = 0; i < textureCount; i++) {
        J3DModelData* modelData = MR::getJ3DModelData(this);
        const char* texName = modelData->mMaterialTable.mTextureName->getName(i);
        if (strcmp(texName, "mario_eyeLid.0") == 0) {
            texNo = i;
            break;
        }
    }

    _B70 = texNo;
    createTextureDL(&_B6C[0], 0, texNo);
    createTextureDL(&_B6C[1], 0, texNo + 1);
    createTextureDL(&_B6C[2], 0, texNo + 2);
    createTextureDL(&_B6C[3], 0, texNo + 3);

    if (_9E4) {
        J3DModelData* beeData = MR::getJ3DModelData(_9E4);
        const u16 textureNum = beeData->mMaterialTable.mTexture->getNum();

        for (u16 i = 0; i < textureNum; i++) {
            J3DModelData* modelData = MR::getJ3DModelData(_9E4);
            const char* texName = modelData->mMaterialTable.mTextureName->getName(i);
            if (strcmp(texName, "mario_eyeLid.0") == 0) {
                texNo = i;
                break;
            }
        }

        for (u32 i = 0; i < textureNum; i++) {
            _B64[i] = MR::getResTIMG(_9E4, i);
        }

        createTextureDL(&_B6C[4], 0, texNo);
        createTextureDL(&_B6C[5], 0, texNo + 1);
        createTextureDL(&_B6C[6], 0, texNo + 2);
        createTextureDL(&_B6C[7], 0, texNo + 3);
    }

    if (_A00) {
        J3DModelData* hopperData = MR::getJ3DModelData(_A00);
        const u16 textureNum = hopperData->mMaterialTable.mTexture->getNum();

        for (u16 i = 0; i < textureNum; i++) {
            J3DModelData* modelData = MR::getJ3DModelData(_A00);
            const char* texName = modelData->mMaterialTable.mTextureName->getName(i);
            if (strcmp(texName, "mario_eyeLid.0") == 0) {
                texNo = i;
                break;
            }
        }

        for (u32 i = 0; i < textureNum; i++) {
            _B64[i] = MR::getResTIMG(_A00, i);
        }

        createTextureDL(&_B6C[8], 0, texNo);
        createTextureDL(&_B6C[9], 0, texNo + 1);
        createTextureDL(&_B6C[10], 0, texNo + 2);
        createTextureDL(&_B6C[11], 0, texNo + 3);
    }

    initBlink();
}

void MarioActor::initFace() {
    _A5B = 8;
    MR::hideJoint(mModels[0], "Face0");
    _A60 = 0;

    if (gIsLuigi) {
        _A5C = new ModelHolder("LuigiFace", true);
    } else {
        _A5C = new ModelHolder("MarioFace", true);
    }

    MR::initDLMakerFog(_A5C, true);
    MR::newDifferedDLBuffer(_A5C);

    MR::getJ3DModelData(_A5C)->mMaterialTable.mTexture = MR::getJ3DModelData(this)->mMaterialTable.mTexture;
    MR::getJ3DModelData(_A5C)->mMaterialTable.mTextureName = MR::getJ3DModelData(this)->mMaterialTable.mTextureName;

    for (u32 i = 0; i < _A5B; i++) {
        copyMaterial(MR::getJ3DModel(_A5C), MR::getJointIndex(this, "Face0"), i);
    }
}

void MarioActor::updateFace() {
    switch (mCurrModel) {
    case 1:
    case 3:
    case 4:
        MR::hideModel(_A5C);
        return;
    }

    if (mMario->isStatusActive(MarioStatus_FpView)) {
        MR::hideModel(_A5C);
        return;
    }

    if (MR::isHiddenModel(this) || is481or482On() || mMario->mMovementStates._F || _1C3) {
        MR::hideModel(_A5C);
        return;
    }

    MR::showModel(_A5C);

    TVec3f partsControl;
    if (mMarioAnim->_6C) {
        XanimePlayer* player = mMarioAnim->mXanimePlayerUpper;
        const s32 partsControlJoint = MR::getJointIndex(this, "PartsControl");
        player->getMainAnimationTrans(static_cast< u32 >(partsControlJoint), &partsControl);
    } else {
        XanimePlayer* player = mMarioAnim->mXanimePlayer;
        const s32 partsControlJoint = MR::getJointIndex(this, "PartsControl");
        player->getMainAnimationTrans(static_cast< u32 >(partsControlJoint), &partsControl);
    }

    _A60 = MR::clamp(static_cast< s32 >(0.49f + partsControl.x / 0.5f), 0, _A5B - 1);

    for (u16 i = 0; i < _A5B; i++) {
        J3DJoint* eyeJoint = MR::getJoint(_A5C, i + 1);
        if (_A60 == i) {
            MR::showJoint(eyeJoint);
        } else {
            MR::hideJoint(eyeJoint);
        }
    }
}

void MarioActor::initBlur() {
    _B14 = 0;
    for (u32 i = 0; i < 6; i++) {
        if (mModels[i] != nullptr) {
            const u16 jointNum = mModels[i]->mModelData->getDrawMtxNum();
            if (jointNum > _B14) {
                _B14 = jointNum;
            }
        }
    }

    for (u32 i = 0; i < 2; i++) {
        for (u32 j = 0; j < 8; j++) {
            mBlurMatrices[i][j] = new (0x20) Mtx[_B14];

            for (u32 k = 0; k < _B14; k++) {
                PSMTXIdentity(mBlurMatrices[i][j][k]);
            }
        }
    }

    _A6E = false;
    _B12 = 0;
    _B10 = 0;
}

J3DModelX* MarioActor::getJ3DModel() const {
    return mModels[mCurrModel];
}

J3DModelData* MarioActor::getModelData() const {
    return getJ3DModel()->mModelData;
}

J3DModelX* MarioActor::getSimpleModel() const {
    return _A14;
}

void MarioActor::calcViewMainModel() {
    J3DModelX* model = mModels[mCurrModel];
    if (mFlag.mIsNoCalcView) {
        return;
    }

    model->viewCalc3(0, nullptr);

    Mtx invView;
    Mtx invBase;
    PSMTXInverse(_BF8, invView);
    PSMTXInverse(mModels[mCurrModel]->mBaseTransformMtx, invBase);
    MR::multMtx(_BC8, invBase, invView);
}

void MarioActor::draw() const {
    if (_B48) {
        _B48->draw();
    }

    if (!is481or482On() && !_A61 && !_1C3) {
        if (_A0C) {
            drawWallShade(_1F0, _1FC, _208);
        }

        drawMarioModel();
        mMario->draw();
    }
}

void MarioActor::drawIndirect() const {
    drawModelBlur();
    drawIndirectModel();

    if (mMario->mDrawStates._3) {
        drawColdWaterDamage();
    }

    if (_1E4 != 0.0f) {
        drawRasterScroll(_1E4, static_cast< s16 >(_1E8), _1EC);
    }

    if (_390) {
        drawMosaic();
    }

    if (_3DA) {
        drawLifeUp();
    }

    drawSpinEffect();
}

void MarioActor::drawIndirectModel() const {
    if (isAllHidden()) {
        return;
    }

    drawReflectModel();

    if (mTornadoMario) {
        if (mMario->mMovementStates._F) {
            if (mMario->_544 > 2) {
                return;
            }
        }

        if (!MR::isDead(mTornadoMario) &&
            (MR::isBckPlaying(mTornadoMario, "MarioTornadoEnd") || MR::isBckPlaying(mTornadoMario, "MarioTornadoLoop"))) {
            return;
        }
    }

    switch (mCurrModel) {
    case 3:
        break;
    default:
        return;
    }

    drawSpinInhibit();

    J3DModelX* model = getJ3DModel();
    if (!mFlag.mIsHiddenModel) {
        model->mFlags._1C = false;
        if (mMario->isStatusActive(MarioStatus_FpView)) {
            if (_1A1) {
                return;
            }

            mDLchanger->addDL(model);
        } else {
            model->setDynamicDL(mDL[mCurrDL], mDLSize);
        }

        MR::showJoint(model, "Face0");
        model->setDrawView(0);
        model->directDraw(nullptr);
        model->mFlags.clear();
    }

    drawIndirectHand();
}

void MarioActor::drawReflectModel() const {
    if (isAllHidden()) {
        return;
    }

    if (_A08 >= 6 && _A24 < 2) {
        return;
    }

    if (_A08 == 3 && _A25 < 2) {
        return;
    }

    if (!(_A08 & 2)) {
        return;
    }

    if (isUseScreenBox()) {
        captureScreenBox();
    }

    J3DModelX* model = getJ3DModel();

    MR::showJoint(model, "HandR0");
    MR::showJoint(model, "HandL0");
    MR::showJoint(model, "Face0");

    if (mCurrModel != 1) {
        MR::hideMaterial(model, "EyeLid_v");
    }

    if (_468 == 0 ? nullptr : _428[0]) {
        if (isUseScreenBox()) {
            mDLchanger->addDL(static_cast< J3DModelX* >(MR::getJ3DModel(_9A0)));
        }

        _9A0->drawType0();
    }

    model->mFlags.clear();

    if (_A08 == 2) {
        GXSetZScaleOffset(1.0f, 0.00001f);
        model->mFlags._1A = true;
        model->setDrawView(3);
        model->directDraw(nullptr);
        model->mFlags._1A = false;
        model->mFlags._1D = true;
        mDLchanger->addDL(model);
    } else {
        if (isUseScreenBox()) {
            mDLchanger->addDL(model);
        }

        model->mFlags._14 = true;
    }

    GXSetZScaleOffset(1.0f, 0.0f);
    model->setDrawView(3);
    model->directDraw(nullptr);
    model->mFlags.clear();

    MR::hideJoint(model, "HandR0");
    MR::hideJoint(model, "HandL0");

    if (mCurrModel != 4) {
        MR::hideJoint(model, "Face0");
    }

    if (mCurrModel != 1) {
        MR::showMaterial(model, "EyeLid_v");
    }

    if (isUseScreenBox()) {
        writeBackScreenBox();
    }
}

void MarioActor::drawModelBlur() const {
    if (isAllHidden()) {
        return;
    }

    J3DModelX* model = mModels[mCurrModel];
    if (!model->_1E4) {
        model->_1E5 = true;
        return;
    }

    model->_1E4 = false;
    if (!_A6E) {
        return;
    }

    model->mFlags._13 = true;

    Mtx inv;
    PSMTXInverse(const_cast< TMtx34f& >(_AB0).toMtxPtr(), inv);
    MR::multMtx(inv, inv, MR::getCameraViewMtx());
    for (u32 i = 1; i < 8; i++) {
        const u32 idx = static_cast< u32 >(i + _B12) & 7;
        model->setDrawViewBuffer(mBlurMatrices[_B10][idx][0]);

        if (!_1C1) {
            for (u32 joint = 0; joint < getModelData()->getDrawMtxNum(); joint++) {
                MR::multMtx(mBlurMatrices[_B10][idx][joint], mBlurMatrices[1 - _B10][idx][joint], inv);
            }
        }

        DCStoreRange(mBlurMatrices[_B10][idx], getModelData()->getDrawMtxNum() * sizeof(Mtx));

        _94[idx + ((i - 1) << 3)]->addDL(model);
        model->directDraw(nullptr);
    }

    model->mFlags._13 = false;
}

void MarioActor::createTextureDL(DLholder* pHolder, u16 texMapID, u16 texIndex) {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    u8 tempDL[0x200] ATTRIBUTE_ALIGN(32);
    GDLObj obj;
    GDInitGDLObj(&obj, tempDL, sizeof(tempDL));
    __GDCurrentDL = &obj;

    ResTIMG* texture = _B64[texIndex];
    GDSetTexImgPtr(static_cast< GXTexMapID >(texMapID), reinterpret_cast< u8* >(texture) + texture->mImageDataOffset);

    GDPadCurr32();

    const u32 dlSize = static_cast< u32 >(obj.ptr - obj.start);
    const u32 alignedSize = (dlSize + 0x1F) & ~0x1F;

    pHolder->mSize = dlSize;
    pHolder->mDL = new (0x20) u8[alignedSize];

    MR::copyMemory(pHolder->mDL, tempDL, alignedSize);
    DCStoreRange(pHolder->mDL, alignedSize);
}

void MarioActor::swapTexture(const char* pMaterialName, u8 texNo) const {
    switch (mPlayerMode) {
    case 4:
        if (texNo > 7) {
            return;
        }
        break;
    case 5:
        if (texNo > 11) {
            return;
        }
        break;
    default:
        if (texNo > 3) {
            return;
        }
    }

    J3DModelData* modelData = MR::getJ3DModelData(this);
    u16 materialNo = MR::getMaterialNo(modelData, pMaterialName);

    const DLholder* holder = _B6C;
    const u8 modelIdx = mCurrModel;
    u8* dl = holder[texNo].mDL;
    u16 dlSize = holder[texNo].mSize;
    J3DModelX* model = mModels[modelIdx];

    if (dl == nullptr) {
        dlSize = 0;
    }

    model->_1C8[materialNo] = dl;
    model->_1CC[materialNo] = dlSize;
}

void MarioActor::copyMaterial(J3DModel* pModel, u16 materialNo, s32 packetIndex) {
    J3DModelData* modelData = mModels[mCurrModel]->mModelData;
    J3DMaterial* material = modelData->getJointNodePointer(materialNo)->getMesh();
    if (material == nullptr) {
        return;
    }

    u16 materialIndex = material->mIndex;

    u16 start;
    u16 end;

    if (packetIndex == -1) {
        start = 0;
        end = 7;
    } else {
        start = static_cast< u16 >(packetIndex);
        end = static_cast< u16 >(packetIndex);
    }

    for (u16 i = start; i <= end; i++) {
        J3DMatPacket* matPacket = &pModel->mMatPacket[i];
        matPacket->mpMaterial = modelData->getMaterialNodePointer(materialIndex);
        J3DShapePacket* shapePacket = &pModel->mShapePacket[i];
        matPacket->mpInitShapePacket = shapePacket;
        matPacket->addShapePacket(shapePacket);
        matPacket->mpTexture = modelData->mMaterialTable.mTexture;
        matPacket->mpDisplayListObj = modelData->getMaterialNodePointer(materialIndex)->mSharedDLObj;
    }
}

namespace MR {
    template <>
    FunctorV0M< const MarioActor*, void (MarioActor::*)() const > Functor< MarioActor >(const MarioActor* pActor, void (MarioActor::*pFunc)() const) {
        return FunctorV0M< const MarioActor*, void (MarioActor::*)() const >(pActor, pFunc);
    }
};  // namespace MR

template <>
void MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const >::operator()() const {
    (mCaller->*mCallee)();
}

template <>
MR::FunctorBase* MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const >::clone(JKRHeap* pHeap) const {
    return new (pHeap, 0) FunctorV0M< const MarioActor*, void (MarioActor::*)() const >(*this);
}

void J3DModelX::copyExtraMtxBuffer(const J3DModelX* pModel) {
    _DD = pModel->_DD;

    for (u32 i = 0; i < _DD; i++) {
        mExtraMtxBuffer[i] = pModel->mExtraMtxBuffer[i];
    }
}

void J3DModelX::swapDrawBuffer(u32 drawBuffer) {
    if (_DC == drawBuffer) {
        _DC = 0;
        return;
    }

    _DC = drawBuffer;
}

void DLchanger::addDL(J3DModelX* pModel) {
    DLholder* buffer = &mBuffers[mCurrentBuffer];
    pModel->setDynamicDL(buffer->mDL, buffer->mSize);
}

void J3DModelX::setDynamicDL(u8* pDL, u32 dlSize) {
    if (pDL == nullptr) {
        dlSize = 0;
    }

    _1B8 = pDL;
    _1BC = dlSize;
}
