#include "Game/NPC/TicoFat.hpp"
#include "Game/Demo/AstroDemoFunction.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/LiveActor/PartsModel.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Screen/FullnessMeter.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/TalkUtil.hpp"
#include <cstdio>

namespace NrvTicoFat {
    NEW_NERVE(TicoFatNrvReaction, TicoFat, Reaction);
    NEW_NERVE(TicoFatNrvPrep, TicoFat, Prep);
    NEW_NERVE(TicoFatNrvWait, TicoFat, Wait);
    NEW_NERVE(TicoFatNrvPoint, TicoFat, Point);
    NEW_NERVE(TicoFatNrvEat, TicoFat, Eat);
    NEW_NERVE(TicoFatNrvChem, TicoFat, Chem);
    NEW_NERVE(TicoFatNrvTest, TicoFat, Test);
    NEW_NERVE(TicoFatNrvFullness, TicoFat, Fullness);
    NEW_NERVE(TicoFatNrvMeta, TicoFat, Meta);
    NEW_NERVE(TicoFatNrvDemo, TicoFat, Demo);
    NEW_NERVE(TicoFatNrvFly, TicoFat, Fly);
    NEW_NERVE(TicoFatNrvWipeOut, TicoFat, WipeOut);
    NEW_NERVE(TicoFatNrvWipeIn, TicoFat, WipeIn);
    NEW_NERVE(TicoFatNrvInfo, TicoFat, Info);
    NEW_NERVE(TicoFatNrvAfter, TicoFat, After);
};

namespace {
    class TicoFatParam : public AnimScaleParam {
    public:
        TicoFatParam() {
            _20 = 0.12f;
            _24 = 0.12f;
            _28 = 0.89999998f;
        }
    };

    static TicoFatParam sParam;
};

const char* TicoFat::getActionName(const char *pName) {
    if (calcScale() > 1.6f) {
        if (!strcmp(pName, "Eat")) {
            return "eat2";
        }
        if (!strcmp(pName, "EatStart")) {
            return "eatstart2";
        }
        if (!strcmp(pName, "Wait")) {
            return "wait2";
        }
        if (!strcmp(pName, "Excite")) {
            return "excite2";
        }
        if (!strcmp(pName, "Joy")) {
            return "joy2";
        }
    }
    else {
        if (calcScale() > 1.3f) {
            if (!strcmp(pName, "Eat")) {
                return "eat1";
            }
            if (!strcmp(pName, "EatStart")) {
                return "eatstart1";
            }
            if (!strcmp(pName, "Wait")) {
                return "wait1";
            }
            if (!strcmp(pName, "Excite")) {
                return "excite1";
            }
            if (!strcmp(pName, "Joy")) {
                return "joy1";
            }
        }
        else {
            if (!strcmp(pName, "Eat")) {
                return "eat0";
            }
            if (!strcmp(pName, "EatStart")) {
                return "eatstart0";
            }
            if (!strcmp(pName, "Wait")) {
                return "wait0";
            }
            if (!strcmp(pName, "Excite")) {
                return "excite0";
            }
            if (!strcmp(pName, "Joy")) {
                return "joy0";
            }
        }
    }

    return pName;
}

TicoFat::TicoFat(const char *pName) : NPCActor(pName) {
    mCameraInfo = nullptr;
    mItem = nullptr;
    mShootPath = nullptr;
    _170 = 0;
    _1DC = -1;
    _1EC = -1;
    _1F0 = 5;
    _1FC = 100.0f;
    _200 = 0;
    _204 = 0;
    _17C.identity();
    _1AC.identity();
}

void TicoFat::makeArchiveList(NameObjArchiveListCollector *pCollector, const JMapInfoIter &rIter) {
    NPCActorItem item("TicoFat");
    if (MR::getNPCItemData(&item, 0)) {
        NPCActor::addArchive(pCollector, item);
    }
}

