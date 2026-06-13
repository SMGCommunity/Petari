#include "Game/Boss/KoopaPlanetShadow.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaFunction.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

KoopaPlanetShadow::~KoopaPlanetShadow() {
}

KoopaPlanetShadow::KoopaPlanetShadow(const Koopa* pKoopa) : LiveActor("惑星用の影"), mKoopa(pKoopa), mHipMtx() {
}

void KoopaPlanetShadow::init(const JMapInfoIter& rIter) {
    initModelManagerWithAnm("KoopaPlanetShadow", nullptr, false);

    MR::connectToSceneEnemyDecoration(this);

    MR::invalidateClipping(this);

    mHipMtx = MR::getJointMtx(mKoopa, "Hip1");

    makeActorDead();
}

void KoopaPlanetShadow::calcAndSetBaseMtx() {
    TVec3f vec(mHipMtx[0][3], mHipMtx[1][3], mHipMtx[2][3]);
    TVec3f direction = vec - KoopaFunction::getPlanetCenterPos(mKoopa);

    if (MR::isNearZero(direction)) {
        return;
    }

    MR::normalize(&direction);

    TPos3f transform;
    transform.identity();

    const Koopa* pKoopa = mKoopa;
    MR::makeMtxUpFrontPos(&transform, direction, pKoopa->mFront, KoopaFunction::getPlanetCenterPos(pKoopa));
    MR::setBaseTRMtx(this, transform);
}
