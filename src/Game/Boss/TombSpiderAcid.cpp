#include "Game/Boss/TombSpiderAcid.hpp"
#include "Game/Boss/TombSpider.hpp"
#include "Game/Boss/TombSpiderFunction.hpp"
#include "Game/Boss/TombSpiderGland.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/MapUtil.hpp"

namespace {
    static const s32 sStepToStartAcidGround = 30;
    static const f32 sDistanceCheckMap = 2500.0f;
    static const s32 sStepToEndAcidGround = 30;
}  // namespace

TombSpiderAcid::TombSpiderAcid(TombSpider* pParent)
    : mParent(pParent), mIsGlandReady(false), mIsEmitOn(false), mIsActive(false), mStateChangeTimer(0) {
    mMouthEffectMtx.identity();
    mGlandFrontLEffectMtx.identity();
    mGlandFrontREffectMtx.identity();
    mGlandRearLEffectMtx.identity();
    mGlandRearREffectMtx.identity();

    MR::setEffectHostMtx(mParent, "AcidSplash", mMouthEffectMtx);
    MR::setEffectHostMtx(TombSpiderFunction::getGlandFrontL(mParent), "AcidSplash", mGlandFrontLEffectMtx);
    MR::setEffectHostMtx(TombSpiderFunction::getGlandFrontR(mParent), "AcidSplash", mGlandFrontREffectMtx);
    MR::setEffectHostMtx(TombSpiderFunction::getGlandRearL(mParent), "AcidSplash", mGlandRearLEffectMtx);
    MR::setEffectHostMtx(TombSpiderFunction::getGlandRearR(mParent), "AcidSplash", mGlandRearREffectMtx);
}

void TombSpiderAcid::start() {
    mIsGlandReady = true;
    mIsEmitOn = true;
    mStateChangeTimer = 0;
}

void TombSpiderAcid::startGland() {
    mIsEmitOn = true;
    mStateChangeTimer = 0;
}

void TombSpiderAcid::end() {
    mIsGlandReady = false;
    mIsEmitOn = false;
    mStateChangeTimer = 0;
}

void TombSpiderAcid::update() {
    if (mIsEmitOn) {
        updateEmitAcid();
    } else {
        updateDeleteAcid();
    }

    if (mIsActive) {
        updateGroundMtx(&mMouthEffectMtx, TombSpiderFunction::getMtxMouth(mParent));

        if (TombSpiderFunction::getGlandFrontL(mParent)->isActive()) {
            if (!TombSpiderFunction::getGlandFrontL(mParent)->isDamage()) {
                updateGroundMtx(&mGlandFrontLEffectMtx, TombSpiderFunction::getMtxGlandFrontL(mParent));
            }
        }

        if (TombSpiderFunction::getGlandFrontR(mParent)->isActive()) {
            if (!TombSpiderFunction::getGlandFrontR(mParent)->isDamage()) {
                updateGroundMtx(&mGlandFrontREffectMtx, TombSpiderFunction::getMtxGlandFrontR(mParent));
            }
        }

        if (TombSpiderFunction::getGlandRearL(mParent)->isActive()) {
            if (!TombSpiderFunction::getGlandRearL(mParent)->isDamage()) {
                updateGroundMtx(&mGlandRearLEffectMtx, TombSpiderFunction::getMtxGlandRearL(mParent));
            }
        }

        if (TombSpiderFunction::getGlandRearR(mParent)->isActive()) {
            if (!TombSpiderFunction::getGlandRearR(mParent)->isDamage()) {
                updateGroundMtx(&mGlandRearREffectMtx, TombSpiderFunction::getMtxGlandRearR(mParent));
            }
        }
    }
}

void TombSpiderAcid::updateEmitAcid() {
    if (!mIsActive && ++mStateChangeTimer >= sStepToStartAcidGround) {
        MR::emitEffect(mParent, "AcidSplash");

        if (TombSpiderFunction::getGlandFrontL(mParent)->isActive()) {
            MR::emitEffect(TombSpiderFunction::getGlandFrontL(mParent), "AcidSplash");
        }

        if (TombSpiderFunction::getGlandFrontR(mParent)->isActive()) {
            MR::emitEffect(TombSpiderFunction::getGlandFrontR(mParent), "AcidSplash");
        }

        if (TombSpiderFunction::getGlandRearL(mParent)->isActive()) {
            MR::emitEffect(TombSpiderFunction::getGlandRearL(mParent), "AcidSplash");
        }

        if (TombSpiderFunction::getGlandRearR(mParent)->isActive()) {
            MR::emitEffect(TombSpiderFunction::getGlandRearR(mParent), "AcidSplash");
        }

        mIsActive = true;
    }
}

void TombSpiderAcid::updateDeleteAcid() {
    if (mIsActive && ++mStateChangeTimer >= sStepToEndAcidGround) {
        MR::deleteEffect(mParent, "AcidSplash");
        MR::deleteEffect(TombSpiderFunction::getGlandFrontL(mParent), "AcidSplash");
        MR::deleteEffect(TombSpiderFunction::getGlandFrontR(mParent), "AcidSplash");
        MR::deleteEffect(TombSpiderFunction::getGlandRearL(mParent), "AcidSplash");
        MR::deleteEffect(TombSpiderFunction::getGlandRearR(mParent), "AcidSplash");

        mIsActive = false;
    }
}

void TombSpiderAcid::updateGroundMtx(TPos3f* pDstMtx, MtxPtr pSrcMtx) {
    // FIXME: regswap, and probably some inlines
    // https://decomp.me/scratch/s6x8u

    TVec3f up(-pSrcMtx[0][2], -pSrcMtx[1][2], -pSrcMtx[2][2]);
    TVec3f side(up.y, -up.x, 0.0f);
    TVec3f front(0.0f, 0.0f, 1.0f);

    TVec3f checkLine(up.multInLine(-sDistanceCheckMap));
    TVec3f mtxPos(pSrcMtx[0][3], pSrcMtx[1][3], pSrcMtx[2][3]);
    TVec3f trans(0.0f, 0.0f, 0.0f);
    MR::getFirstPolyOnLineToMap(&trans, nullptr, mtxPos, checkLine);

    pDstMtx->setVecAndTransInline(side, up, front, trans);
}
