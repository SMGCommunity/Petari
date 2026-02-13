#include "Game/Boss/TombSpiderParts.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderAcid.hpp"
#include "Game/Boss/TombSpiderGland.hpp"
#include "Game/Boss/TombSpiderThreadAttacher.hpp"
#include "Game/Boss/TombSpiderVitalSpot.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace {
    static const f32 sAttachBodyBackRadius = 500.0f;
    static const f32 sAttachBodyBackOffset = 100.0f;
    static const f32 sAttachFootRadius = 300.0f;
    static const f32 sAttachFootOffset = 100.0f;
}  // namespace

TombSpiderParts::TombSpiderParts(TombSpider* pParent)
    : mParent(pParent), mGlandFrontL(nullptr), mGlandFrontR(nullptr), mGlandRearL(nullptr), mGlandRearR(nullptr), mVitalSpotC(nullptr),
      mVitalSpotL(nullptr), mVitalSpotR(nullptr), mAcid(nullptr), mThreadAttacherWing(nullptr), mThreadAttacherFrontL(nullptr),
      mThreadAttacherCenterL(nullptr), mThreadAttacherRearL(nullptr), mThreadAttacherFrontR(nullptr), mThreadAttacherCenterR(nullptr),
      mThreadAttacherRearR(nullptr), mMtxEye(nullptr), mMtxMouth(nullptr), mMtxHip(nullptr), mMtxFrontL(nullptr), mMtxFrontR(nullptr),
      mMtxRearL(nullptr), mMtxRearR(nullptr) {
}

void TombSpiderParts::appearThreadAttacherAll() {
    mThreadAttacherWing->appear();
    mThreadAttacherFrontL->appear();
    mThreadAttacherCenterL->appear();
    mThreadAttacherRearL->appear();
    mThreadAttacherFrontR->appear();
    mThreadAttacherCenterR->appear();
    mThreadAttacherRearR->appear();
}

void TombSpiderParts::killThreadAttacherAll() {
    mThreadAttacherWing->kill();
    mThreadAttacherFrontL->kill();
    mThreadAttacherCenterL->kill();
    mThreadAttacherRearL->kill();
    mThreadAttacherFrontR->kill();
    mThreadAttacherCenterR->kill();
    mThreadAttacherRearR->kill();
}

void TombSpiderParts::pauseOffThreadAttacherAll() {
    MR::requestMovementOn(mThreadAttacherWing);
    MR::requestMovementOn(mThreadAttacherFrontL);
    MR::requestMovementOn(mThreadAttacherCenterL);
    MR::requestMovementOn(mThreadAttacherRearL);
    MR::requestMovementOn(mThreadAttacherFrontR);
    MR::requestMovementOn(mThreadAttacherCenterR);
    MR::requestMovementOn(mThreadAttacherRearR);
}

void TombSpiderParts::init() {
    MR::startBrk(mParent, "Battle1st");
    MR::addEffectHitNormal(mParent, nullptr);
    initParts();
}

namespace {
    TombSpiderGland* createGland(TombSpider* pParent, const char* pName, const TVec3f& rPos, const TVec3f& a1, const char* pJointName) {
        TombSpiderGland* gland = new TombSpiderGland(pParent, pName, rPos, a1, pJointName);
        gland->initWithoutIter();
        MR::registerDemoSimpleCastAll(gland);
        return gland;
    }

    TombSpiderVitalSpot* createVitalSpot(TombSpider* pParent, const char* pName, const TVec3f& rPos, const TVec3f& a1, const char* pJointName) {
        TombSpiderVitalSpot* vitalSpot = new TombSpiderVitalSpot(pParent, pName, rPos, a1, pJointName);
        vitalSpot->initWithoutIter();
        MR::registerDemoSimpleCastAll(vitalSpot);
        return vitalSpot;
    }

    TombSpiderThreadAttacher* createThreadAttacherFoot(TombSpider* pParent, const char* pName) {
        TombSpiderThreadAttacher* threadAttacherFoot = new TombSpiderThreadAttacher(pParent, pName, sAttachFootRadius, sAttachFootOffset);
        threadAttacherFoot->initWithoutIter();
        return threadAttacherFoot;
    }
}  // namespace

void TombSpiderParts::initParts() {
    mGlandFrontL = createGland(mParent, "毒腺[左前]", TVec3f(30.0f, 0.0f, -10.0f), TVec3f(-35.0f, -5.0f, 0.0f), "FrontL");
    mGlandFrontR = createGland(mParent, "毒腺[右前]", TVec3f(-30.0f, 0.0f, -10.0f), TVec3f(-35.0f, 5.0f, 0.0f), "FrontR");
    mGlandRearL = createGland(mParent, "毒腺[左後]", TVec3f(30.0f, 0.0f, -10.0f), TVec3f(-33.0f, -5.0f, 0.0f), "RearL");
    mGlandRearR = createGland(mParent, "毒腺[右後]", TVec3f(-30.0f, 0.0f, -10.0f), TVec3f(-33.0f, 5.0f, 0.0f), "RearR");

    mVitalSpotC = createVitalSpot(mParent, "急所[中]", TVec3f(0.0f, 0.0f, -320.0f), TVec3f(0.0f, 180.0f, 0.0f), "Body5");
    mVitalSpotL = createVitalSpot(mParent, "急所[左]", TVec3f(130.0f, -280.0f, -200.0f), TVec3f(10.0f, 167.0f, 0.0f), "Body1");
    mVitalSpotR = createVitalSpot(mParent, "急所[右]", TVec3f(130.0f, 280.0f, -200.0f), TVec3f(-10.0f, 167.0f, 0.0f), "Body1");

    mThreadAttacherWing = new TombSpiderThreadAttacher(mParent, "WingL", sAttachBodyBackRadius, sAttachBodyBackOffset);
    mThreadAttacherWing->initWithoutIter();

    mThreadAttacherFrontL = createThreadAttacherFoot(mParent, "FrontHandL3");
    mThreadAttacherCenterL = createThreadAttacherFoot(mParent, "CenterHandL3");
    mThreadAttacherRearL = createThreadAttacherFoot(mParent, "RearHandL3");
    mThreadAttacherFrontR = createThreadAttacherFoot(mParent, "FrontHandR3");
    mThreadAttacherCenterR = createThreadAttacherFoot(mParent, "CenterHandR3");
    mThreadAttacherRearR = createThreadAttacherFoot(mParent, "RearHandR3");

    mMtxEye = MR::getJointMtx(mParent, "Eye");
    mMtxMouth = MR::getJointMtx(mParent, "Mouth");
    mMtxHip = MR::getJointMtx(mParent, "Hip");
    mMtxFrontL = MR::getJointMtx(mParent, "FrontL");
    mMtxFrontR = MR::getJointMtx(mParent, "FrontR");
    mMtxRearL = MR::getJointMtx(mParent, "RearL");
    mMtxRearR = MR::getJointMtx(mParent, "RearR");

    mAcid = new TombSpiderAcid(mParent);
}
