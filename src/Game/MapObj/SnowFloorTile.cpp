#include "Game/MapObj/SnowFloorTile.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace {
    u16 sTextureSize = 32;
    u16 sStripCount = 16;
    s16 sLayerCount = 12;
    s16 sColorR = 220;
    s16 sColorStepR = -3;
    s16 sColorG = 220;
    s16 sColorStepG = -3;
    s16 sColorB = 250;
    s16 sColorStepB = -3;
    s16 sColorA = 64;
    s16 sColorStepA = 8;
    s16 sLayerHeight = 6;
    f32 cDpdDigAmount = 30.0f;
    f32 cPlayerDigAmount = 600.0f;
    f32 cFireballDigAmount = 200.0f;
    f32 cSwingDigAmount = 20.0f;
    f32 cSwingDigRadius = 100.0f;
    f32 cHipdropDigAmount = 1000.0f;
    f32 cHipdropDigRadius = 50.0f;
    u16 sCellSize = 128 / sTextureSize;
}  // namespace

SnowFloorTile::~SnowFloorTile() {
}

SnowFloorTile::SnowFloorTile(const char* pName) : LiveActor(pName), _F8(), _FC() {
}

void SnowFloorTile::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    mTextureSize = sTextureSize * mScale.x;
    mTextureShift = 0;
    u32 size = mTextureSize;

    for (u32 i = 0; i < mTextureSize; i++) {
        if ((size >>= 1) == 0) {
            break;
        }

        mTextureShift++;
    }

    mCellSize = 128.0f * mScale.x / sTextureSize;
    initHitSensor(1);
    MR::addHitSensorEye(this, "eye", 16, 1.4142f * (mTextureSize * mCellSize), TVec3f(0.0f, 0.0f, 0.0f));
    mSnowDepth = new f32[mTextureSize * mTextureSize];

    for (u32 y = 0; y < mTextureSize; y++) {
        for (u32 x = 0; x < mTextureSize; x++) {
            mSnowDepth[(y << mTextureShift) + x] = 32.0f + 120.0f * MR::getRandom();
        }
    }

    initLocalPoint(rIter);
    mTextures[0] = new JUTTexture(mTextureSize, mTextureSize, GX_TF_I8);
    mTextures[1] = new JUTTexture(mTextureSize, mTextureSize, GX_TF_I8);
    mTexture = mTextures[0];
    mTextureIndex = 0;
    mImage = mTexture->mImage;
    mLastDpdPoint.zero();
    mLastPlayerPoint.zero();

    MR::connectToSceneNormalMapObj(this);
    MR::validateClipping(this);
    MR::setClippingFar100m(this);
    MR::useStageSwitchWriteDead(this, rIter);
    initSound(4, false);
    makeActorAppeared();
    createReduceMap(800.0f / mCellSize);
    mDirtyFrames = 2;
}

void SnowFloorTile::initLocalPoint(const JMapInfoIter& rIter) {
    mRight.set(1.0f, 0.0f, 0.0f);
    mUp.set(0.0f, 1.0f, 0.0f);
    mForward.set(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, mRotation.x, mRotation.y, mRotation.z);
    PSMTXMultVecSR(mtx, &mRight, &mRight);
    PSMTXMultVecSR(mtx, &mUp, &mUp);
    PSMTXMultVecSR(mtx, &mForward, &mForward);
}

bool SnowFloorTile::calcLocalPoint(const TVec3f& rPosition, TVec3f* pPoint, f32 heightLimit) {
    TVec3f offset(rPosition - mPosition);
    f32 right = offset.dot(mRight);
    f32 up = offset.dot(mUp);
    f32 forward = offset.dot(mForward);
    f32 height;
    f32 x = -right / mCellSize;
    f32 y = forward / mCellSize;
    x += mTextureSize;
    y += mTextureSize;
    x *= 0.5f;
    y *= 0.5f;
    x /= mTextureSize;
    y /= mTextureSize;
    x = 1.0f - x;
    pPoint->set(x, y, up);

    height = __fabsf(up);

    if (height >= heightLimit) {
        return false;
    }

    if (MR::isInRange(right, -mTextureSize * mCellSize, mTextureSize * mCellSize) &&
        MR::isInRange(forward, -mTextureSize * mCellSize, mTextureSize * mCellSize)) {
        return true;
    }

    return false;
}

