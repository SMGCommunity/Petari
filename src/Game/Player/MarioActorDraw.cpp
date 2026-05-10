#include "Game/MapObj/IceStep.hpp"
#include "Game/Player/DLchanger.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Player/TornadoMario.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/FootPrint.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include "JSystem/J3DGraphAnimator/J3DModelData.hpp"
#include "JSystem/J3DGraphBase/J3DMaterial.hpp"
#include "JSystem/J3DGraphBase/J3DTexture.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "JSystem/JUtility/JUTNameTab.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include <cstring>
#include <new>
#include <revolution/gd/GDBase.h>

namespace MR {
void showMaterial(J3DModel*, const char*);
template < class T >
FunctorV0M< const T*, void (T::*)() const > Functor(const T*, void (T::*)() const) NO_INLINE;
}

extern "C" {
void GDSetTexImgPtr(GXTexMapID, void*);
}

struct DLholder {
    u8* mDL;
    u16 mSize;
    u16 _6;
};

class DrawAdaptor {
public:
    DrawAdaptor(const MR::FunctorBase&, int);
};

class JetTurtleShadow : public LiveActor {
public:
    void drawType0() const;
};

namespace {
struct DLBufferInfoForAddDL {
    u8* mDL;
    u16 mSize;
    u16 _6;
};

struct DLchangerForAddDL : public DLchanger {
    DLBufferInfoForAddDL* mBuffers;
    u8 _4;
    u8 mCurrentBuffer;
};

struct DisplayListMakerForModelSwap {
    u32 _0[5];
    J3DModel* mModel;
};

struct J3DMaterialForMarioActorDraw {
    u8 _0[0x14];
    u16 mIndex;
    u8 _16[0x32];
    J3DDisplayListObj* mSharedDLObj;
    u8 _4C[0xC];
    J3DMaterialForMarioActorDraw* mpOrigMaterial;
};

struct J3DModelDataForMarioActorDraw {
    u8 _0[0x28];
    J3DMaterialForMarioActorDraw** mMaterialNodePointer;
    u8 _2C[0x34];
    J3DMaterialForMarioActorDraw** mMaterialRemapTable;
    u8 _64[0x8];
    J3DTexture* mTexture;
    JUTNameTab* mTextureName;
};
}  // namespace

void MarioActor::initDrawAndModel() {
    DrawAdaptor* adaptor = nullptr;

    void* mem = ::operator new(0x10);
    if (mem) {
        MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const > functor =
            MR::Functor(static_cast< const MarioActor* >(this), &MarioActor::drawShadow);
        adaptor = new (mem) DrawAdaptor(functor, 0x29);
    }
    _218 = adaptor;

    adaptor = nullptr;
    mem = ::operator new(0x10);
    if (mem) {
        MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const > functor =
            MR::Functor(static_cast< const MarioActor* >(this), &MarioActor::drawSilhouette);
        adaptor = new (mem) DrawAdaptor(functor, 0x28);
    }
    _21C = adaptor;

    adaptor = nullptr;
    mem = ::operator new(0x10);
    if (mem) {
        MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const > functor =
            MR::Functor(static_cast< const MarioActor* >(this), &MarioActor::drawPreWipe);
        adaptor = new (mem) DrawAdaptor(functor, 0x41);
    }
    _220 = adaptor;

    adaptor = nullptr;
    mem = ::operator new(0x10);
    if (mem) {
        MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const > functor =
            MR::Functor(static_cast< const MarioActor* >(this), &MarioActor::drawScreenBlend);
        adaptor = new (mem) DrawAdaptor(functor, 0x2F);
    }
    _228 = adaptor;

    adaptor = nullptr;
    mem = ::operator new(0x10);
    if (mem) {
        MR::FunctorV0M< const MarioActor*, void (MarioActor::*)() const > functor =
            MR::Functor(static_cast< const MarioActor* >(this), &MarioActor::drawIndirect);
        adaptor = new (mem) DrawAdaptor(functor, 0x24);
    }
    _22C = adaptor;

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

    DLchangerForAddDL* changer = new DLchangerForAddDL;
    changer->mBuffers = new DLBufferInfoForAddDL[2];
    changer->_4 = 2;
    changer->mCurrentBuffer = 0;

    for (u32 i = 0; i < changer->_4; i++) {
        changer->mBuffers[i].mDL = new (0x20) u8[0x100];
    }

    mDLchanger = static_cast< DLchanger* >(changer);
    _1A4 = 0.0f;

    swapTextureInit();
    createRainbowDL();

    {
        MR::CurrentHeapRestorer restorer(static_cast< JKRHeap* >(MR::getSceneHeapGDDR3()));
        _B7C = new JUTTexture(0x80, 0x40, GX_TF_RGBA8);

        JUTTexture** ppTexture = &_B80;
        for (u32 i = 0; i < 2; i++) {
            ppTexture[i] = new JUTTexture(8, 8, GX_TF_IA8);
            ppTexture[i]->mWrapS = 1;
            ppTexture[i]->mWrapT = 1;
            ppTexture[i]->mMinType = 0;
            ppTexture[i]->mMagType = 0;
            ppTexture[i]->init();
        }

        _B88 = 0;
        updateRandomTexture(1000.0f);
        updateRandomTexture(1000.0f);
    }

    initScreenBox();
    MR::startBtp(this, "ElementEnd");
}

