#include "Game/Util/StarPointerUtil.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionParts.hpp"
#include "Game/MapObj/StarPieceDirector.hpp"
#include "Game/Screen/LayoutManager.hpp"
#include "Game/Screen/StarPointerController.hpp"
#include "Game/Screen/StarPointerDirector.hpp"
#include "Game/Screen/StarPointerLayout.hpp"
#include "Game/Screen/StarPointerTarget.hpp"
#include "Game/System/GameSequenceDirector.hpp"
#include "Game/System/GameSequenceProgress.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/StarPointerOnOffController.hpp"
#include "Game/System/WPadRumble.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include <JSystem/JUtility/JUTVideo.hpp>
#include <revolution/wpad.h>

namespace {

    // static const f32 hPointColRayLength =

    class StarPointerTargetInfo {
    public:
        StarPointerTargetInfo(StarPointerTarget* pTarget, HitInfo* pHitInfo, CollisionParts* pCollisionParts)
            : mTarget(pTarget), mHitInfo(pHitInfo), mCollisionParts(pCollisionParts) {
        }

        /* 0x00 */ StarPointerTarget* mTarget;
        /* 0x04 */ HitInfo* mHitInfo;
        /* 0x08 */ CollisionParts* mCollisionParts;
    };

    typedef bool (*StarPointerFunc1)(StarPointerTargetInfo*, const TVec3f&, const TVec2f&, f32, f32);
    typedef bool (*StarPointerFunc2)(s32);

    StarPointerDirector* getStarPointerDirector() {
        return StarPointerFunction::getStarPointerDirector();
    }

    inline StarPointerLayout* getStarPointerLayout(s32 channel) {
        return getStarPointerDirector()->getStarPointerLayout(channel);
    }

    StarPointerOnOffController* getStarPointerOnOffController() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mSequenceDirector->mGameSequenceProgress->mStarPointerOnOffController;
    }

    inline StarPointerController* getStarPointerController(s32 channel) {
        return getStarPointerDirector()->getStarPointerController(channel);
    }

    bool always(s32 channel) {
        return true;
    }

    void onReaction(u64 touchID, s32 channel, bool enableTouch, bool disableShoot, bool singleTouch) {
        StarPointerLayout* layout = StarPointerFunction::getStarPointerDirector()->getStarPointerLayout(channel);
        if (layout != nullptr) {
            layout->mNewTouchedID = touchID;
            layout->mStartTouch = enableTouch;
            layout->mStartDisableShoot = disableShoot;
            layout->mStartSingleTouch = singleTouch;
        }
    }

    bool checkPointingTarget(StarPointerTargetInfo* pTargetInfo, const TVec3f& rOffset, const TVec2f& rPointerPos, f32 zMargin, f32 radiusMargin) {
        return pTargetInfo->mTarget->isPointing(rPointerPos, zMargin, radiusMargin);
    }

    bool checkPointingWithoutCheckZ(StarPointerTargetInfo* pTargetInfo, const TVec3f& rOffset, const TVec2f& rPointerPos, f32 zMargin,
                                    f32 radiusMargin) {
        return checkPointingTarget(pTargetInfo, rOffset, rPointerPos, 99999.0f, radiusMargin);
    }

    bool isStarPointerPointingCore(StarPointerTargetInfo* pTargetInfo, const LiveActor* pActor, s32 channel, StarPointerFunc1 targetPointCheckFunc,
                                   StarPointerFunc2 checkUpdateChannelFunc, bool enableTouch, bool disableShoot, bool singleTouch) {
        if (!getStarPointerLayout(channel)->mIsPointerValid) {
            return false;
        }

        if (!MR::isConnectedWPad(channel)) {
            return false;
        }

        if (channel == WPAD_CHAN1 && MR::isStarPointer2PTransparencyMode()) {
            return false;
        }

        if (channel == WPAD_CHAN0 && getStarPointerOnOffController()->compareMode(StarPointerMode_1PInvalid2PValid)) {
            return false;
        }

        StarPointerController* controller = getStarPointerController(channel);
        if (!controller->isInScreen()) {
            return false;
        }

        StarPointerLayout* layout = getStarPointerLayout(channel);
        if (layout == nullptr) {
            return false;
        }

        if (!singleTouch && layout->isSingleTouch()) {
            return false;
        }

        if (singleTouch && layout->isTouch()) {
            return false;
        }

        if (!targetPointCheckFunc(pTargetInfo, controller->mWorldPos, controller->mPastInfo.mPos, controller->getViewDistZ(), layout->getRadius())) {
            return false;
        }

        onReaction(reinterpret_cast< u64 >(pActor), channel, enableTouch, disableShoot, singleTouch);

        if (!checkUpdateChannelFunc(channel)) {
            return false;
        }

        pTargetInfo->mTarget->mLastPointedChannel = channel;
        return true;
    }
};  // namespace