void SnowFloorTile::createReduceMap(u16 size) {
    u32 count = size * size;
    mReduceMapSize = size;
    mReduceMap = new f32[count];

    for (u32 i = 0; i < count; i++) {
        mReduceMap[i] = -1.0f;
    }

    u32 samples = 0;
    f32 total = 0.0f;
    f32 center = size / 2;

    for (s32 y = 0; y < size; y++) {
        for (s32 x = 0; x < size; x++) {
            TVec2f offset(x - center, y - center);
            f32 distance = offset.length();

            if (distance >= size / 2) {
                continue;
            }

            f32 value = distance + 0.5f * (distance * MR::getRandom());
            samples++;
            total += value;
            mReduceMap[y * size + x] = value;
        }
    }

    f32 average = total / samples;

    for (s32 y = 0; y < size; y++) {
        for (s32 x = 0; x < size; x++) {
            if (mReduceMap[y * size + x] < 0.0f) {
                mReduceMap[y * size + x] = 0.0f;
            } else {
                mReduceMap[y * size + x] -= average;
                mReduceMap[y * size + x] /= 0.2f * average;
            }
        }
    }
}

void SnowFloorTile::digPlayerWalk(const TVec3f& rPoint) {
    TVec2f point(rPoint.x, rPoint.y);
    MR::setPlayerWalkingResist(0.1f);
    doMove(mLastPlayerPoint, point, 20.0f / mCellSize, cPlayerDigAmount);
    mLastPlayerPoint = point;
}

void SnowFloorTile::digDpd(const TVec3f& rPoint) {
    TVec2f point(rPoint.x, rPoint.y);
    s32 volume = doMove(mLastDpdPoint, point, 50.0f / mCellSize, cDpdDigAmount) / 200.0f;

    if (volume > 100) {
        volume = 100;
    }

    MR::startLevelSound(this, "SE_OJ_LV_SNOW_DIG", volume);
    mLastDpdPoint = point;
}

void SnowFloorTile::control() {
    TVec3f position(*MR::getPlayerPos());
    TVec3f localPoint;
    bool changed = false;

    if (calcLocalPoint(position, &localPoint, 20.0f)) {
        digPlayerWalk(localPoint);
        changed = true;
    }

    if (MR::isPlayerSwingAction()) {
        if (doErase(*MR::getPlayerPos(), cSwingDigRadius, cSwingDigAmount)) {
            MR::startLevelSound(this, "SE_OJ_LV_SNOW_ERASE");
        }

        changed = true;
    } else if (MR::isPlayerHipDropLand()) {
        if (doErase(*MR::getPlayerPos(), cHipdropDigRadius, cHipdropDigAmount)) {
            MR::startLevelSound(this, "SE_OJ_LV_SNOW_ERASE");
        }

        changed = true;
    }

    if (MR::testCorePadButtonB(0)) {
        position = *MR::getStarPointerWorldPosUsingDepth(0);

        if (calcLocalPoint(position, &localPoint, 20.0f) || MR::isInRange(localPoint.z, -100.0f, 100.0f)) {
            digDpd(localPoint);
            changed = true;
        }
    }

    if (changed) {
        mDirtyFrames = 2;
    }

    createTexture();
}

bool SnowFloorTile::doErase(const TVec3f& rPosition, f32 radius, f32 amount) {
    TVec3f localPoint;

    if (calcLocalPoint(rPosition, &localPoint, 20.0f)) {
        f32 radiusSq = radius * radius;
        f32 height = localPoint.z;

        if (radiusSq < height * height) {
            return false;
        }

        f32 localRadius = MR::sqrt(radiusSq - height * height);
        TVec2f point(localPoint.x, localPoint.y);
        doErase(point, localRadius / mCellSize, amount);
        return true;
    }

    return false;
}