void MarioActor::initBeeMario() {
    const char* modelName = gIsLuigi ? "BeeLuigi" : "BeeMario";
    const char* archiveName = gIsLuigi ? "BeeLuigi.arc" : "BeeMario.arc";
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
    _9E4->getBaseMtx();
    MR::hideModel(_9E4);
    _9EC = MR::initFurPlayer(_9E4);
    MR::hideJoint(_9E4, "Face0");

    _9E8 = new MarioParts(this, "B-WING", "BeeMarioWing", false, nullptr, nullptr);
    _9E8->initWithoutIter();

    TVec3f localTrans(0.0f, 0.0f, 0.0f);
    TVec3f localRot(0.0f, 0.0f, 0.0f);
    _9E8->initFixedPosition(_DDC.toMtxPtr(), localRot, localTrans);
    _9E8->makeActorDead();

    _9F0 = false;
    _3D2 |= 0x10;
}

void MarioActor::initTeresaMario() {
    const char* modelName = gIsLuigi ? "TeresaLuigi" : "TeresaMario";
    if (!MR::isLoadedObjectOrLayoutArchive(modelName)) {
        return;
    }

    const char* partName = "\x83\x65\x83\x8C\x83\x54\x82\xCC\x94\xE7";
    _9A4 = new MarioParts(this, modelName, partName, nullptr);
    if (gIsLuigi) {
        _9A4->mName = "TeresaMario";
    }
    _9A4->initWithoutIter();
    _9A4->makeActorDead();

    TVec3f zero(0.0f, 0.0f, 0.0f);
    _9A4->initFixedPosition(zero, zero, nullptr);

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
    MR::hideModel(_A04);
    _3D2 |= 0x100;
}

void MarioActor::initHopperMario() {
    const char* modelName = gIsLuigi ? "HopperLuigi" : "HopperMario";
    const char* archiveName = gIsLuigi ? "HopperLuigi.arc" : "HopperMario.arc";

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
    MR::hideModel(_A00);
    MR::hideJoint(_A00, "Face0");
    _3D2 |= 0x20;
}

void MarioActor::initInvincibleMario() {
    const char* modelName = gIsLuigi ? "InvincibleLuigi" : "InvincibleMario";
    const char* archiveName = gIsLuigi ? "InvincibleLuigi.arc" : "InvincibleMario.arc";

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
    MR::hideModel(_9C8);
    MR::startBrk(_9C8, "InvincibleMario");
    _3D2 |= 0x2;
}

