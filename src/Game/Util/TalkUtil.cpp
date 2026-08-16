#include "Game/Util/TalkUtil.hpp"
#include "Game/NPC/TalkMessageCtrl.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/NPC/TalkNodeCtrl.hpp"
#include "Game/Screen/GameSceneLayoutHolder.hpp"
#include "Game/Screen/YesNoController.hpp"
#include "Game/Screen/YesNoLayout.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"

namespace MR {
    void registerBranchFunc(TalkMessageCtrl* pCtrl, const TalkMessageFuncBase& rFunc) {
        pCtrl->registerBranchFunc(rFunc);
    }

    void registerEventFunc(TalkMessageCtrl* pCtrl, const TalkMessageFuncBase& rFunc) {
        pCtrl->registerEventFunc(rFunc);
    }

    void registerAnimeFunc(TalkMessageCtrl* pCtrl, const TalkMessageFuncBase& rFunc) {
        pCtrl->registerAnimeFunc(rFunc);
    }

    void registerKillFunc(TalkMessageCtrl* pCtrl, const TalkMessageFuncBase& rFunc) {
        pCtrl->registerKillFunc(rFunc);
    }

    void setMessageArg(TalkMessageCtrl* pCtrl, int a2) {
        CustomTagArg arg(a2, CustomTagArg::Type_Int);
        pCtrl->setMessageArg(arg);
    }

    void setMessageArg(TalkMessageCtrl* pCtrl, const wchar_t* pArg) {
        CustomTagArg arg(pArg, CustomTagArg::Type_Char);
        pCtrl->setMessageArg(arg);
    }

    // ...

    bool tryTalkNearPlayer(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        bool ret = false;

        if (pCtrl->requestTalk()) {
            if (pCtrl->startTalk()) {
                ret = true;
            }
        }

        return ret;
    }

    bool tryTalkNearPlayerAtEnd(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        if (pCtrl->endTalk()) {
            pCtrl->requestTalk();
            return true;
        } else {
            if (pCtrl->requestTalk()) {
                pCtrl->startTalk();
            }

            return false;
        }
    }

    bool tryTalkForce(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        bool ret = false;

        if (pCtrl->requestTalkForce() && pCtrl->startTalkForce()) {
            ret = true;
        }

        return ret;
    }

    bool tryTalkForceAtEnd(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        if (pCtrl->endTalk()) {
            return true;
        }

        if (pCtrl->requestTalkForce()) {
            pCtrl->startTalkForce();
        }

        return false;
    }

    bool tryTalkForceWithoutDemo(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        bool ret = false;

        if (pCtrl->requestTalkForce() && pCtrl->startTalkForceWithoutDemo()) {
            ret = true;
        }

        return ret;
    }

    bool tryTalkForceWithoutDemoMarioPuppetable(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        bool ret = false;

        if (pCtrl->requestTalkForce()) {
            if (pCtrl->startTalkForceWithoutDemoPuppetable()) {
                ret = true;
            }
        }

        return ret;
    }

    bool tryTalkForceWithoutDemoAtEnd(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        bool ret = false;

        if (pCtrl->endTalk()) {
            return true;
        }

        if (pCtrl->requestTalkForce()) {
            pCtrl->startTalkForceWithoutDemo();
        }

        return false;
    }

    bool tryTalkForceWithoutDemoMarioPuppetableAtEnd(TalkMessageCtrl* pCtrl) {
        if (MR::isTimeKeepDemoActive()) {
            return false;
        }

        if (pCtrl->endTalk()) {
            return true;
        }

        if (pCtrl->requestTalkForce()) {
            pCtrl->startTalkForceWithoutDemoPuppetable();
        }

        return false;
    }

    bool tryTalkTimeKeepDemo(TalkMessageCtrl* pCtrl) {
        bool ret = false;

        if (pCtrl->requestTalkForce()) {
            if (pCtrl->startTalkForce()) {
                ret = true;
            }
        }

        return ret;
    }

    bool tryTalkTimeKeepDemoMarioPuppetable(TalkMessageCtrl* pCtrl) {
        bool ret = false;

        if (pCtrl->requestTalkForce()) {
            if (pCtrl->startTalkForcePuppetable()) {
                ret = true;
            }
        }

        return ret;
    }

    bool tryTalkTimeKeepDemoWithoutPauseMarioPuppetable(TalkMessageCtrl* pCtrl) {
        bool ret = false;

        if (pCtrl->requestTalkForce()) {
            if (pCtrl->startTalkForceWithoutDemoPuppetable()) {
                ret = true;
            }
        }

        return ret;
    }

    bool tryTalkRequest(TalkMessageCtrl* pCtrl) {
        return pCtrl->requestTalk();
    }

    bool tryTalkSelectLeft(TalkMessageCtrl* pCtrl) {
        bool ret = false;

        if (MR::isYesNoSelected()) {
            if (MR::isYesNoSelectedYes()) {
                ret = true;
            }
        }

        return ret;
    }

