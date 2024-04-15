#include "Game/Player/MarioActor.hpp"
#include "Game/Player/TornadoMario.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/DLchanger.hpp"
#include "Game/Player/ModelHolder.hpp"

void MarioActor::drawMarioModel() const {
    if(isAllHidden()) {
        return;
    }

    if(mTornadoMario) {

        if(mMario->mMovementStates._F) {
            if(mMario->_544 > 2) {
                return;
            }
        }

        if (
            !MR::isDead(mTornadoMario) &&
            (
                MR::isBckPlaying(mTornadoMario, "MarioTornadoEnd") ||
                MR::isBckPlaying(mTornadoMario, "MarioTornadoLoop")
            )
        ) {
            return;
        }
        
    }

    switch(mCurrModel) {
        case 3:
            return;
        default:
            break;
            return;
    }

    drawSpinInhibit();
    drawSphereMask();
    bool res = drawDarkMask();
    
    J3DModelX *model = mModels[mCurrModel];
    
    if(res) {
        model->mFlags._10 = true;
    }
    
    if(!mFlags.mIsHiddenModel) {
        model->mFlags._1C = false;
        if(mMario->isStatusActive(0x12)) {

            if(_1A1) {
                return;
            }

            mDLchanger->addDL(model);

            MR::showJoint(model, "HandR0");
            MR::showJoint(model, "HandL0");
            MR::showJoint(model, "Face0");
            
        }
        else {
            if(mCurrModel == 4) {
                model->setDynamicDL(nullptr, 0);
            }
            else {
                model->setDynamicDL(mDL[mCurrDL], mDLSize);
            }
        }

        if(mAlphaEnable) {
            GXSetAlphaUpdate(1);
            GXSetDstAlpha(1, 0);
        }
        model->setDrawView(0);
        model->directDraw(nullptr);
        model->mFlags.clear();
    }

    if(mMario->isStatusActive(0x12)) {
        MR::hideJoint(model, "HandR0");
        MR::hideJoint(model, "HandL0");
        MR::hideJoint(model, "Face0");
    }

    drawHand();
    
    if(!MR::isHiddenModel(_A5C)) {
        J3DModelX *cool = (J3DModelX *)MR::getJ3DModel(_A5C);
        cool->setDynamicDL(mDL[mCurrDL], mDLSize);
        cool->directDraw(nullptr);
    }
    
    if(mAlphaEnable) {
        GXSetAlphaUpdate(0);
        GXSetDstAlpha(0, 0);
    }
    
}