void MarioActor::initIceMario() {
    const char* modelName = gIsLuigi ? "IceLuigi" : "IceMario";
    const char* archiveName = gIsLuigi ? "IceLuigi.arc" : "IceMario.arc";

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
    MR::hideModel(_9C0);

    _B4C = new IceStep*[20];
    for (u32 i = 0; i < 20; i++) {
        _B4C[i] = new IceStep("IceStep");
        _B4C[i]->initWithoutIter();
        _B4C[i]->makeActorDead();
    }

    _B50 = 0;
    _3D2 |= 0x8;
}

void MarioActor::swapTextureInit() {
    J3DModelDataForMarioActorDraw* actorData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(this));
    _B60 = actorData->mTexture->getNum();
    _B64 = new ResTIMG*[_B60];
    _B6A = 0;

    for (u16 i = 0; i < _B60; i++) {
        _B64[i] = MR::getResTIMG(this, i);
    }

    _B6C = new DLholder[12];

    u16 texNo = 0;
    for (u16 i = 0; i < _B60; ++i) {
        J3DModelDataForMarioActorDraw* modelData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(this));
        const char* texName = modelData->mTextureName->getName(i);
        if (strcmp(texName, "mario_eyeLid.0") == 0) {
            texNo = i;
            break;
        }
    }

    _B70 = texNo;
    createTextureDL(&_B6C[0], 0, _B70);
    createTextureDL(&_B6C[1], 0, _B70 + 1);
    createTextureDL(&_B6C[2], 0, _B70 + 2);
    createTextureDL(&_B6C[3], 0, _B70 + 3);

    if (_9E4) {
        J3DModelDataForMarioActorDraw* beeData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(_9E4));
        const u16 textureNum = beeData->mTexture->getNum();
        u16 eyeNo = 0;

        for (u16 i = 0; i < textureNum; ++i) {
            J3DModelDataForMarioActorDraw* modelData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(_9E4));
            const char* texName = modelData->mTextureName->getName(i);
            if (strcmp(texName, "mario_eyeLid.0") == 0) {
                eyeNo = i;
                break;
            }
        }

        for (u16 i = 0; i < textureNum; i++) {
            _B64[i] = MR::getResTIMG(_9E4, i);
        }

        createTextureDL(&_B6C[4], 0, eyeNo);
        createTextureDL(&_B6C[5], 0, eyeNo + 1);
        createTextureDL(&_B6C[6], 0, eyeNo + 2);
        createTextureDL(&_B6C[7], 0, eyeNo + 3);
    }

    if (_A00) {
        J3DModelDataForMarioActorDraw* hopperData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(_A00));
        const u16 textureNum = hopperData->mTexture->getNum();
        u16 eyeNo = 0;

        for (u16 i = 0; i < textureNum; ++i) {
            J3DModelDataForMarioActorDraw* modelData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(_A00));
            const char* texName = modelData->mTextureName->getName(i);
            if (strcmp(texName, "mario_eyeLid.0") == 0) {
                eyeNo = i;
                break;
            }
        }

        for (u16 i = 0; i < textureNum; i++) {
            _B64[i] = MR::getResTIMG(_A00, i);
        }

        createTextureDL(&_B6C[8], 0, eyeNo);
        createTextureDL(&_B6C[9], 0, eyeNo + 1);
        createTextureDL(&_B6C[10], 0, eyeNo + 2);
        createTextureDL(&_B6C[11], 0, eyeNo + 3);
    }

    initBlink();
}

void MarioActor::initBlur() {
    _B14 = 0;
    for (u32 i = 0; i < 6; i++) {
        if (mModels[i] != nullptr) {
            const u16 jointNum = mModels[i]->mModelData->getJointNum();
            if (jointNum > _B14) {
                _B14 = jointNum;
            }
        }
    }

    for (u32 i = 0; i < 2; i++) {
        for (u32 j = 0; j < 8; j++) {
            _A70[i * 8 + j] = new (0x20) Mtx[_B14];

            Mtx* mtxBuffer = _A70[i * 8 + j];
            for (u16 k = 0; k < _B14; k++) {
                PSMTXIdentity(mtxBuffer[k]);
            }
        }
    }

    _A6E = false;
    _B12 = 0;
    _B10 = 0;
}

