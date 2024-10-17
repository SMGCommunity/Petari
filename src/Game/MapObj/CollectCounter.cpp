#include "Game/MapObj/CollectCounter.hpp"
#include "Game/Util.hpp"

CollectCounter::CollectCounter(const char *pName) : LayoutActor(pName, true) {
    mFollowPosition.x = 0.0f;
    mFollowPosition.y = 0.0f;
}

void CollectCounter::init(const JMapInfoIter &rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("CollectCounter", 1);
    initNerve(&NrvCollectCounter::CollectCounterNrvHide::sInstance);
    MR::setFollowPos(&mFollowPosition, this, 0);
    kill();
}

void CollectCounter::control() {
    setPosition();
}

void CollectCounter::setCount(s32 num) {
    appear();
    setNerve(&NrvCollectCounter::CollectCounterNrvShow::sInstance);
    MR::setTextBoxNumberRecursive(this, "CollectCounter", num);
}

bool CollectCounter::tryEndShow() {
    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvCollectCounter::CollectCounterNrvHide::sInstance);
        kill();
        return true;
    }

    return false;
}

void CollectCounter::setPosition() {
    f32 point_rad = MR::calcPointRadius2D(*MR::getPlayerCenterPos(), 75.0f);
    TVec2f screen_pos;
    MR::calcScreenPosition(&screen_pos, *MR::getPlayerCenterPos());
    screen_pos.y -= point_rad;
    mFollowPosition.x = screen_pos.x;
    mFollowPosition.y = screen_pos.y;
}

CollectCounter::~CollectCounter() {

}

namespace NrvCollectCounter {
    CollectCounterNrvHide CollectCounterNrvHide::sInstance;
    CollectCounterNrvShow CollectCounterNrvShow::sInstance;

    void CollectCounterNrvShow::execute(Spine *pSpine) const {
        CollectCounter* counter = reinterpret_cast<CollectCounter*>(pSpine->mExecutor);
        
        if (MR::isFirstStep(counter)) {
            MR::startAnim(counter, "Appear", 0);
        }
 
        counter->tryEndShow();
    }

    void CollectCounterNrvHide::execute(Spine *pSpine) const {
        
    }
};