namespace MR {

    void initStarPointerGameScene() {
        StarPointerFunction::getStarPointerDirector()->init();
    }

    void createStarPointerLayout() {
        StarPointerFunction::getStarPointerDirector()->createLayout();
    }

    void onStarPointerSceneOut() {
        ::getStarPointerLayout(WPAD_CHAN0)->forceEndCommandStream();
        ::getStarPointerLayout(WPAD_CHAN1)->forceEndCommandStream();
        ::getStarPointerDirector()->invalidate();
        setStarPointerModeBase();
        WPadFunction::getWPadRumble(WPAD_CHAN0)->stop();
        WPadFunction::getWPadRumble(WPAD_CHAN1)->stop();
    }

    void setStarPointerModeBase() {
        ::getStarPointerOnOffController()->setStateToBase(nullptr);
    }

    void setStarPointerCameraMtxAtGameScene() {
        ::getStarPointerDirector()->setGameSceneCameraMtx();
    }

    void initStarPointerTarget(LiveActor* pActor, f32 radius, const TVec3f& rOffset) {
        pActor->initActorStarPointerTarget(radius, &pActor->mPosition, pActor->getBaseMtx(), TVec3f(rOffset));
    }

    void initStarPointerTargetAtPos(LiveActor* pActor, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
        pActor->initActorStarPointerTarget(radius, pPos, pActor->getBaseMtx(), TVec3f(rOffset));
    }

    void initStarPointerTargetAtMtx(LiveActor* pActor, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        pActor->initActorStarPointerTarget(radius, nullptr, pMtx, TVec3f(rOffset));
    }

    void initStarPointerTargetAtJoint(LiveActor* pActor, const char* pJointName, f32 radius, const TVec3f& rOffset) {
        MtxPtr mtx = MR::getJointMtx(pActor, pJointName);
        pActor->initActorStarPointerTarget(radius, nullptr, mtx, TVec3f(rOffset));
    }

    void addStarPointerTargetCircle(LayoutActor* pLayout, const char* pLayoutName, f32 radius, const TVec2f& rPosition, const char* pPaneName) {
        pLayout->mStarPointerTargetKeeper->addTargetCircle(pLayout, pLayoutName, radius, rPosition, pPaneName);
    }

    bool isStarPointerValid(s32 channel) {
        return ::getStarPointerLayout(channel)->mIsPointerValid && MR::isConnectedWPad(channel);
    }

    bool isStarPointerPointing(const LiveActor* pActor, s32 channel, bool enableTouch, const char* pStrength) {
        if (channel == WPAD_CHAN0) {
            return MR::isStarPointerPointing1P(pActor, pStrength, enableTouch, false);
        } else {
            return MR::isStarPointerPointing2P(pActor, pStrength, enableTouch, false);
        }
    }

    bool tryStarPointerCheck(const LiveActor* pActor, bool enableTouch, const char* pStrength) {
        return MR::isStarPointerPointing1Por2P(pActor, pStrength, enableTouch, false);
    }

    bool tryStarPointerCheckWithoutRumble(LiveActor* pActor, bool enableTouch) {
        return MR::isStarPointerPointing1Por2P(pActor, nullptr, enableTouch, false);
    }

    bool isStarPointerPointingPane(const LayoutActor* pLayout, const char* pLayoutName, s32 channel, bool enableTouch, const char* pStrength) {
        if (::getStarPointerLayout(channel)->mIsPointerValid) {
            return isStarPointerPointingPaneForMeterLayout(pLayout, pLayoutName, channel, enableTouch, pStrength);
        }
        return false;
    }