void MarioActor::calcViewAndEntry() {
    decideShadowMode();

    J3DModelX* model = getJ3DModel();
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

    if (mMario->isStatusActive(0x12)) {
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

    if (_A48) {
        _A48->updateMtx(_C8C.toMtxPtr());
    }

    if (_A4C) {
        _A4C->updateMtx(_CBC.toMtxPtr());
    }

    if (_A50) {
        _A50->updateMtx(_C8C.toMtxPtr());
    }

    if (_A54) {
        _A54->updateMtx(_CBC.toMtxPtr());
    }

    if (_A5C) {
        _A5C->updateMtx(_CEC.toMtxPtr());
    }

    updateDarkMask(0x96);
    updateDarkMask(0x96);
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

    J3DModelDataForMarioActorDraw* actorData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(this));
    J3DModelDataForMarioActorDraw* faceData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(MR::getJ3DModelData(_A5C));
    faceData->mTexture = actorData->mTexture;
    faceData->mTextureName = actorData->mTextureName;

    for (long i = 0; i < _A5B; i++) {
        s32 faceJoint = MR::getJointIndex(this, "Face0");
        J3DModel* faceModel = MR::getJ3DModel(_A5C);
        copyMaterial(faceModel, static_cast< u16 >(faceJoint), i);
    }
}

