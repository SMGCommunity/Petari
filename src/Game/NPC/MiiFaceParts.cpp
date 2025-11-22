#include "Game/NPC/MiiFaceParts.hpp"
#include "Game/NPC/MiiFaceRecipe.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/FixedPosition.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <RVLFaceLib.h>

MiiFaceParts::MiiFaceParts(const char* pName, const MiiFaceRecipe& rRecipe)
    : LiveActor(pName),
      mCharModel(nullptr),
      _90(nullptr),
      mRecipe(nullptr),
      _CC(nullptr),
      _D0(false) {
    mRecipe = new MiiFaceRecipe(rRecipe);

    MR::createSceneObj(SceneObj_MiiFacePartsHolder);
}

MiiFaceParts::~MiiFaceParts() {
}

void MiiFaceParts::init(const JMapInfoIter& rIter) {
    _90 = new (32) u8[mRecipe->getModelBufferSize()];
    mCharModel = new RFLCharModel;
    _D1 = true;
}

void MiiFaceParts::calcAnim() {
    if (_CC != nullptr) {
        _CC->calc();
        mPosition.set< f32 >(_C8[0][3], _C8[1][3], _C8[2][3]);
        PSMTXCopy(_C8, _98);
    } else {
        MR::makeMtxTR(_98, mPosition, mRotation);
    }
    MR::preScaleMtx(_98, mScale);
}

void MiiFaceParts::calcViewAndEntry() {
    Mtx viewMtx;

    if (_D1) {
        return;
    }

    PSMTXConcat(j3dSys.mViewMtx, _98, viewMtx);
    RFLSetMtx(mCharModel, viewMtx);
}

void MiiFaceParts::drawOpa(const RFLDrawCoreSetting* pSetting) const {
    if (_D1) {
        return;
    }

    if (pSetting != nullptr) {
        RFLDrawOpaCore(mCharModel, pSetting);
    } else {
        RFLDrawOpa(mCharModel);
    }
}

void MiiFaceParts::drawXlu(const RFLDrawCoreSetting* pSetting) const {
    if (_D1) {
        return;
    }

    if (pSetting != nullptr) {
        RFLDrawXluCore(mCharModel, pSetting);
    } else {
        RFLDrawXlu(mCharModel);
    }
}

void MiiFaceParts::initFixedPosition(MtxPtr pParam1, const TVec3f& rParam2, const TVec3f& rParam3) {
    _CC = new FixedPosition(pParam1, rParam2, rParam3);
    _C8 = _CC->_1C;
}

void MiiFaceParts::changeExpressionNormal() {
    tryChangeExpression(RFLExp_Normal);
}

void MiiFaceParts::changeExpressionBlink() {
    tryChangeExpression(RFLExp_Blink);
}

void MiiFaceParts::changeFaceModel(const MiiFaceRecipe& rRecipe) {
    mRecipe->mIndex = rRecipe.mIndex;
    mRecipe->mDataSource = rRecipe.mDataSource;

    if (rRecipe.mDataSource == RFLDataSource_Middle) {
        mRecipe->setDatabase(rRecipe.mMiddleDB);
    }

    _D0 = true;
}

void MiiFaceParts::initFaceModel() {
    mRecipe->initFaceModel(mCharModel, _90);

    _D0 = false;
    _D1 = false;
}

bool MiiFaceParts::tryChangeExpression(u32 expression) {
    if (_D1) {
        return false;
    }

    if (!mRecipe->hasExpression(RFLExpression(expression))) {
        return false;
    }

    RFLSetExpression(mCharModel, RFLExpression(expression));

    return true;
}
