#include "Game/MapObj/QuestionBoxGalleryObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"
#include "Game/MapObj/StageEffectDataTable.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapPartsUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "revolution/types.h"

namespace NrvQuestionBoxGalleryObj {
    NEW_NERVE(QuestionBoxGalleryObjNrvOnStart, QuestionBoxGalleryObj, OnStart);
    NEW_NERVE(QuestionBoxGalleryObjNrvOnWait, QuestionBoxGalleryObj, OnWait);
    NEW_NERVE(QuestionBoxGalleryObjNrvOffStart, QuestionBoxGalleryObj, OffStart);
    NEW_NERVE(QuestionBoxGalleryObjNrvOffWait, QuestionBoxGalleryObj, OffWait);
};  // namespace NrvQuestionBoxGalleryObj

QuestionBoxGalleryObj::QuestionBoxGalleryObj(const char* pName) : MapObjActor(pName) {}

void QuestionBoxGalleryObj::init(const JMapInfoIter& rIter) {
    MapObjActor::init(rIter);
    MapObjActorInitInfo rInitInfo;

    if (MR::isEqualSubString(mObjectName, "Planet")) {
        rInitInfo.setupHioNode("惑星");
    } else {
        rInitInfo.setupHioNode("地形オブジェ");
    }

    rInitInfo.setupDefaultPos();
    rInitInfo.setupConnectToScene();
    rInitInfo.setupGroupClipping(8);
    rInitInfo.setupEffect(nullptr);
    rInitInfo.setupSound(4);

    if (MR::isEqualSubString(mObjectName, "PlanetA")) {
        rInitInfo.setupFarClipping(-1.0f);
    } else {
        rInitInfo.setupFarClipping(100.0f);
    }

    f32 rRotateSpeedArg = 3.4028235e38f;
    MR::getMapPartsArgRotateSpeed(&rRotateSpeedArg, rIter);

    if (MR::isConnectedWithRail(rIter)) {
        rInitInfo.setupRailMover();
    } else if (rRotateSpeedArg != 0.0f && rRotateSpeedArg != 3.4028235e38f) {
        rInitInfo.setupRotator();
        rInitInfo.setupProjmapMtx(false);
    }

    initialize(rIter, rInitInfo);
    startMapPartsFunctions();

    bool v6 = false;
    MR::getJMapInfoArg0NoInit(rIter, &v6);

    if (v6) {
        initNerve(&NrvQuestionBoxGalleryObj::QuestionBoxGalleryObjNrvOnWait::sInstance);
    } else {
        initNerve(&NrvQuestionBoxGalleryObj::QuestionBoxGalleryObjNrvOffWait::sInstance);
    }
}

void QuestionBoxGalleryObj::exeOnStart() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "On");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvQuestionBoxGalleryObj::QuestionBoxGalleryObjNrvOnWait::sInstance);
    }
}

void QuestionBoxGalleryObj::exeOnWait() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "On");
        MR::setBckFrameAndStop(this, MR::getBckFrameMax(this));
    }
}

void QuestionBoxGalleryObj::exeOffStart() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Off");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvQuestionBoxGalleryObj::QuestionBoxGalleryObjNrvOffWait::sInstance);
    }
}

void QuestionBoxGalleryObj::exeOffWait() {
    if (MR::isFirstStep(this)) {
        MR::startAllAnim(this, "Off");
        MR::setBckFrameAndStop(this, MR::getBckFrameMax(this));
    }
}

void QuestionBoxGalleryObj::initCaseUseSwitchA(const MapObjActorInitInfo& pInitInfo) {
    MR::FunctorV0M< QuestionBoxGalleryObj*, void (QuestionBoxGalleryObj::*)() > SwitchOnFunc =
        MR::Functor< QuestionBoxGalleryObj >(this, &QuestionBoxGalleryObj::startSwitchOn);
    MR::listenStageSwitchOnA(this, SwitchOnFunc);
}

void QuestionBoxGalleryObj::initCaseUseSwitchB(const MapObjActorInitInfo& pInitInfo) {
    MR::FunctorV0M< QuestionBoxGalleryObj*, void (QuestionBoxGalleryObj::*)() > SwitchOnFunc =
        MR::Functor< QuestionBoxGalleryObj >(this, &QuestionBoxGalleryObj::startSwitchOff);
    MR::listenStageSwitchOnB(this, SwitchOnFunc);
}

void QuestionBoxGalleryObj::control() {
    MapObjActor::control();

    if (mRotator && MapObjActorUtil::isRotatorMoving(this)) {
        MR::StageEffect::tryStageEffectMoving(this, mObjectName);
    }

    if (mRailMover) {
        if (MapObjActorUtil::isRailMoverWorking(this)) {
            MR::StageEffect::tryStageEffectMoving(this, mObjectName);
        }

        if (MapObjActorUtil::isRailMoverReachedEnd(this)) {
            MR::StageEffect::tryStageEffectStop(this, mObjectName);
        }
    }
}

void QuestionBoxGalleryObj::startSwitchOn() {
    MR::startSound(this, "SE_OJ_Q_BOX_LIGHT_ON", -1, -1);
    setNerve(&NrvQuestionBoxGalleryObj::QuestionBoxGalleryObjNrvOnStart::sInstance);
}

void QuestionBoxGalleryObj::startSwitchOff() {
    MR::startSound(this, "SE_OJ_Q_BOX_LIGHT_OFF", -1, -1);
    setNerve(&NrvQuestionBoxGalleryObj::QuestionBoxGalleryObjNrvOffStart::sInstance);
}
