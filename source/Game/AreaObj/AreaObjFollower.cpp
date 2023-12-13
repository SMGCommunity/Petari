#include "Game/AreaObj/AreaObjFollower.hpp"

AreaObjFollower::AreaObjFollower(AreaObj *pAreaObj, const JMapInfoIter &rIter) : BaseMatrixFollower(pAreaObj, rIter) {
    mObj = pAreaObj;
    mFollowMtx.identity();
    mObj->setFollowMtx(&mFollowMtx);
}

void AreaObjFollower::update() {

    if (MR::isDead((LiveActor*)getFollowTargetActor()) || !isValid()) {
        mObj->_15 = 0;
    }
    else {
        calcFollowMatrix(&mFollowMtx);
        mObj->_15 = 1;  
    }
}

namespace MR {
    bool addBaseMatrixFollowerAreaObj(AreaObj *pArea, const JMapInfoIter &rIter) {
        if (!MR::isValidFollowID(rIter)) {
            return false;
        }

        AreaObjFollower* follower = new AreaObjFollower(pArea, rIter);
        MR::addBaseMatrixFollower(follower);
        return true;
    }
};