f32 SnowFloorTile::doMove(TVec2f from, TVec2f to, f32 radius, f32 amount) {
    f32 outerRadius;
    f32 removed = 0.0f;
    f32 weightTotal = 0.0f;
    f32 depthTotal = 0.0f;
    u32 minX;
    u32 count = 0;
    TVec2f unitDirection(to - from);
    unitDirection.normalize();
    u16 xs[4096];
    u16 ys[4096];
    f32 weights[4096];
    outerRadius = 1.732051f * radius;
    to.x *= mTextureSize;
    to.y *= mTextureSize;
    s32 left = static_cast< s16 >(to.x - outerRadius) - 1;
    s32 top = static_cast< s16 >(to.y - outerRadius) - 1;
    s32 right = static_cast< s16 >(to.x + outerRadius) + 1;
    s32 bottom = static_cast< s16 >(to.y + outerRadius) + 1;
    minX = MR::clamp(left, 0L, static_cast< s32 >(mTextureSize));
    u32 minY = MR::clamp(top, 0L, static_cast< s32 >(mTextureSize));
    u32 maxX = MR::clamp(right, 0L, static_cast< s32 >(mTextureSize));
    u32 maxY = MR::clamp(bottom, 0L, static_cast< s32 >(mTextureSize));
    f32 radiusSq;
    f32 totalSnow = 0.0f;
    outerRadius *= outerRadius;
    radiusSq = radius * radius;

    for (u32 y = minY; y < maxY; y++) {
        u32 row = y * mTextureSize;

        for (u32 x = minX; x < maxX; x++) {
            TVec2f offset(x - to.x, y - to.y);
            f32 distanceSq = offset.squared();

            totalSnow += mSnowDepth[row + x];

            if (distanceSq <= radiusSq) {
                u32 index = row + x;

                if (mSnowDepth[index] > amount) {
                    removed += amount;
                    mSnowDepth[index] -= amount;
                } else {
                    removed += mSnowDepth[index];
                    mSnowDepth[index] = 0.0f;
                }

                depthTotal += mSnowDepth[index];
            } else if (distanceSq <= outerRadius) {
                f32 weight = offset.dot(unitDirection);

                if (weight > 0.0f) {
                    weightTotal += weight;
                    xs[count] = x;
                    ys[count] = y;
                    weights[count] = weight;
                    count++;
                }
            }
        }
    }

    if (MR::isNearZero(totalSnow)) {
        if (MR::isValidSwitchDead(this)) {
            MR::onSwitchDead(this);
        }

        kill();
        return 0.0f;
    }

    if (count == 0) {
        return 0.0f;
    }

    removed /= weightTotal;

    for (u32 i = 0; i < count; i++) {
        mSnowDepth[(ys[i] << mTextureShift) + xs[i]] += removed * weights[i];
    }

    return depthTotal;
}

void SnowFloorTile::doErase(TVec2f point, f32 radius, f32 amount) {
    point.x *= mTextureSize;
    point.y *= mTextureSize;
    u32 minX;
    s32 left = static_cast< s16 >(point.x - radius) - 1;
    s32 top = static_cast< s16 >(point.y - radius) - 1;
    s32 right = static_cast< s16 >(point.x + radius) + 1;
    s32 bottom = static_cast< s16 >(point.y + radius) + 1;
    minX = MR::clamp(left, 0L, static_cast< s32 >(mTextureSize));
    u32 minY = MR::clamp(top, 0L, static_cast< s32 >(mTextureSize));
    u32 maxX = MR::clamp(right, 0L, static_cast< s32 >(mTextureSize));
    u32 maxY = MR::clamp(bottom, 0L, static_cast< s32 >(mTextureSize));
    f32 radiusSq = radius * radius;

    for (u32 y = minY; y < maxY; y++) {
        u32 row = y * mTextureSize;

        for (u32 x = minX; x < maxX; x++) {
            TVec2f offset(x - point.x, y - point.y);
            f32 distanceSq = offset.squared();

            if (distanceSq <= radiusSq) {
                f32 falloff = 1.0f - distanceSq / radiusSq;
                f32 dig = falloff * falloff;
                dig *= amount;
                u32 index = row + x;

                if (mSnowDepth[index] > dig) {
                    mSnowDepth[index] -= dig;
                } else {
                    mSnowDepth[index] = 0.0f;
                }
            }
        }
    }
}

