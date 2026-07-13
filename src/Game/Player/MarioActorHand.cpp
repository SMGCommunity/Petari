#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioAnimator.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/ModelHolder.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"

void MarioActor::initHand() {
    MR::hideJoint(mModels[0], "HandR0");
    MR::hideJoint(mModels[0], "HandL0");

    if (_A04 != nullptr) {
        MR::hideJoint(mModels[1], "HandR0");
        MR::hideJoint(mModels[1], "HandL0");
    }

    if (_9E4 != nullptr) {
        MR::hideJoint(mModels[2], "HandR0");
        MR::hideJoint(mModels[2], "HandL0");
    }

    if (_9C0 != nullptr) {
        MR::hideJoint(mModels[3], "HandR0");
        MR::hideJoint(mModels[3], "HandL0");
    }

    if (_9C8 != nullptr) {
        MR::hideJoint(mModels[4], "HandR0");
        MR::hideJoint(mModels[4], "HandL0");
    }

    _A59 = 0;
    _A5A = 0;
    _A40 = nullptr;
    _A44 = nullptr;

    if (_9C0 != nullptr) {
        _A40 = new ModelHolder("IceMarioHandL", true);
        MR::initDLMakerFog(_A40, true);
        MR::newDifferedDLBuffer(_A40);

        _A44 = new ModelHolder("IceMarioHandR", true);
        MR::initDLMakerFog(_A44, true);
        MR::newDifferedDLBuffer(_A44);
    }

    _A48 = new ModelHolder("MarioHandL", true);
    MR::initDLMakerFog(_A48, true);
    MR::newDifferedDLBuffer(_A48);

    _A4C = new ModelHolder("MarioHandR", true);
    MR::initDLMakerFog(_A4C, true);
    MR::newDifferedDLBuffer(_A4C);

    _A50 = nullptr;
    _A54 = nullptr;

    if (_9C8 != nullptr) {
        _A50 = new ModelHolder("InvincibleMarioHandL", true);
        MR::initDLMakerFog(_A50, true);
        MR::newDifferedDLBuffer(_A50);

        _A54 = new ModelHolder("InvincibleMarioHandR", true);
        MR::initDLMakerFog(_A54, true);
        MR::newDifferedDLBuffer(_A54);
    }

    for (u32 idx = 0; idx < MR::getJointNum(_A48) - 1; idx++) {
        copyMaterial(MR::getJ3DModel(_A48), MR::getJointIndex(this, "HandL0"), idx);
        copyMaterial(MR::getJ3DModel(_A4C), MR::getJointIndex(this, "HandR0"), idx);
    }

    updateHandAtMorph();
}

// void MarioActor::updateHand() {}

void MarioActor::changeHandMaterial() {
}

void MarioActor::updateHandAtMorph() {
    ModelHolder* holder1 = _A48;
    ModelHolder* holder2 = _A4C;

    if (mPlayerMode == 3) {
        holder1 = _A40;
        holder2 = _A44;
    } else if (mPlayerMode == 1) {
        holder1 = _A50;
        holder2 = _A54;
    }

    for (u32 idx = 0; idx <= 7; idx++) {
        MR::hideJoint(MR::getJoint(holder1, idx + 1));
        MR::hideJoint(MR::getJoint(holder2, idx + 1));
    }

    _A59 = -1;
    _A5A = -1;
}

void MarioActor::drawHand() const {
    if (_A58) {
        return;
    }

    if (mPlayerMode == 1) {
        reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A50))->directDraw(nullptr);
        reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A54))->directDraw(nullptr);

        return;
    }

    if (MR::isHiddenModel(_A48)) {
        return;
    }

    J3DModelX* model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A48));
    model->setDynamicDL(mDL[mCurrDL], mDLSize);
    model->directDraw(nullptr);

    model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A4C));
    model->setDynamicDL(mDL[mCurrDL], mDLSize);
    model->directDraw(nullptr);
}

void MarioActor::drawIndirectHand() const {
    if (_A58) {
        return;
    }

    if (MR::isHiddenModel(_A40)) {
        return;
    }

    J3DModelX* model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A40));

    if (mPlayerMode != 3) {
        return;
    }

    model->setDynamicDL(mDL[mCurrDL], mDLSize);
    model->directDraw(nullptr);

    model = reinterpret_cast< J3DModelX* >(MR::getJ3DModel(_A44));
    model->setDynamicDL(mDL[mCurrDL], mDLSize);
    model->directDraw(nullptr);
}
