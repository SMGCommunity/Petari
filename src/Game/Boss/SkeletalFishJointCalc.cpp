#include "Game/Boss/SkeletalFishJointCalc.hpp"
#include "Game/Boss/SkeletalFishRailControl.hpp"
#include "Game/Util/JointController.hpp"

void SkeletalFishJointCalc::calcJoint(TPos3f* pDst, const TPos3f* pSrc, SkeletalFishRailControl* pRailCtrl,
                                      const JointControllerInfo&) {
    pDst->concat(*pSrc, *pDst);
    
    TVec3f trans;
    pDst->getTrans(trans);

    TPos3f offsetMtx;
    offsetMtx.identity33();
    offsetMtx.setTrans(0.0f, 0.0f, -trans.z);
    pDst->concat(offsetMtx, *pDst);

    TPos3f fishRailControlMtx;
    pRailCtrl->getMtx(&fishRailControlMtx, -trans.z);
    pDst->concat(fishRailControlMtx, *pDst);
}
