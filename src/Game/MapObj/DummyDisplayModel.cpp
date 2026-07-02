#include "Game/MapObj/DummyDisplayModel.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/MapObj/CoinRotater.hpp"
#include "Game/MapObj/PowerStar.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"
#include "Game/Util/EventUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JMath/JMath.hpp"

namespace {
    const static DummyDisplayModelInfo cDummyDisplayModelInfoTable[14] = {

    };

    s32 getItemType(const JMapInfoIter& rIter) {
        s32 type = 0;

        if (MR::isValidInfo(rIter)) {
            MR::getJMapInfoArg6NoInit(rIter, &type);
        }

        return type;
    }

    const DummyDisplayModelInfo* getInfo(int idx) {
        if (idx < 0) {
            idx = 0;
        }

        return &cDummyDisplayModelInfoTable[idx];
    }

    DummyDisplayModel* tryCreateDummyModel(LiveActor* pHost, const JMapInfoIter& rIter, s32 a3, int itemIdx) NO_INLINE {
        s32 modelId = MR::getDummyDisplayModelId(rIter, a3);

        if (modelId == -1) {
            return nullptr;
        }

        if (modelId == 0 && MR::isGalaxyDarkCometAppearInCurrentStage()) {
            return nullptr;
        }

        DummyDisplayModel* mdl = new DummyDisplayModel(pHost, getInfo(itemIdx), itemIdx, modelId, getItemType(rIter));
        mdl->initWithoutIter();
        return mdl;
    }
};  // namespace

DummyDisplayModel::DummyDisplayModel(LiveActor* pHost, const DummyDisplayModelInfo* pInfo, int a3, s32 itemType, s32 a5)
    : PartsModel(pHost, pInfo->mName, pInfo->mName, nullptr, a3, 0) {
    mModelInfo = pInfo;
    mItemType = itemType;
    _A4 = a5;
    _A8 = 0;
    _AC = a3 == 33;
    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;

    switch (itemType) {
    case ITEM_TYPE_COIN: {
        if (!_AC) {
            MR::createCoinRotater();
        }
    }
    }
}

void DummyDisplayModel::init(const JMapInfoIter& rIter) {
    PartsModel::init(rIter);

    if (mModelInfo->mAnim != nullptr) {
        MR::startBck(this, mModelInfo->mAnim, nullptr);
    }

    if (mModelInfo->mHasColorChange) {
        MR::startBrk(this, "ColorChange");
        MR::setBrkFrameAndStop(this, _A4);
    }

    if (mItemType != ITEM_TYPE_GRAND_STAR && MR::getLightNumMax(this) > 0) {
        MR::initLightCtrl(this);
    }

    switch (mItemType) {
    case ITEM_TYPE_STARPIECE: {
        if (!_AC) {
            MR::initShadowFromCSV(this, "Shadow");
        }
        break;
    }
    case ITEM_TYPE_POWER_STAR:
    case ITEM_TYPE_GRAND_STAR: {
        if (!_AC) {
            PowerStar::initShadowPowerStar(this, false);
        }
        PowerStar::setupColor(this, mHost, -1);
        MR::emitEffect(this, "Light");
        break;
    }

    case ITEM_TYPE_KINOPIO:
        _A8 = new LodCtrl(this, rIter);
        _A8->createLodModel(33, -1, -1);
        _A8->syncMaterialAnimation();
        _A8->syncJointAnimation();
        _A8->initLightCtrl();
        _A8->setDistanceToMiddleAndLow(800.0f, 1500.0f);
        break;
    }

    MR::registerDemoSimpleCastAll(this);
}

void DummyDisplayModel::makeActorAppeared() {
    PartsModel::makeActorAppeared();

    if (_A8 != nullptr) {
        _A8->appear();
    }
}

void DummyDisplayModel::makeActorDead() {
    if (_A8 != nullptr) {
        _A8->kill();
    }

    PartsModel::makeActorDead();
}

void DummyDisplayModel::control() {
    switch (mItemType) {
    case ITEM_TYPE_POWER_STAR:
    case ITEM_TYPE_GRAND_STAR:
        PowerStar::requestPointLight(this, mHost, -1);
        break;
    case ITEM_TYPE_KINOPIO:
        _A8->update();
        break;
    }
}

