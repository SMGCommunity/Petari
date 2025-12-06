#include "Game/NPC/SignBoard.hpp"
#include "Game/AreaObj/FollowCollisionArea.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvSignBoard {
    NEW_NERVE(SignBoardNrvWait, SignBoard, Wait);
};

SignBoard::SignBoard(const char* pName) : NPCActor(pName) {}

void SignBoard::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("Signboard");
    caps.setDefault();
    caps.mWaitNerve = &NrvSignBoard::SignBoardNrvWait::sInstance;
    caps._60 = 30.0f;
    caps.mSensorSize = 100.0f;
    caps.mSensorOffset.x = 0.0f;
    caps.mSensorOffset.y = 130.0f;
    caps.mSensorOffset.z = 0.0f;
    caps.mUseLightCtrl = false;
    caps._38 = 0;
    initialize(rIter, caps);
    getSensor("Body")->setType(127);
    FollowCollisionArea* area = new FollowCollisionArea(TVec3f(130.0f, 160.0f, 20.0f), getBaseMtx(), 80.0f);
    TVec3f v8;
    MR::getUpVecFromQuat(&v8, _A0);
    MR::setShadowProjection(this, nullptr, mPosition, v8, true);
    MR::offCalcShadow(this, nullptr);
}

void SignBoard::control() {}

void SignBoard::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

void SignBoard::exeWait() {
    MR::tryTalkNearPlayer(mMsgCtrl);
}

SignBoard::~SignBoard() {}
