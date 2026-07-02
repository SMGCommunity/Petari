#include "Game/MapObj/InvisiblePolygonObjGCapture.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorSensorUtil.hpp"

InvisiblePolygonObjGCapture::InvisiblePolygonObjGCapture(const char* pName) : InvisiblePolygonObj(pName) {
}

bool InvisiblePolygonObjGCapture::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return msg == ACTMES_IS_NO_BREAK_G_CAPTURE;
}