void DummyDisplayModel::calcAndSetBaseMtx() {
    PartsModel::calcAndSetBaseMtx();
    MtxPtr baseMtx = getBaseMtx();
    TPos3f& m = *(TPos3f*)baseMtx;

    TVec3f v19;
    JMathInlineVEC::PSVECNegate(mModelInfo->_4, &v19);
    PSMTXMultVec(baseMtx, &v19, &mPosition);
    m.mMtx[0][3] = mPosition.x;
    m.mMtx[1][3] = mPosition.y;
    m.mMtx[2][3] = mPosition.z;

    switch (mItemType) {
    case ITEM_TYPE_COIN:
        if (!_AC) {
            PSMTXConcat(m, MR::getCoinRotateYMatrix(), m);
        }

        break;
    case ITEM_TYPE_POWER_STAR:
    case ITEM_TYPE_GRAND_STAR:
        if (!_AC) {
            TPos3f rot;
            TVec3f v;
            v.x = 0.0f;
            v.y = 1.0f;
            v.z = 0.0f;
            rot.setRotateInline(v, PI_180 * mRotation.y);
            PSMTXConcat(m, rot, m);
        }

        break;
    }
}

namespace MR {
    DummyDisplayModel* createDummyDisplayModel(LiveActor* pHost, const JMapInfoIter& rIter, s32 a3, const TVec3f& a4, const TVec3f& a5) {
        DummyDisplayModel* mdl = ::tryCreateDummyModel(pHost, rIter, a3, -1);

        if (mdl == nullptr) {
            return nullptr;
        }

        mdl->initFixedPosition(a4, a5, nullptr);
        return mdl;
    }

    DummyDisplayModel* createDummyDisplayModel(LiveActor* pHost, const JMapInfoIter& rIter, const char* pName, s32 a4) {
        DummyDisplayModel* mdl = ::tryCreateDummyModel(pHost, rIter, a4, -1);

        if (mdl == nullptr) {
            return nullptr;
        }

        mdl->initFixedPosition(pName);
        return mdl;
    }

    DummyDisplayModel* createDummyDisplayModel(LiveActor* pHost, const JMapInfoIter& rIter, MtxPtr mtx, s32 a3, const TVec3f& a4, const TVec3f& a5) {
        DummyDisplayModel* mdl = ::tryCreateDummyModel(pHost, rIter, a3, -1);

        if (mdl == nullptr) {
            return nullptr;
        }

        mdl->initFixedPosition(mtx, a4, a5);
        return mdl;
    }

    DummyDisplayModel* createDummyDisplayModelCrystalItem(LiveActor* pHost, const JMapInfoIter& rIter, const TVec3f& a4, const TVec3f& a5) {
        DummyDisplayModel* mdl = ::tryCreateDummyModel(pHost, rIter, -1, 0x21);

        if (mdl == nullptr) {
            return nullptr;
        }

        mdl->initFixedPosition(a4, a5, nullptr);
        return mdl;
    }

    DummyDisplayModel* createDummyDisplayModelCrystalItem(LiveActor* pHost, s32 a2, const TVec3f& a3, const TVec3f& a4) {
        DummyDisplayModel* mdl = ::tryCreateDummyModel(pHost, JMapInfoIter(), a2, 0x21);
        mdl->initFixedPosition(a3, a4, nullptr);
        return mdl;
    }

    s32 getDummyDisplayModelId(const JMapInfoIter& rIter, s32 a2) {
        s32 arg = -1;

        if (MR::isValidInfo(rIter)) {
            MR::getJMapInfoArg7NoInit(rIter, &arg);
        }

        if (arg == -1 && a2 != -1) {
            arg = a2;
        }

        return arg;
    }

    s32 getDummyDisplayModelId(const LiveActor* pActor) {
        return static_cast< const DummyDisplayModel* >(pActor)->mItemType;
    }

    void makeArchiveListDummyDisplayModel(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
        s32 id = MR::getDummyDisplayModelId(rIter, -1);

        if (id >= 0) {
            pCollector->addArchive(cDummyDisplayModelInfoTable[id].mName);
        }
    }
};  // namespace MR