void MarioActor::updateFace() {
    const s32 model = mCurrModel;
    if (model == 2) {
        MR::hideModel(_A5C);
        return;
    }

    if (model >= 1 && model < 5) {
        MR::hideModel(_A5C);
        return;
    }

    if (mMario->isStatusActive(0x12)) {
        MR::hideModel(_A5C);
        return;
    }

    if (MR::isHiddenModel(this)) {
        MR::hideModel(_A5C);
        return;
    }

    s32 hide = 0;
    if (_482 || _481) {
        hide = 1;
    }

    if (hide || mMario->mMovementStates._F || _1C3) {
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

    s32 eyeIdx = static_cast< s32 >(0.49f + partsControl.x / 0.5f);
    if (eyeIdx < 0) {
        eyeIdx = 0;
    }

    const s32 maxEyeIdx = _A5B - 1;
    if (eyeIdx > maxEyeIdx) {
        eyeIdx = maxEyeIdx;
    }

    _A60 = eyeIdx;

    for (u16 i = 0; i < _A5B; i++) {
        J3DJoint* eyeJoint = MR::getJoint(_A5C, i + 1);
        if (i == _A60) {
            MR::showJoint(eyeJoint);
        } else {
            MR::hideJoint(eyeJoint);
        }
    }
}

void MarioActor::swapTexture(const char* pMaterialName, u8 texNo) const {
    const s16 mode = static_cast< s16 >(mPlayerMode);
    if (mode == 5) {
        if (texNo > 11) {
            return;
        }
    } else if (mode >= 5) {
        if (texNo > 3) {
            return;
        }
    } else {
        if (mode >= 4) {
            if (texNo > 7) {
                return;
            }
        } else {
            if (texNo > 3) {
                return;
            }
        }
    }

    J3DModelData* modelData = MR::getJ3DModelData(this);
    u32 materialNo = MR::getMaterialNo(modelData, pMaterialName);

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

void MarioActor::createTextureDL(DLholder* pHolder, u16 texMapID, u16 texIndex) {
    MR::ProhibitSchedulerAndInterrupts prohibit(false);

    u8 tempDL[0x200];
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

void MarioActor::copyMaterial(J3DModel* pModel, u16 materialNo, long packetIndex) {
    J3DModelDataForMarioActorDraw* modelData = reinterpret_cast< J3DModelDataForMarioActorDraw* >(mModels[mCurrModel]->mModelData);
    J3DMaterialForMarioActorDraw* material = modelData->mMaterialNodePointer[materialNo];
    material = material->mpOrigMaterial;
    if (material == nullptr) {
        return;
    }

    u16 materialIndex = material->mIndex;

    u16 start = 0;
    u16 end = 7;

    if (packetIndex != -1) {
        start = static_cast< u16 >(packetIndex);
        end = static_cast< u16 >(packetIndex);
    }

    for (u16 i = start; i <= end; i++) {
        J3DMatPacket* matPacket = &pModel->mMatPacket[i];
        J3DShapePacket* shapePacket = &pModel->mShapePacket[i];

        matPacket->mpMaterial = reinterpret_cast< J3DMaterial* >(modelData->mMaterialRemapTable[materialIndex]);
        matPacket->mpShapePacket = shapePacket;
        matPacket->addShapePacket(shapePacket);
        matPacket->mpTexture = modelData->mTexture;
        matPacket->mpDisplayListObj = modelData->mMaterialRemapTable[materialIndex]->mSharedDLObj;
    }
}

void MarioActor::changeDisplayMode(u8 mode) {
    mCurrModel = mode;
    mMarioAnim->mXanimePlayer->setModel(mModels[mCurrModel]);
    mMarioAnim->mXanimePlayerUpper->setModel(mModels[mCurrModel]);

    DisplayListMakerForModelSwap* dlMaker = reinterpret_cast< DisplayListMakerForModelSwap* >(mModelManager->mDisplayListMaker);
    dlMaker->mModel = mModels[mCurrModel];

    if (_494) {
        MtxPtr handMtx = MR::getJointMtx(this, "HandR");
        _494->setBaseMtx(handMtx);
    }

    MtxPtr handMtx = MR::getJointMtx(this, "HandR");
    _4A0->setBaseMtx(handMtx);
}

void MarioActor::calcViewMainModel() {
    J3DModelX* model = mModels[mCurrModel];
    if (mFlag.mIsNoCalcView) {
        return;
    }

    model->viewCalc3(0, nullptr);

    Mtx invView;
    Mtx invBase;
    PSMTXInverse(_BF8.toMtxPtr(), invView);
    PSMTXInverse(mModels[mCurrModel]->mBaseTransformMtx, invBase);
    PSMTXConcat(invView, invBase, _BC8.toMtxPtr());
}

void MarioActor::draw() const {
    if (_B48) {
        _B48->draw();
    }

    s32 hide = 0;
    if (_482 || _481) {
        hide = 1;
    }

    if (hide || _A61 || _1C3) {
        return;
    }

    if (_A0C) {
        drawWallShade(_1F0, _1FC, _208);
    }

    drawMarioModel();
    mMario->draw();
}

void MarioActor::drawIndirect() const {
    drawModelBlur();
    drawIndirectModel();

    if (mMario->mDrawStates._3) {
        drawColdWaterDamage();
    }

    if (_1E4 != 0.0f) {
        drawRasterScroll(_1E4, static_cast<s16>(_1E8), _1EC);
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

    if (mCurrModel != 3) {
        return;
    }

    drawSpinInhibit();

    J3DModelX* model = getJ3DModel();
    if (mFlag.mIsHiddenModel) {
        return;
    }

    model->mFlags._1C = false;
    if (mMario->isStatusActive(0x12)) {
        if (_1A1) {
            return;
        }

        if (mDLchanger) {
            mDLchanger->addDL(model);
        }
    } else {
        model->setDynamicDL(mDL[mCurrDL], mDLSize);
    }

    MR::showJoint(model, "Face0");
    model->setDrawView(0);
    model->directDraw(nullptr);
    model->mFlags.clear();
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

    HitSensor* shadowSensor = nullptr;
    if (_468 != 0) {
        shadowSensor = _428[0];
    }

    if (shadowSensor) {
        if (isUseScreenBox()) {
            mDLchanger->addDL(static_cast< J3DModelX* >(MR::getJ3DModel(_9A0)));
        }

        _9A0->drawType0();
    }

    model->mFlags.clear();

    if (_A08 == 2) {
        GXSetZScaleOffset(1.0f, 0.00001f);
        model->mFlags._5 = true;
        model->setDrawView(3);
        model->directDraw(nullptr);
        model->mFlags._5 = false;
        model->mFlags._2 = true;
        mDLchanger->addDL(model);
    } else {
        if (isUseScreenBox()) {
            mDLchanger->addDL(model);
        }

        model->mFlags._B = true;
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

    const bool blurEnabled = _A6E;
    model->_1E4 = false;
    if (!blurEnabled) {
        return;
    }

    model->mFlags._C = true;

    Mtx inv;
    PSMTXInverse(const_cast< TMtx34f& >(_AB0).toMtxPtr(), inv);
    PSMTXConcat(MR::getCameraViewMtx(), inv, inv);
    for (u32 i = 1; i < 8; i++) {
        const u32 idx = static_cast< u32 >(i + _B12) & 7;
        model->setDrawViewBuffer(reinterpret_cast< MtxPtr >(_A70[idx + (static_cast< u32 >(_B10) << 3)]));

        if (!_1C1) {
            for (u16 joint = 0; joint < getModelData()->getJointNum(); joint++) {
                const u32 prevBuffer = idx + (static_cast< u32 >(1 - _B10) << 3);
                const u32 currBuffer = idx + (static_cast< u32 >(_B10) << 3);
                Mtx* src = _A70[prevBuffer];
                Mtx* dst = _A70[currBuffer];
                PSMTXConcat(inv, src[joint], dst[joint]);
            }
        }

        const u32 jointCount = getModelData()->getJointNum();
        DCStoreRange(_A70[idx + (static_cast< u32 >(_B10) << 3)], jointCount * sizeof(Mtx));

        _94[idx + ((i - 1) << 3)]->addDL(model);
        model->directDraw(nullptr);
    }

    model->mFlags._C = false;
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
        if (mMario->isStatusActive(0x12)) {
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

        if (mAlphaEnable) {
            GXSetAlphaUpdate(1);
            GXSetDstAlpha(1, 0);
        }
        model->setDrawView(0);
        model->directDraw(nullptr);
        model->mFlags.clear();
    }

    if (mMario->isStatusActive(0x12)) {
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

    if (mAlphaEnable) {
        GXSetAlphaUpdate(0);
        GXSetDstAlpha(0, 0);
    }
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

bool MarioActor::isAllHidden() const {
    bool hidden = false;
    if (_482 || _483 || _481) {
        hidden = true;
    }

    return hidden;
}

namespace MR {
template <>
FunctorV0M< const MarioActor*, void (MarioActor::*)() const >
Functor< MarioActor >(const MarioActor* pActor, void (MarioActor::*pFunc)() const) {
    return FunctorV0M< const MarioActor*, void (MarioActor::*)() const >(pActor, pFunc);
}
}

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
    DLchangerForAddDL* changer = static_cast< DLchangerForAddDL* >(this);
    DLBufferInfoForAddDL* buffer = &changer->mBuffers[changer->mCurrentBuffer];
    pModel->setDynamicDL(buffer->mDL, buffer->mSize);
}

void J3DModelX::setDynamicDL(u8* pDL, u32 dlSize) {
    if (pDL == nullptr) {
        dlSize = 0;
    }

    _1B8 = pDL;
    _1BC = dlSize;
}

namespace NrvMarioActor {
INIT_NERVE(MarioActorNrvWait);
INIT_NERVE(MarioActorNrvGameOver);
INIT_NERVE(MarioActorNrvGameOverAbyss);
INIT_NERVE(MarioActorNrvGameOverAbyss2);
INIT_NERVE(MarioActorNrvGameOverFire);
INIT_NERVE(MarioActorNrvGameOverBlackHole);
INIT_NERVE(MarioActorNrvGameOverNonStop);
INIT_NERVE(MarioActorNrvGameOverSink);
INIT_NERVE(MarioActorNrvTimeWait);
INIT_NERVE(MarioActorNrvNoRush);
}  // namespace NrvMarioActor
