#include "Game/MapObj/InvisiblePolygonObjGCapture.hpp"

InvisiblePolygonObjGCapture::InvisiblePolygonObjGCapture(const char *pName) : InvisiblePolygonObj(pName) {

}

bool InvisiblePolygonObjGCapture::receiveOtherMsg(u32 msg, HitSensor *pSender, HitSensor *pReceiver) {
    return msg == 197;
}