void TicoFat::init(const JMapInfoIter &rIter) {
    NPCActorCaps caps("TicoFat");
    NPCActorItem item("TicoFat");
    caps.setDefault();
    caps.mWaitNerve = &NrvTicoFat::TicoFatNrvPrep::sInstance;
    caps._E = 0;
    caps.mSensorSize = 100.0f;
    caps.mSensorOffset.y = 30.0f;
    caps._44 = "Center";
    caps.mUseShadow = true;
    caps.mUseRail = false;
    caps._6C = "Center";
    caps._80 = 110.0f;
    caps._38 = 0;
    caps._30 = 6;
    caps._58 = 2;
    _178 = 0;
    _174 = 0;
    mStartEat = false;
    _1F4 = -1;
    mScaleController = new AnimScaleController(&sParam);
    NPCActor::initialize(rIter, caps);
    s32 itemType = _1DC;
    MR::getJMapInfoArg0NoInit(rIter, &itemType);
    _1DC = itemType;

    switch (_1DC) {
        case 0:
            mItem = MR::createKinokoOneUp();
            break;
        case 1:
            mItem = MR::createKinokoSuper();
            break;
        case -1:
        case 2:
            break;
    }

    if (mItem != nullptr) {
        mItem->mPosition.set(mPosition);
        MR::initDefaultPos(mItem, rIter);
    }

    s32 numItem = 10;
    MR::getJMapInfoArg1NoInit(rIter, &numItem);
    _1E0 = _1E4 = numItem;
    _1E8 = 0;
    MR::addHitSensorAtJointEye(this, "Mouth", "Mouth", 8, 30.0f, TVec3f(0.0f, 0.0f, 0.0f));
    initStarPieceSaveData(rIter);
    MR::declareStarPieceReceiver(this, _1E0);
    mMsgCtrl = MR::createTalkCtrlDirect(this, rIter, "Common_TicoFat001", TVec3f(0.0f, 220.0f, 0.0f), nullptr);
    _16C = MR::createTalkCtrlDirect(this, rIter, "Common_TicoFat000", TVec3f(0.0f, 220.0f, 0.0f), nullptr);
    MR::registerEventFunc(mMsgCtrl, TalkMessageFunc_Inline(this, &TicoFat::eventFunc));
    MR::setMessageArg(mMsgCtrl, _1E0);
    MR::setMessageArg(_16C, _1E0);
    MR::setDistanceToTalk(mMsgCtrl, 280.0f);
    MR::setDistanceToTalk(_16C, 280.0f);
    MR::onRootNodeAutomatic(mMsgCtrl);
    MR::onStartOnlyFront(mMsgCtrl);
    MR::onStartOnlyFront(_16C);
    s32 arg2 = -1;
    MR::getJMapInfoArg2NoInit(rIter, &arg2);
    if (arg2 != -1) {
        startEat();
    }

    s32 arg3 = -1;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);
    _1F4 = arg3;
    mMeter = new FullnessMeter(this, _1E4, _1E4 - _1E0);
    mMeter->initWithoutIter();
    mMeter->setNumber(_1E4 - _1E0);
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "変身");
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "飛行");
    MR::initMultiActorCamera(this, rIter, &mCameraInfo, "喜び");
    MR::declareEventCameraProgrammable("デブチコカメラ");
    MR::useStageSwitchWriteA(this, rIter);
    MR::getNPCItemData(&item, 0);
    equipment(item, 0);
    MR::startAction(_94, "TicoFatGoodsStarPiece");
    MR::startAction(_98, "TicoFatGoodsStarPiece");
    MR::setBpkFrame(_94, 0.0f);
    MR::setBpkFrame(_98, 100.0f);
    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        if (_1DC == -1) {
            mShootPath = new SpinDriverShootPath();
            mShootPath->initUsingParabolic(rIter, mPosition);
            MR::getJMapInfoArg4NoInit(rIter, &_1FC);
        }
    }

    MR::startBva(this, "Small0");
    MR::startBrk(this, "Normal");
    MR::setBrkFrameAndStop(this, 0.0f);
    setScale(calcScale());
    MR::joinToGroupArray(this, rIter, "惑星出現", 32);
    AstroDemoFunction::tryRegisterSimpleCastIfAstroGalaxy(this);
    setDefaults();
    _12C = 1000.0f;
    const char* waitActionName = getActionName("Wait");
    _FC = waitActionName;
    _100 = waitActionName;
    const char* talkActionName = getActionName("Talk");
    _104 = talkActionName;
    _108 = talkActionName;
    
    if (!enableAppear()) {
        makeActorDead();
    }
}

void TicoFat::initAfterPlacement() {
    MR::extractMtxYDir(getBaseMtx(), &mGravity);
    mGravity.invert();
    if (!_1E0) {
        disappear(false);
        makeActorDead();
    }
}

void TicoFat::kill() {
    MR::validateClipping(this);
    mMeter->requestDisappear();
    NPCActor::kill();
}

// stack isn't quite there
void TicoFat::setCameraParam() {
    TVec3f trans, yDir, zDir, xDir, v18;
    MR::extractMtxXYZDir(getBaseMtx(), &xDir, &yDir, &zDir);
    MR::extractMtxTrans(getBaseMtx(), &trans);
    xDir.set<f32>(((1.0f - (2.0f * (_B0.y * _B0.y))) - (2.0f * (_B0.z * _B0.z))), ((2.0f * (_B0.x * _B0.y)) + (2.0f * (_B0.h * _B0.z))), ((2.0f * (_B0.x * _B0.z)) - (2.0f * (_B0.h * _B0.y))));
    yDir.set<f32>(((2.0f * (_B0.x * _B0.y)) - (2.0f * (_B0.h * _B0.z))), ((1.0f - (2.0f * (_B0.x * _B0.x))) - (2.0f * (_B0.z * _B0.z))), ((2.0f * (_B0.y * _B0.z)) + (2.0f * (_B0.h * _B0.x))));
    zDir.set<f32>(((2.0f * (_B0.x * _B0.z)) + (2.0f * (_B0.h * _B0.y))), ((2.0f * (_B0.y * _B0.z)) - (2.0f * (_B0.h * _B0.x))), ((1.0f - (2.0f * (_B0.x * _B0.x))) - (2.0f * (_B0.y * _B0.y))));
    
    v18.setPS2(_C0);
    TVec3f* ptr = &v18;
    MR::setProgrammableCameraParam("デブチコカメラ", (*ptr + (xDir * 0.0f) + (yDir * 0.0f)) + (zDir * 0.0f), (*ptr + (xDir * 0.0f)) + ((yDir * 100.0f)) + (zDir * MR::getLinerValueFromMinMax(calcScale(), 1.0f, 1.9f, 1300.0f, 1680.0f)), yDir, false);
}