    bool isStarPointerPointingPaneForMeterLayout(const LayoutActor* pLayout, const char* pLayoutName, s32 channel, bool enableTouch,
                                                 const char* pStrength) {
        if (!MR::isConnectedWPad(channel)) {
            return false;
        }

        if (channel == WPAD_CHAN1 && isStarPointer2PTransparencyMode()) {
            return false;
        }

        if (channel == WPAD_CHAN0 && ::getStarPointerOnOffController()->compareMode(StarPointerMode_1PInvalid2PValid)) {
            return false;
        }

        if (!isStarPointerInScreen(channel)) {
            return false;
        }

        TVec2f pos = getStarPointerScreenPositionOrEdge(channel);
        if (pLayout->getLayoutManager()->isPointing(pLayoutName, pos)) {
            ::onReaction(reinterpret_cast< u64 >(pLayout) + reinterpret_cast< u64 >(pLayoutName), channel, enableTouch, false, false);

            if (pStrength != nullptr) {
                if (::getStarPointerLayout(channel)->isChanceToRumble()) {
                    MR::tryRumblePad(pLayout, pStrength, channel);
                }
            }
            return true;
        }

        return false;
    }

    bool isStarPointerPointingTarget(const LayoutActor* pLayout, const char* pLayoutName, s32 channel, bool enableTouch, const char* pStrength) {
        if (!MR::isConnectedWPad(channel)) {
            return false;
        }

        if (!::getStarPointerLayout(channel)->mIsPointerValid) {
            return false;
        }

        if (channel == WPAD_CHAN1 && isStarPointer2PTransparencyMode()) {
            return false;
        }

        if (channel == WPAD_CHAN0 && ::getStarPointerOnOffController()->compareMode(StarPointerMode_1PInvalid2PValid)) {
            return false;
        }

        if (!isStarPointerInScreen(channel)) {
            return false;
        }

        StarPointerLayoutTarget* target = pLayout->mStarPointerTargetKeeper->getTarget(pLayoutName);
        TVec2f pos = getStarPointerScreenPositionOrEdge(channel);
        if (target->isPointing(pos)) {
            ::onReaction(reinterpret_cast< u64 >(pLayout) + reinterpret_cast< u64 >(pLayoutName), channel, enableTouch, false, false);

            if (pStrength != nullptr) {
                if (::getStarPointerLayout(channel)->isChanceToRumble()) {
                    MR::tryRumblePad(pLayout, pStrength, channel);
                }
            }
            return true;
        }

        return false;
    }

