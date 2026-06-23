#include "JSystem/JAudio2/JAISoundStarter.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"

JAISoundStarter::JAISoundStarter(bool setInstance) : JASGlobalInstance< JAISoundStarter >(setInstance) {
}

JAISoundStarter::~JAISoundStarter() {
}

bool JAISoundStarter::startLevelSound(JAISoundID id, JAISoundHandle* handlePtr, const TVec3f* posPtr) {
    JAISoundHandle* handlePtr2 = handlePtr;
    if (handlePtr2->isSoundAttached()) {
        if ((*handlePtr2)->getID() == id) {
            (*handlePtr2)->updateLifeTime(1);
            if (posPtr != nullptr) {
                (*handlePtr2)->setPos(*posPtr);
            }
            return true;
        }
    }

    bool rt = startSound(id, handlePtr, posPtr);
    if (rt && handlePtr2->isSoundAttached()) {
        (*handlePtr2)->setLifeTime(1, false);
    }
    return rt;
}
