#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/FileSelectNumber.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DAnimation.hpp>

namespace {
    NEW_NERVE(FileSelectNumberNrvAppear, FileSelectNumber, Appear);
    NEW_NERVE(FileSelectNumberNrvWait, FileSelectNumber, Wait);
    NEW_NERVE(FileSelectNumberNrvEnd, FileSelectNumber, End);
};

FileSelectNumber::FileSelectNumber(const char* pName) :
    LayoutActor(pName, true),
    mNumber(-1),
    mSelectAnimCtrl(NULL)
{
    
}

void FileSelectNumber::init(const JMapInfoIter& rIter) {
    initLayoutManager("FileNumber", 2);
    MR::connectToSceneLayout(this);
    initNerve(&FileSelectNumberNrvWait::sInstance);

    mSelectAnimCtrl = new FileSelectNumberSub::SelectAnimController(this);
}

void FileSelectNumber::appear() {
    J3DFrameCtrl* pAnimCtrl;
    f32 animFrame;

    if (MR::isDead(this)) {
        LayoutActor::appear();
        MR::startAnim(this, "Appear", 0);
        setNerve(&FileSelectNumberNrvAppear::sInstance);
        mSelectAnimCtrl->appear();
    }
    else if (isNerve(&FileSelectNumberNrvEnd::sInstance)) {
        animFrame = MR::getAnimFrame(this, 0);

        MR::startAnim(this, "Appear", 0);

        pAnimCtrl = MR::getAnimCtrl(this, 0);

        MR::setAnimFrame(this, pAnimCtrl->mEndFrame - animFrame, 0);
        setNerve(&FileSelectNumberNrvAppear::sInstance);
    }
}

void FileSelectNumber::disappear() {
    J3DFrameCtrl* pAnimCtrl;
    f32 animFrame;

    if (isNerve(&FileSelectNumberNrvAppear::sInstance)) {
        animFrame = MR::getAnimFrame(this, 0);

        MR::startAnim(this, "End", 0);

        pAnimCtrl = MR::getAnimCtrl(this, 0);

        MR::setAnimFrame(this, pAnimCtrl->mEndFrame - animFrame, 0);
        setNerve(&FileSelectNumberNrvEnd::sInstance);
    }
    else if (isNerve(&FileSelectNumberNrvWait::sInstance)) {
        MR::startAnim(this, "End", 0);
        setNerve(&FileSelectNumberNrvEnd::sInstance);
    }
}

void FileSelectNumber::setNumber(s32 number) {
    mNumber = number;

    MR::setTextBoxNumberRecursive(this, "FileNumber", number);
}

void FileSelectNumber::onSelectIn() {
    mSelectAnimCtrl->selectIn();
}

void FileSelectNumber::onSelectOut() {
    mSelectAnimCtrl->selectOut();
}

void FileSelectNumber::control() {
    mSelectAnimCtrl->updateNerve();
}

void FileSelectNumber::exeAppear() {
    if (MR::isFirstStep(this)) {
        
    }

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&FileSelectNumberNrvWait::sInstance);
    }
}

void FileSelectNumber::exeWait() {
    
}

void FileSelectNumber::exeEnd() {
    if (MR::isFirstStep(this)) {
        
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

namespace FileSelectNumberSub {
    NEW_NERVE(SelectAnimControllerNrvSelectInStart, SelectAnimController, SelectInStart);
    NEW_NERVE(SelectAnimControllerNrvSelectIn, SelectAnimController, SelectIn);
    NEW_NERVE(SelectAnimControllerNrvSelectOutStart, SelectAnimController, SelectOutStart);
    NEW_NERVE(SelectAnimControllerNrvSelectOut, SelectAnimController, SelectOut);

    SelectAnimController::SelectAnimController(LayoutActor* pHost) :
        NerveExecutor("セレクトアニメ制御"),
        mHost(pHost)
    {
        initNerve(&SelectAnimControllerNrvSelectOut::sInstance);
    }

    void SelectAnimController::appear() {
        J3DFrameCtrl* pAnimCtrl;

        setNerve(&SelectAnimControllerNrvSelectOut::sInstance);
        MR::startAnim(mHost, "SelectOut", 1);

        pAnimCtrl = MR::getAnimCtrl(mHost, 1);

        MR::setAnimFrame(mHost, pAnimCtrl->mEndFrame - 1.0f, 1);
    }

    void SelectAnimController::selectIn() {
        if (isNerve(&SelectAnimControllerNrvSelectOut::sInstance)) {
            setNerve(&SelectAnimControllerNrvSelectInStart::sInstance);

            _C = NULL;
        }
        else if (isNerve(&SelectAnimControllerNrvSelectOutStart::sInstance)) {
            _C = &SelectAnimControllerNrvSelectIn::sInstance;
        }
    }

    void SelectAnimController::selectOut() {
        if (isNerve(&SelectAnimControllerNrvSelectIn::sInstance)) {
            setNerve(&SelectAnimControllerNrvSelectOutStart::sInstance);

            _C = NULL;
        }
        else if (isNerve(&SelectAnimControllerNrvSelectInStart::sInstance)) {
            _C = &SelectAnimControllerNrvSelectOut::sInstance;
        }
    }

    void SelectAnimController::exeSelectInStart() {
        if (MR::isFirstStep(this)) {
            MR::startAnim(mHost, "SelectIn", 1);
        }

        if (MR::isAnimStopped(mHost, 0)) {
            if (_C == &SelectAnimControllerNrvSelectOut::sInstance) {
                setNerve(&SelectAnimControllerNrvSelectOutStart::sInstance);
            }
            else {
                setNerve(&SelectAnimControllerNrvSelectIn::sInstance);
            }

            _C = NULL;
        }
    }

    void SelectAnimController::exeSelectIn() {
        
    }

    void SelectAnimController::exeSelectOutStart() {
        if (MR::isFirstStep(this)) {
            MR::startAnim(mHost, "SelectOut", 1);
        }

        if (MR::isAnimStopped(mHost, 0)) {
            if (_C == &SelectAnimControllerNrvSelectIn::sInstance) {
                setNerve(&SelectAnimControllerNrvSelectInStart::sInstance);
            }
            else {
                setNerve(&SelectAnimControllerNrvSelectOut::sInstance);
            }

            _C = NULL;
        }
    }

    void SelectAnimController::exeSelectOut() {
        
    }
};