    bool isStarPointerPointing1P(const LiveActor* pActor, const char* pStrength, bool enableTouch, bool disableShoot) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);

        if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN0, ::checkPointingTarget, ::always, enableTouch, disableShoot, false)) {
            if (pStrength != nullptr && ::getStarPointerLayout(WPAD_CHAN0)->isChanceToRumble()) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN0);
            }
            return true;
        }
        return false;
    }

    bool isStarPointerPointing1PWithoutCheckZ(const LiveActor* pActor, const char* pStrength, bool enableTouch, bool disableShoot) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);

        if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN0, ::checkPointingWithoutCheckZ, ::always, enableTouch, disableShoot, false)) {
            if (pStrength != nullptr && ::getStarPointerLayout(WPAD_CHAN0)->isChanceToRumble()) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN0);
            }
            return true;
        }
        return false;
    }

    bool isStarPointerPointing2P(const LiveActor* pActor, const char* pStrength, bool enableTouch, bool disableShoot) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);

        if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN1, ::checkPointingTarget, ::always, enableTouch, disableShoot, false)) {
            if (pStrength != nullptr && ::getStarPointerLayout(WPAD_CHAN1)->isChanceToRumble()) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN1);
            }
            return true;
        }
        return false;
    }

    bool isStarPointerPointing2POnPressButton(const LiveActor* pActor, const char* pStrength, bool enableTouch, bool disableShoot) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);

        if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN1, ::checkPointingTarget, MR::testCorePadButtonA, enableTouch, disableShoot, false)) {
            if (pStrength != nullptr && ::getStarPointerLayout(WPAD_CHAN1)->isChanceToRumble()) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN1);
            }
            return true;
        }
        return false;
    }

    bool isStarPointerPointing2POnTriggerButton(const LiveActor* pActor, const char* pStrength, bool enableTouch, bool disableShoot) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);

        if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN1, ::checkPointingTarget, MR::testCorePadTriggerA, enableTouch, disableShoot,
                                        false)) {
            if (pStrength != nullptr) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN1);
            }
            return true;
        }

        return false;
    }

    bool isStarPointerPointingFileSelect(const LiveActor* pActor) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);
        return ::isStarPointerPointingCore(&info, pActor, WPAD_CHAN0, ::checkPointingWithoutCheckZ, ::always, true, false, false);
    }

    bool isStarPointerPointing1Por2P(const LiveActor* pActor, const char* pStrength, bool enableTouch, bool disableShoot) {
        StarPointerTargetInfo info(pActor->mStarPointerTarget, nullptr, nullptr);

        if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN0, ::checkPointingTarget, ::always, enableTouch, disableShoot, false)) {
            if (pStrength != nullptr && ::getStarPointerLayout(WPAD_CHAN0)->isChanceToRumble()) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN0);
            }
            return true;

        } else if (::isStarPointerPointingCore(&info, pActor, WPAD_CHAN1, ::checkPointingTarget, ::always, enableTouch, disableShoot, false)) {
            if (pStrength != nullptr && ::getStarPointerLayout(WPAD_CHAN1)->isChanceToRumble()) {
                MR::tryRumblePad(pActor, pStrength, WPAD_CHAN1);
            }
            return true;
        }
        return false;
    }

    bool requestPointerGuidanceNoInformation() {
        return ::getStarPointerDirector()->mGuidance->request1PGuidance(nullptr, true);
    }

    bool isExistStarPointerGuidance() {
        return ::getStarPointerDirector()->mGuidance->isExistGuidanceOrFrame();
    }

    bool isExistStarPointerGuidanceFrame1P() {
        return ::getStarPointerDirector()->mGuidance->isExistFrame1P();
    }

    void activeStarPointerGuidance() {
        if (::getStarPointerDirector()->mGuidance != nullptr) {
            ::getStarPointerDirector()->mGuidance->active();
        }
    }

    void deactiveStarPointerGuidance() {
        if (::getStarPointerDirector()->mGuidance != nullptr) {
            ::getStarPointerDirector()->mGuidance->deactive();
        }
    }

    void tryShowTimeoutedStarPointerGuidance() {
        if (::getStarPointerDirector()->mGuidance != nullptr) {
            ::getStarPointerDirector()->mGuidance->tryResetTimeout();
        }
    }

    bool isExistStarPointerTarget(const LiveActor* pActor) {
        return pActor->mStarPointerTarget != nullptr;
    }

    void setStarPointerTargetRadius3d(LiveActor* pActor, f32 radius) {
        pActor->mStarPointerTarget->mRadius3d = radius;
    }

    bool isStarPointerPointing(const TVec3f& rPos, f32 radius, s32 channel, bool enableTouch) {
        f32 r = getStarPointerRadius(channel);
        StarPointerTarget target(radius, &rPos, nullptr, TVec3f(0.0f, 0.0f, 0.0f));

        StarPointerController* controller = ::getStarPointerController(channel);

        if (!::getStarPointerLayout(channel)->mIsPointerValid) {
            return false;
        }

        if (!controller->isInScreen()) {
            return false;
        }

        StarPointerLayout* layout = ::getStarPointerLayout(channel);
        if (layout == nullptr) {
            return false;
        }

        if (target.isPointing(controller->mPastInfo.mPos, controller->getViewDistZ(), layout->getRadius())) {
            ::onReaction(reinterpret_cast< u64 >(&rPos), channel, enableTouch, false, false);
            return true;
        }

        return false;
    }

    MtxPtr getStarPointerViewMtx() {
        return ::getStarPointerDirector()->mTransHolder->mViewMtx;
    }

    Mtx44Ptr getStarPointerProjMtx() {
        return ::getStarPointerDirector()->mTransHolder->mProjMtx;
    }

    f32 getStarPointerRadius(s32 channel) {
        if (::getStarPointerLayout(channel) == nullptr) {
            return 10.0f;
        }
        return ::getStarPointerLayout(channel)->mRadius;
    }

    TVec2f* getStarPointerScreenPosition(s32 channel) {
        return &::getStarPointerController(channel)->mPastInfo.mPos;
    }

    TVec2f getStarPointerScreenPositionOrEdge(s32 channel) {
        TVec2f pos(::getStarPointerController(channel)->mPastInfo.mPos);
        StarPointerFunction::forceInsideScreenEdge(&pos);
        return pos;
    }

    f32 getStarPointerScreenSpeed(s32 channel) {
        return ::getStarPointerController(channel)->mScreenSpeed;
    }

    bool isStarPointerInScreen(s32 channel) {
        return ::getStarPointerController(channel)->isInScreen();
    }

    bool isStarPointerInScreenAnyPort(s32* pInScreenChannel) {
        for (s32 channel = 0; channel < StarPointerFunction::getNumStarPointer(); channel++) {
            if (isStarPointerInScreen(channel)) {
                if (pInScreenChannel != nullptr) {
                    *pInScreenChannel = channel;
                }
                return true;
            }
        }
        if (pInScreenChannel != nullptr) {
            *pInScreenChannel = -1;
        }
        return false;
    }

    s32* getStarPointerLastPointedPort(const LiveActor* pActor) {
        return &pActor->mStarPointerTarget->mLastPointedChannel;
    }

    TVec2f* getStarPointerScreenVelocity(s32 channel) {
        return &::getStarPointerController(channel)->mScreenVel;
    }

    void getStarPointerWorldVelocityDirection(TVec3f* pVel, s32 channel) {
        pVel->set(::getStarPointerController(channel)->mWorldVel);
    }

    TVec3f* getStarPointerWorldPosUsingDepth(s32 channel) {
        return &::getStarPointerController(channel)->mWorldPos;
    }

    void calcStarPointerWorldPointingPos(TVec3f* pPos, const TVec3f& rPos, s32 channel) {
        f32 camZ = MR::calcCameraDistanceZ(rPos);
        MR::calcWorldPositionFromScreen(pPos, ::getStarPointerController(channel)->mPastInfo.mPos, camZ);
    }

    void calcStarPointerWorldPointingPosInsideEdge(TVec3f* pPos, const TVec3f& rPos, s32 channel) {
        f32 camZ = MR::calcCameraDistanceZ(rPos);
        MR::calcWorldPositionFromScreen(pPos, getStarPointerScreenPositionOrEdge(channel), camZ);
    }

    bool calcStarPointerPosOnPlane(TVec3f* pPos, const TVec3f& rPlaneBasePos, const TVec3f& rPlaneNorm, s32 channel, bool clampEdge) {
        ::getStarPointerController(channel);  // something stripped

        TVec2f screenPos;
        if (clampEdge) {
            screenPos = getStarPointerScreenPositionOrEdge(channel);
        } else {
            screenPos.set(*getStarPointerScreenPosition(channel));
        }

        TVec3f camPos = MR::getCamPos();
        TVec3f pointRay;
        MR::calcWorldPositionFromScreen(&pointRay, screenPos, -1.0f);
        pointRay.sub(camPos);
        MR::normalize(&pointRay);

        if (MR::checkHitSemilinePlane(pPos, camPos, pointRay, rPlaneBasePos, rPlaneNorm) == true) {
            return true;
        }

        TVec3f camNorm = -MR::getCamZdir();
        TVec3f forceIntersectPos;
        MR::checkHitSemilinePlane(&forceIntersectPos, camPos, pointRay, rPlaneBasePos, camNorm);

        TVec3f forceNorm;
        TVec3f checkDir = rPlaneBasePos;
        checkDir.sub(forceIntersectPos);

        if (checkDir.dot(rPlaneNorm) < 0.0f) {
            forceNorm = -rPlaneNorm;
        } else {
            forceNorm = rPlaneNorm;
        }

        MR::checkHitSemilinePlane(pPos, forceIntersectPos, forceNorm, rPlaneBasePos, rPlaneNorm);

        return true;
    }

    bool calcStarPointerWorldVelocityDirectionOnPlane(TVec3f* pDir, const TVec3f& rPlaneBasePos, const TVec3f& rPlaneNorm, s32 channel) {
        StarPointerController* controller = ::getStarPointerController(channel);
        TVec3f camPos = MR::getCamPos();
        const TVec2f& posA = controller->mPastInfo.mPos;
        const TVec2f& posB = posA.subInline(controller->mScreenVel);
        TVec3f toPointA, toPointB;
        MR::calcWorldPositionFromScreen(&toPointA, posA, -1.0f);
        MR::calcWorldPositionFromScreen(&toPointB, posB, -1.0f);
        toPointA.sub(camPos);
        toPointB.sub(camPos);
        MR::normalize(&toPointA);
        MR::normalize(&toPointB);

        f32 projA = toPointA.dot(rPlaneNorm);
        if (MR::isNearZero(projA)) {
            return false;
        }

        f32 projB = toPointB.dot(rPlaneNorm);
        if (MR::isNearZero(projB)) {
            return false;
        }

        TVec3f camDiff = rPlaneBasePos;
        camDiff.sub(camPos);
        f32 projCam = rPlaneNorm.dot(camDiff);

        f32 distA = projCam / projA;
        f32 distB = projCam / projB;

        TVec3f planePosA, planePosB;
        planePosA.set(camPos);
        planePosA.add(toPointA * distA);
        planePosB.set(camPos);
        planePosB.add(toPointB * distB);
        pDir->set(planePosA);
        pDir->sub(planePosB);
        MR::normalizeOrZero(pDir);
        return true;
    }

    f32 calcPointRadius2D(const TVec3f& rPosition, f32 radius) {
        f32 fovyRad = MR::getFovy() * PI_180;
        f32 tan = MR::tan(fovyRad * 0.5f);
        f32 focalDist = (static_cast< s32 >(JUTVideo::getManager()->getEfbHeight()) * 0.5f) / tan;

        TVec3f viewPos;
        reinterpret_cast< TPos3f* >(MR::getCameraViewMtx())->mult(rPosition, viewPos);

        return radius * focalDist / -viewPos.z;
    }

    void DUMMY() {
        TVec2f a;
        TVec2f b;
        TVec3f scale;

        a.sub(b);
        scale.scale(1.0f);
    }

    bool calcStarPointerStrokeRotateMoment(TVec3f* pMoment, const TVec3f& rPosition, f32 radius, s32 channel) {
        if (!isStarPointerInScreen(channel)) {
            pMoment->zero();
            return false;
        }

        TVec2f screenVel = ::getStarPointerController(channel)->mScreenVel;

        f32 rad2D = MR::calcPointRadius2D(rPosition, radius);
        screenVel.scale(1.0f / rad2D);

        f32 speed = screenVel.length();
        if (speed < 0.00726f) {
            pMoment->zero();
            return false;
        }
        if (speed > 0.726f) {
            screenVel.scale(0.726f / speed);
        }

        TVec2f screenPos;
        screenPos.set(::getStarPointerController(channel)->mPastInfo.mPos);

        TVec2f pos2D;
        MR::calcScreenPosition(&pos2D, rPosition);

        f32 dist;
        TVec2f dir;
        MR::separateScalarAndDirection(&dist, &dir, screenPos - pos2D);

        f32 ratio = dist / rad2D;
        if (ratio > 1.0f) {
            pMoment->zero();
            return false;
        }
        ratio = MR::clamp(ratio, 0.0f, 1.0f);

        TVec3f moment;
        moment.set< f32 >(screenVel.y, screenVel.x, 0.0f);

        TVec3f stroke;
        stroke.x = dir.x * ratio;
        stroke.y = -dir.y * ratio;
        stroke.z = MR::sqrt(1.0f - ratio * ratio);

        TQuat4f rot;
        TVec3f front;
        front.x = 0.0f;
        front.y = 0.0f;
        front.z = 1.0f;
        rot.setRotate(front, stroke);
        rot.rotate(moment);

        MR::getCameraInvViewMtx()->mult33Inline(moment, *pMoment);
        return true;
    }

    bool calcStarPointerScreenDistanceToTarget(const LiveActor* pActor, f32* pDistance, s32 channel) {
        TVec2f screenPos;
        if (!pActor->mStarPointerTarget->calcScreenPosition(&screenPos)) {
            return false;
        }

        StarPointerController* controller = ::getStarPointerController(channel);
        if (!::getStarPointerLayout(channel)->mIsPointerValid) {
            return false;
        }

        if (!controller->isInScreen()) {
            return false;
        }

        TVec2f targetPos(controller->mPastInfo.mPos);
        screenPos -= targetPos;
        *pDistance = screenPos.length();
        return true;
    }

    bool tryStartStarPointerCommandStream(const LiveActor* pActor, const TVec3f* pPos, s32 channel, bool b) {
        return ::getStarPointerLayout(channel)->startCommandStream(pActor, pPos, b);
    }

    bool tryEndStarPointerCommandStream(const LiveActor* pActor, s32 channel) {
        StarPointerLayout* layout = ::getStarPointerLayout(channel);
        if (layout->isCommandStream(pActor)) {
            layout->endCommandStream(pActor);
            return true;
        }
        return false;
    }

    bool isStarPointerCommandStream(const LiveActor* pActor, s32 channel) {
        return ::getStarPointerLayout(channel)->isCommandStream(pActor);
    }

    void startStarPointerModeTitle(void* pRequester) {
        ::getStarPointerOnOffController()->setStateToTitle(pRequester);
    }

    void startStarPointerModeFileSelect(void* pRequester) {
        ::getStarPointerOnOffController()->setStateToFileSelect(pRequester);
    }

    void startStarPointerModeGame(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_Game);
    }

    void startStarPointerModeDemo(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_Demo);
    }

    void startStarPointerModeDemoWithStarPointer(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_DemoWithStarPointer);
    }

    void startStarPointerModeDemoWithHandPointerFinger(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_DemoWithHandPointerFinger);
    }

    void startStarPointerModeDemoMarioDeath(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_DemoMarioDeath);
    }

    void startStarPointerModeMarioLauncher(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_MarioLauncher);
    }

    void startStarPointerModeHomeButton(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_HomeButton);
    }

    void startStarPointerModeChooseYesNo(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_ChooseYesNo);
    }

    void startStarPointerModePauseMenu(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_PauseMenu);
    }

    void startStarPointerModeScenarioSelectScene(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_ScenarioSelectScene);
    }

    void startStarPointerModeBlueStar(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_BlueStar);
    }

    void startStarPointerModePowerStarGetDemo(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_PowerStarGetDemo);
    }

    void startStarPointerModeStarPieceTarget(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_StarPieceTarget);
    }

    void startStarPointerModeSphereSelectorFinger(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_SphereSelectorFinger);
    }

    void startStarPointerModeSphereSelectorOnReaction(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_SphereSelectorOnReaction);
    }

    void startStarPointerModeEnding(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_Base);
    }

    void startStarPointerModeCommandStream(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_CommandStream);
    }

    void startStarPointerMode1PInvalid2PValid(void* pRequester) {
        ::getStarPointerOnOffController()->incModeCounter(pRequester, StarPointerMode_1PInvalid2PValid);
    }

    void endStarPointerMode(void* pRequester) {
        ::getStarPointerOnOffController()->popState(pRequester);
    }

    void requestStarPointerModeErrorWindow(void* pRequester) {
        ::getStarPointerOnOffController()->requestMode(pRequester, StarPointerMode_ErrorWindow);
    }

    void requestStarPointerModeSaveLoad(void* pRequester) {
        ::getStarPointerOnOffController()->requestMode(pRequester, StarPointerMode_SaveLoad);
    }

    void requestStarPointerModePictureBook(void* pRequester) {
        ::getStarPointerOnOffController()->requestMode(pRequester, StarPointerMode_PictureBook);
    }

    void requestStarPointerModePauseMenu(void* pRequester) {
        ::getStarPointerOnOffController()->requestMode(pRequester, StarPointerMode_PauseMenu);
    }

    void requestStarPointerModeBlueStarReady(void* pRequester) {
        ::getStarPointerOnOffController()->requestMode(pRequester, StarPointerMode_BlueStarReady);
    }

    void requestStarPointerModeBigBubble(void* pRequester, const TVec3f& rPosition) {
        ::getStarPointerOnOffController()->requestMode(pRequester, StarPointerMode_BigBubble);
        ::getStarPointerDirector()->mNozzleAimPos.set(rPosition);
    }

    bool isStarPointerModeBlueStarReady() {
        return ::getStarPointerOnOffController()->compareMode(StarPointerMode_BlueStarReady);
    }

    bool isStarPointerModeStarPieceTarget() {
        return ::getStarPointerOnOffController()->compareMode(StarPointerMode_StarPieceTarget);
    }

    bool isStarPointerModeMarioLauncher() {
        return ::getStarPointerOnOffController()->isMode(StarPointerMode_MarioLauncher);
    }

    bool isStarPointerModeHomeButton() {
        return ::getStarPointerOnOffController()->isMode(StarPointerMode_HomeButton);
    }

    bool isStarPointerModeErrorWindow() {
        return ::getStarPointerOnOffController()->isMode(StarPointerMode_ErrorWindow);
    }

    void enableStarPointerShootStarPiece() {
        StarPointerDirector* director = ::getStarPointerDirector();
        director->mIsAllowP1StarPieceShot = true;
        director->mIsAllowP2StarPieceShot = true;
    }

    void disableStarPointerShootStarPiece() {
        StarPointerDirector* director = ::getStarPointerDirector();
        director->mIsAllowP1StarPieceShot = false;
        director->mIsAllowP2StarPieceShot = false;
    }

    bool isEnableStarPointerShootStarPiece(s32 channel) {
        StarPointerDirector* director = ::getStarPointerDirector();
        if (channel == WPAD_CHAN0) {
            return director->mIsAllowP1StarPieceShot;
        } else {
            return director->isEnableStarPointerShootStarPiece();
        }
    }

    bool isStarPointer2PTransparencyMode() {
        return ::getStarPointerOnOffController()->compareMode(StarPointerMode_FileSelect) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_ScenarioSelectScene) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_PauseMenu) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_ChooseYesNo) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_PictureBook) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_SphereSelectorOnReaction) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_SphereSelectorFinger) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_SaveLoad) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_StarPieceTarget) ||
               ::getStarPointerOnOffController()->compareMode(StarPointerMode_MarioLauncher);
    }

    bool isStarPointer1PInvalid2PValidMode() {
        return ::getStarPointerOnOffController()->mMode == StarPointerMode_1PInvalid2PValid;
    }

    void setStarPointerDrawSyncToken() {
        StarPointerDirector* director = ::getStarPointerDirector();
        if (director->getStarPointerLayout(WPAD_CHAN0)->mIsPointerValid || director->getStarPointerLayout(WPAD_CHAN1)->mIsPointerValid) {
            ::getStarPointerDirector()->mPeekZ->setDrawSyncToken();
        }
    }

    bool requestBlueStarGuidance() {
        if (MR::isInAreaObj("BlueStarGuidanceCube", *MR::getPlayerPos()) && !MR::isSystemTalking()) {
            return ::getStarPointerDirector()->mGuidance->request1PGuidance("PointerGuidance_BlueStar", true);
        }

        return false;
    }

    bool requestTicoSeedGuidance(s32 arg) {
        if (MR::isDemoActive()) {
            return false;
        }

        AreaObj* area = MR::getAreaObj("TicoSeedGuidanceCube", *MR::getPlayerPos());
        if (area != nullptr && arg == MR::getAreaObjArg(area, 0)) {
            return ::getStarPointerDirector()->mGuidance->request1PGuidance("PointerGuidance_TicoFat", true);
        }

        return false;
    }

    bool requestTicoSeedGuidanceForce() {
        if (MR::isDemoActive()) {
            return false;
        }

        return ::getStarPointerDirector()->mGuidance->request1PGuidance("PointerGuidance_TicoFat", true);
    }

    bool requestBigBubbleGuidance() {
        return ::getStarPointerDirector()->mGuidance->request1PGuidance("PointerGuidance_BigBubble", true);
    }

    bool requestMarioLauncherGuidance() {
        return ::getStarPointerDirector()->mGuidance->request1PGuidance("PointerGuidance_MarioLauncher", true);
    }

    bool requestFileSelectGuidance() {
        return ::getStarPointerDirector()->mGuidance->request1PGuidance("System_FileSelect008", true);
    }

    bool requestFileSelectCopyGuidance() {
        return ::getStarPointerDirector()->mGuidance->request1PGuidance("System_FileSelect002", false);
    }

    bool requestStarPieceLectureGuidance() {
        return ::getStarPointerDirector()->mGuidance->request1PGuidance("PointerGuidance_StarPieceLecture", false);
    }
};  // namespace MR
