#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"

#include <cstddef>

KoopaPlanetShadow::KoopaPlanetShadow(const Koopa* pKoopa) : LiveActor("惑星用の影"), mParent(pKoopa), mParentJointMtx(nullptr){};

void KoopaPlanetShadow::init(const JMapInfoIter& pIter) {
    initModelManagerWithAnm("KoopaPlanetShadow", nullptr, false);
    MR::connectToSceneEnemyDecoration(this);
    MR::invalidateClipping(this);
    mParentJointMtx = MR::getJointMtx(mParent, "Hip1");
    makeActorDead();
}

void KoopaPlanetShadow::calcAndSetBaseMtx() {
    TVec3f v1(mParentJointMtx[0][3], mParentJointMtx[1][3], mParentJointMtx[2][3]);
    TVec3f* centerPos = KoopaFunction::getPlanetCenterPos(mParent);
    TVec3f v2(v1 - *centerPos);

    if (MR::isNearZero(v2, 0.001f)) {
        return;
    }

    MR::normalize(&v2);
    TPos3f v3;
    v3.identity();
    MR::makeMtxUpFrontPos(&v3, v2, mParent->_8C, *centerPos);
    MR::setBaseTRMtx(this, v3);
}