    bool tryTalkSelectRight(TalkMessageCtrl* pCtrl) {
        bool ret = false;

        if (MR::isYesNoSelected()) {
            if (!MR::isYesNoSelectedYes()) {
                ret = true;
            }
        }

        return ret;
    }

    const MtxPtr getMessageBalloonFollowMatrix(const TalkMessageCtrl* pCtrl) {
        return pCtrl->mMtx;
    }

    const TVec3f& getMessageBalloonFollowOffset(const TalkMessageCtrl* pCtrl) {
        return pCtrl->mMsgBalloonFollowOffs;
    }

    void setMessageBalloonFollowOffset(TalkMessageCtrl* pCtrl, const TVec3f& rOffs) {
        pCtrl->setMessageBallonFollowOffs(rOffs);
    }

    bool isNearPlayer(const TalkMessageCtrl* pCtrl, f32 dist) {
        return pCtrl->isNearPlayer(dist);
    }

    bool inMessageArea(const TalkMessageCtrl* pCtrl) {
        return pCtrl->inMessageArea();
    }

    bool isTalkNone(const TalkMessageCtrl* pCtrl) {
        return pCtrl->_18 == 0;
    }

    bool isTalkEntry(const TalkMessageCtrl* pCtrl) {
        return pCtrl->_18 == 1;
    }

    bool isTalkTalking(const TalkMessageCtrl* pCtrl) {
        return pCtrl->_18 == 3;
    }

    bool isTalkEnableEnd(const TalkMessageCtrl* pCtrl) {
        return pCtrl->_18 == 4;
    }

    void clearTalkState(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
    }

    void resetNode(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->resetFlowNode();
    }

    void readMessage(TalkMessageCtrl* pCtrl) {
        pCtrl->readMessage();
    }

    void forwardNode(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->forwardFlowNode();
        pCtrl->mNodeCtrl->recordTempFlowNode();
    }

    void resetAndForwardNode(TalkMessageCtrl* pCtrl, s32 numNode) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->resetFlowNode();

        for (s32 i = 0; i < numNode; i++) {
            TalkFunction::onTalkStateNone(pCtrl);
            pCtrl->mNodeCtrl->forwardFlowNode();
            pCtrl->mNodeCtrl->recordTempFlowNode();
        }
    }

    void forwardNodeNextBranchLeft(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->forwardFlowNode();
        pCtrl->mNodeCtrl->forwardCurrentBranchNode(true);
        pCtrl->mNodeCtrl->recordTempFlowNode();
    }

    void forwardNodeNextBranchRight(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->forwardFlowNode();
        pCtrl->mNodeCtrl->forwardCurrentBranchNode(false);
        pCtrl->mNodeCtrl->recordTempFlowNode();
    }

    void forwardNodeCurrentBranchLeft(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->forwardCurrentBranchNode(true);
        pCtrl->mNodeCtrl->recordTempFlowNode();
    }

    void forwardNodeCurrentBranchRight(TalkMessageCtrl* pCtrl) {
        TalkFunction::onTalkStateNone(pCtrl);
        pCtrl->mNodeCtrl->forwardCurrentBranchNode(false);
        pCtrl->mNodeCtrl->recordTempFlowNode();
    }

    bool tryForwardNode(TalkMessageCtrl* pCtrl) {
        if (pCtrl->mNodeCtrl->isExistNextNode()) {
            TalkFunction::onTalkStateNone(pCtrl);
            TalkFunction::onTalkStateNone(pCtrl);
            pCtrl->mNodeCtrl->forwardFlowNode();
            pCtrl->mNodeCtrl->recordTempFlowNode();
            return true;
        }

        return false;
    }

    bool isExistNextNode(const TalkMessageCtrl* pCtrl) {
        return pCtrl->mNodeCtrl->isExistNextNode();
    }

    bool isShortTalk(const TalkMessageCtrl* pCtrl) {
        return TalkFunction::isShortTalk(pCtrl);
    }

    void setDistanceToTalk(TalkMessageCtrl* pCtrl, f32 dist) {
        pCtrl->mTalkDistance = dist;
    }

    void onRootNodeAutomatic(TalkMessageCtrl* pCtrl) {
        pCtrl->mIsOnRootNodeAuto = true;
    }

    void offRootNodeAutomatic(TalkMessageCtrl* pCtrl) {
        pCtrl->mIsOnRootNodeAuto = false;
    }

    void onReadNodeAutomatic(TalkMessageCtrl* pCtrl) {
        pCtrl->mIsOnReadNodeAuto = true;
    }

    void offReadNodeAutomatic(TalkMessageCtrl* pCtrl) {
        pCtrl->mIsOnReadNodeAuto = false;
    }

    void onStartOnlyFront(TalkMessageCtrl* pCtrl) {
        pCtrl->mIsStartOnlyFront = true;
    }

    bool isTalkStart(const TalkMessageCtrl* pCtrl) {
        return TalkFunction::isTalkSystemStart(pCtrl);
    }

    bool isTalkEnd(const TalkMessageCtrl* pCtrl) {
        return TalkFunction::isTalkSystemEnd(pCtrl);
    }
};  // namespace MR
