#include "Game/Camera/CameraPoseParam.hpp"

CameraPoseParam::CameraPoseParam()
    : mWatchUpVec(0.0f, 1.0f, 0.0f), mWatchPos(0.0f, 0.0f, 1.0f), mUpVec(0.0f, 1.0f, 0.0f), mPos(0.0f, 0.0f, 0.0f), mFovy(40.0f),
      mGlobalOffset(0.0f, 0.0f, 0.0f), mLocalOffset(0.0f, 0.0f, 0.0f), mFrontOffset(0.0f), mUpperOffset(0.0f), mRoll(0.0f) {}
