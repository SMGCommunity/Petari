#include "Game/Player/MarioMessenger.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

#define MSG_SIZE 32

MarioMessenger::MarioMessenger(HitSensor* pSender) : NameObj("マリオメッセンジャー"), mSender(pSender) {
    mReceiverArray = new HitSensor*[MSG_SIZE];
    mMsgArray = new u32[MSG_SIZE];
    mArraySize = 0;

    MR::connectToScene(this, MR::MovementType_PlayerMessenger, -1, -1, -1);
}

void MarioMessenger::movement() {
    for (int i = 0; i < mArraySize; i++) {
        MR::sendArbitraryMsg(mMsgArray[i], mReceiverArray[i], mSender);
    }

    mArraySize = 0;
}

void MarioMessenger::addRequest(HitSensor* pReceiver, u32 msg) {
    if (mArraySize == MSG_SIZE) {
        return;
    }

    mReceiverArray[mArraySize] = pReceiver;
    mMsgArray[mArraySize] = msg;
    mArraySize++;
}

MarioMessenger::~MarioMessenger() {}