void SnowFloorTile::createTexture() {
    mTextureIndex = 1 - mTextureIndex;
    mTexture = mTextures[mTextureIndex];
    mImage = mTexture->mImage;

    if (mDirtyFrames != 0) {
        mDirtyFrames--;
        s32 index = 0;

        for (u32 y = 0; y < mTextureSize; y++) {
            for (u32 x = 0; x < mTextureSize; x++) {
                mImage[index] = MR::clamp(static_cast< s32 >(mSnowDepth[index]), 0, 255);
                index++;
            }
        }

        TDDraw::tileConversion8(mImage, mTextureSize, mTextureSize);
        DCStoreRange(mImage, mTextureSize * mTextureSize);
    }
}

void SnowFloorTile::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemy(pReceiver)) {
        if (MR::isEqualString(pReceiver->mHost->mName, "マリオ炎球")) {
            TVec3f localPoint;
            calcLocalPoint(pReceiver->mPosition, &localPoint, 200.0f);
            doErase(TVec2f(localPoint.x, localPoint.y), 30.0f / mCellSize, cHipdropDigAmount);
        } else if (pReceiver->mHost->getBaseMtx()) {
            TVec3f up;
            MR::calcUpVec(&up, pReceiver->mHost);
            TVec3f position(pReceiver->mPosition - up * (pReceiver->mRadius - 20.0f));
            doErase(position, 20.0f, cFireballDigAmount);
        }

        mDirtyFrames = 2;
    }
}

void SnowFloorTile::draw() const {
    MR::reinitGX();
    GXInvalidateTexAll();
    TDDraw::setup(1, 1, false);
    GXSetCullMode(GX_CULL_BACK);
    GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
    GXSetAlphaCompare(GX_GREATER, 1, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_C1, GX_CC_ONE, GX_CC_ZERO);

    for (u32 i = 0; i < static_cast< u32 >(sLayerCount); i++) {
        u8 threshold = i * (240 / sLayerCount) + 1;
        Color8 alpha(threshold, threshold, threshold, threshold);
        GXSetTevColor(GX_TEVREG0, alpha);
        GXSetAlphaCompare(GX_GREATER, threshold, GX_AOP_AND, GX_ALWAYS, 0);
        s32 r = sColorR + i * sColorStepR;
        s32 g = sColorG + i * sColorStepG;
        s32 b = sColorB + i * sColorStepB;
        s32 a = sColorA + i * sColorStepA;
        MR::clamp(static_cast< s32 >(r), 0L, 255L);
        MR::clamp(static_cast< s32 >(g), 0L, 255L);
        MR::clamp(static_cast< s32 >(b), 0L, 255L);
        MR::clamp(static_cast< s32 >(a), 0L, 255L);
        Color8 color(r, g, b, a);
        GXSetTevColor(GX_TEVREG1, color);
        drawLayer(i);
    }
}

void SnowFloorTile::drawLayer(s32 layer) const {
    mTexture->load(GX_TEXMAP0);
    TVec3f p3, p2, p1, p0;
    p0 = -mRight * mTextureSize * mCellSize - mForward * mTextureSize * mCellSize;
    p1 = mRight * mTextureSize * mCellSize - mForward * mTextureSize * mCellSize;
    p2 = mRight * mTextureSize * mCellSize + mForward * mTextureSize * mCellSize;
    p3 = -mRight * mTextureSize * mCellSize + mForward * mTextureSize * mCellSize;

    for (u32 i = 0; i < sStripCount; i++) {
        TVec3f center(mPosition + mUp * (5.0f + sLayerHeight * layer));
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        TDDraw::sendPoint(center + p0);
        GXTexCoord2f32(0.0f, 0.0f);
        TDDraw::sendPoint(center + p1);
        GXTexCoord2f32(1.0f, 0.0f);
        TDDraw::sendPoint(center + p2);
        GXTexCoord2f32(1.0f, 1.0f);
        TDDraw::sendPoint(center + p3);
        GXTexCoord2f32(0.0f, 1.0f);
        GXEnd();
    }
}
