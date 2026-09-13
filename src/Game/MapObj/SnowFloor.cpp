#include "Game/MapObj/SnowFloor.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"

namespace {
    u16 sTextureSize = 128;
    u16 sTextureShift = 7;
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
    f32 cSnowFallRateBottom = 30.0f;
    // cSnowFallRateLimit
    f32 cSnowFallRate = 0.2f;
    f32 cSnowFallRateQuick = 0.5f;
    f32 cSnowFallRateFirst = 0.1f;
    f32 cPlayerWalkResist = 0.5f;
    u16 sCellSize = 1024 / sTextureSize;
}  // namespace

SnowFloor::~SnowFloor() {
}

SnowFloor::SnowFloor(const char* pName) : LiveActor(pName), _E4(), _E8() {
}

void SnowFloor::init(const JMapInfoIter& rIter) {
    initHitSensor(1);
    MR::addHitSensorEye(this, "eye", 16, 1.4142f * (sTextureSize * sCellSize), TVec3f(0.0f, 0.0f, 0.0f));
    mSnowDepth = new f32[sTextureSize * sTextureSize];
    mBaseDepth = new u8[sTextureSize * sTextureSize];

    for (u32 y = 0; y < sTextureSize; y++) {
        for (u32 x = 0; x < sTextureSize; x++) {
            mSnowDepth[(y << sTextureShift) + x] = 32.0f + 120.0f * MR::getRandom();
            mBaseDepth[(y << sTextureShift) + x] = mSnowDepth[(y << sTextureShift) + x];
            mSnowDepth[(y << sTextureShift) + x] += 80.0f;
        }
    }

    initLocalPoint(rIter);
    drawSetup();
    mTextures[0] = new JUTTexture(sTextureSize, sTextureSize, GX_TF_I8);
    mTextures[1] = new JUTTexture(sTextureSize, sTextureSize, GX_TF_I8);
    mTexture = mTextures[0];
    mTextureIndex = 0;
    mImage = mTexture->mImage;
    mWalkingResist = 0.0f;
    mResistFrames = 0;
    mCheckCount = 0;

    for (s32 i = 0; i < 8; i++) {
        mCheckList[i] = nullptr;
    }

    mLastDpdPoint.zero();
    mLastPlayerPoint.zero();
    MR::connectToSceneNormalMapObj(this);
    MR::validateClipping(this);
    MR::setClippingTypeSphere(this, 1.414f * 1024.0f);
    MR::setClippingFar100m(this);
    initSound(4, false);
    makeActorAppeared();
    createReduceMap(800 / sCellSize);
    mDirtyFrames = 2;
}

void SnowFloor::drawSetup() {
    mTexCoords = new f32[sStripCount + 1];
    mDirections = new TVec3f[sStripCount];
    f32 halfStripAngle = mHalfAngle / sStripCount;
    f32 angleRange = 2.0f * mHalfAngle - 2.0f * halfStripAngle;
    u32 i;

    for (i = 0; i < sStripCount; i++) {
        mTexCoords[i] = static_cast< f32 >(i) / sStripCount;
        Mtx mtx;
        f32 angle = angleRange * (static_cast< f32 >(i) / (sStripCount - 1)) - mHalfAngle;
        angle += halfStripAngle;
        PSMTXRotAxisRad(mtx, &mAxis, angle);
        PSMTXMultVecSR(mtx, &mUp, &mDirections[i]);
    }

    mTexCoords[i] = 1.0f;
}

void SnowFloor::initLocalPoint(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    Mtx mtx;
    MR::makeMtxTR(mtx, 0.0f, 0.0f, 0.0f, mRotation.x, mRotation.y, mRotation.z);
    mUp.set(0.0f, 1.0f, 0.0f);
    mAxis.set(0.0f, 0.0f, 1.0f);
    PSMTXMultVecSR(mtx, &mUp, &mUp);
    PSMTXMultVecSR(mtx, &mAxis, &mAxis);
    f32 halfLength = sTextureSize * sCellSize;
    mRadius = 900.0f;
    mHalfLength = halfLength;
    mHalfAngle = halfLength / 900.0f;
}

bool SnowFloor::calcLocalPoint(const TVec3f& rPosition, TVec3f* pPoint) {
    TVec3f offset(rPosition - mPosition);
    TVec3f radial;
    MR::vecKillElement(offset, mAxis, &radial);
    f32 radius = radial.length();
    f32 angle = MR::diffAngleSignedHorizontal(mUp, radial, mAxis);
    f32 axisPos = offset.dot(mAxis) / mHalfLength;

    if (MR::isInRange(angle, -mHalfAngle, mHalfAngle) && MR::isInRange(axisPos, -1.0f, 1.0f)) {
        pPoint->set(angle / mHalfAngle, axisPos, radius);
        return true;
    }

    return false;
}

void SnowFloor::createReduceMap(u16 size) {
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

void SnowFloor::digPlayerWalk(const TVec3f& rPoint) {
    TVec2f point(rPoint.x, rPoint.y);

    if (rPoint.z < mRadius - 50.0f) {
        return;
    }

    if (rPoint.z > 50.0f + mRadius) {
        return;
    }

    point.x += 1.0f;
    point.y += 1.0f;
    point.x *= 0.5f;
    point.y *= 0.5f;
    point.y = 1.0f - point.y;
    f32 depth = doMove(mLastPlayerPoint, point, 20.0f / sCellSize, cPlayerDigAmount);
    mLastPlayerPoint = point;
    f32 excess = depth - 200.0f;

    if (excess < 0.0f) {
        if (mResistFrames != 0) {
            MR::setPlayerWalkingResist(mWalkingResist);
            mWalkingResist *= 0.8f;
        } else {
            MR::setPlayerWalkingResist(0.0f);
        }
    } else {
        f32 resist = MR::clamp(excess / 500.0f, 0.0f, 1.0f);
        resist *= cPlayerWalkResist;
        MR::setPlayerWalkingResist(resist);

        if (mResistFrames != 0) {
            if (resist > mWalkingResist) {
                mWalkingResist = resist;
                mResistFrames = 5;
            }
        } else {
            mWalkingResist = resist;
            mResistFrames = 5;
        }
    }

    if (mResistFrames != 0) {
        mResistFrames--;
    }
}

void SnowFloor::digDpd(const TVec3f& rPoint) {
    TVec2f point(rPoint.x, rPoint.y);

    if (rPoint.z < mRadius - 250.0f) {
        return;
    }

    if (rPoint.z > 250.0f + mRadius) {
        return;
    }

    point.x += 1.0f;
    point.y += 1.0f;
    point.x *= 0.5f;
    point.y *= 0.5f;
    point.y = 1.0f - point.y;
    s32 volume = doMove(mLastDpdPoint, point, 50.0f / sCellSize, cDpdDigAmount) / 200.0f;

    if (volume > 100) {
        volume = 100;
    }

    MR::startLevelSound(this, "SE_OJ_LV_SNOW_DIG", volume);
    mLastDpdPoint = point;
}

void SnowFloor::control() {
    TVec3f position(*MR::getPlayerPos());
    TVec3f localPoint;
    bool changed = false;
    updateCheckList();

    if (calcLocalPoint(position, &localPoint)) {
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

    if (MR::isStarPointerInScreen(0)) {
        position = *MR::getStarPointerWorldPosUsingDepth(0);

        if (calcLocalPoint(position, &localPoint)) {
            digDpd(localPoint);
            changed = true;
        }
    }

    if (MR::isStarPointerInScreen(1)) {
        position = *MR::getStarPointerWorldPosUsingDepth(1);

        if (calcLocalPoint(position, &localPoint)) {
            digDpd(localPoint);
            changed = true;
        }
    }

    for (u32 y = 0; y < sTextureSize; y++) {
        u32 row = y << sTextureShift;

        for (u32 x = 0; x < sTextureSize; x++) {
            u32 index = row + x;

            if (mSnowDepth[index] < 10.0f) {
                mSnowDepth[index] += cSnowFallRateFirst * MR::getRandom();
            } else if (mSnowDepth[index] < cSnowFallRateBottom) {
                mSnowDepth[index] += cSnowFallRateQuick * MR::getRandom();
            } else {
                f32 limit = 50.0f + mBaseDepth[index];
                f32 deficit = limit - mSnowDepth[index];

                if (deficit > 0.0f) {
                    deficit /= limit - cSnowFallRateBottom;
                    mSnowDepth[index] += deficit * (cSnowFallRate * MR::getRandom());
                }
            }

            mSnowDepth[index] = MR::clamp(mSnowDepth[index], 0.0f, 100.0f + mBaseDepth[index]);
            changed = true;
        }
    }

    if (changed) {
        mDirtyFrames = 2;
    }

    createTexture();
}

bool SnowFloor::doErase(const TVec3f& rPosition, f32 radius, f32 amount) {
    TVec3f localPoint;

    if (calcLocalPoint(rPosition, &localPoint)) {
        f32 radiusSq = radius * radius;
        f32 height = localPoint.z - mRadius;

        if (radiusSq < height * height) {
            return false;
        }

        f32 localRadius = MR::sqrt(radiusSq - height * height);
        TVec2f point(localPoint.x, localPoint.y);
        point.x += 1.0f;
        point.y += 1.0f;
        point.x *= 0.5f;
        point.y *= 0.5f;
        point.y = 1.0f - point.y;
        doErase(point, localRadius / sCellSize, amount);
        return true;
    }

    return false;
}

f32 SnowFloor::doMove(TVec2f from, TVec2f to, f32 radius, f32 amount) {
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
    f32 outerRadius = 1.732051f * radius;
    to.x *= sTextureSize;
    to.y *= sTextureSize;
    s32 left = static_cast< s16 >(to.x - outerRadius) - 1;
    s32 top = static_cast< s16 >(to.y - outerRadius) - 1;
    s32 right = static_cast< s16 >(to.x + outerRadius) + 1;
    s32 bottom = static_cast< s16 >(to.y + outerRadius) + 1;
    minX = MR::clamp(left, 0L, static_cast< s32 >(sTextureSize));
    u32 minY = MR::clamp(top, 0L, static_cast< s32 >(sTextureSize));
    u32 maxX = MR::clamp(right, 0L, static_cast< s32 >(sTextureSize));
    u32 maxY = MR::clamp(bottom, 0L, static_cast< s32 >(sTextureSize));
    f32 outerRadiusSq = outerRadius * outerRadius;
    f32 radiusSq = radius * radius;

    for (u32 y = minY; y < maxY; y++) {
        u32 row = y * sTextureSize;

        for (u32 x = minX; x < maxX; x++) {
            TVec2f offset(x - to.x, y - to.y);
            f32 distanceSq = offset.squared();

            if (distanceSq <= radiusSq) {
                u32 index = row + x;
                depthTotal += mSnowDepth[index];

                if (mSnowDepth[index] > amount) {
                    removed += amount;
                    mSnowDepth[index] -= amount;
                } else {
                    removed += mSnowDepth[index];
                    mSnowDepth[index] = 0.0f;
                }
            } else if (distanceSq <= outerRadiusSq) {
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

    if (count == 0) {
        return 0.0f;
    }

    removed /= weightTotal;

    for (u32 i = 0; i < count; i++) {
        mSnowDepth[(ys[i] << sTextureShift) + xs[i]] += removed * weights[i];
    }

    return depthTotal;
}

void SnowFloor::doErase(TVec2f point, f32 radius, f32 amount) {
    point.x *= sTextureSize;
    point.y *= sTextureSize;
    u32 minX;
    s32 left = static_cast< s16 >(point.x - radius) - 1;
    s32 top = static_cast< s16 >(point.y - radius) - 1;
    s32 right = static_cast< s16 >(point.x + radius) + 1;
    s32 bottom = static_cast< s16 >(point.y + radius) + 1;
    minX = MR::clamp(left, 0L, static_cast< s32 >(sTextureSize));
    u32 minY = MR::clamp(top, 0L, static_cast< s32 >(sTextureSize));
    u32 maxX = MR::clamp(right, 0L, static_cast< s32 >(sTextureSize));
    u32 maxY = MR::clamp(bottom, 0L, static_cast< s32 >(sTextureSize));
    f32 radiusSq = radius * radius;

    for (u32 y = minY; y < maxY; y++) {
        u32 row = y * sTextureSize;

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

void SnowFloor::createTexture() {
    mTextureIndex = 1 - mTextureIndex;
    mTexture = mTextures[mTextureIndex];
    mImage = mTexture->mImage;

    if (mDirtyFrames != 0) {
        mDirtyFrames--;
        s32 index = 0;

        for (u32 y = 0; y < sTextureSize; y++) {
            for (u32 x = 0; x < sTextureSize; x++) {
                mImage[index] = MR::clamp(static_cast< s32 >(mSnowDepth[index]), 0, 255);
                index++;
            }
        }

        TDDraw::tileConversion8(mImage, sTextureSize, sTextureSize);
        DCStoreRange(mImage, sTextureSize * sTextureSize);
    }
}

void SnowFloor::addCheckList(HitSensor* pSensor) {
    for (u32 i = 0; i < mCheckCount; i++) {
        if (mCheckList[i] == pSensor) {
            return;
        }

        if (mCheckList[i]->mHost == pSensor->mHost) {
            return;
        }
    }

    mCheckList[mCheckCount] = pSensor;
    mCheckCount++;
}

void SnowFloor::updateCheckList() {
    bool discovered = false;

    if (mCheckCount != 0) {
        HitSensor* sensor = mCheckList[0];
        TVec3f localPoint;

        if (calcLocalPoint(sensor->mPosition, &localPoint)) {
            TVec2f point(localPoint.x, localPoint.y);
            point.x += 1.0f;
            point.y += 1.0f;
            point.x *= 0.5f;
            point.y *= 0.5f;
            point.y = 1.0f - point.y;
            f32 cellSize = sCellSize;
            f32 radius = sensor->mRadius;
            f32 cover = calcCoverSnow(point, radius / cellSize);
            f32 sensorRadius = sensor->mRadius;
            f32 threshold = 100.0f * sensorRadius;

            if (cover < threshold) {
                discovered = MR::sendArbitraryMsg(ACTMES_NOTIFY_DISCOVER_SNOW, sensor, getSensor("eye"));
            }
        }

        mCheckCount--;

        for (u32 i = 0; i < mCheckCount; i++) {
            mCheckList[i] = mCheckList[i + 1];
        }

        if (!discovered) {
            mCheckList[mCheckCount] = sensor;
            mCheckCount++;
        }
    }
}

f32 SnowFloor::calcCoverSnow(const TVec2f& rPoint, f32 radius) {
    f32 total = 0.0f;
    TVec2f point(rPoint);
    point.x *= sTextureSize;
    point.y *= sTextureSize;
    f32 outerRadius = 1.732051f;
    outerRadius *= radius;
    s32 left = static_cast< s16 >(point.x - outerRadius) - 1;
    s32 top = static_cast< s16 >(point.y - outerRadius) - 1;
    s32 right = static_cast< s16 >(point.x + outerRadius) + 1;
    s32 bottom = static_cast< s16 >(point.y + outerRadius) + 1;
    u32 minX = MR::clamp(left, 0L, static_cast< s32 >(sTextureSize));
    u32 minY = MR::clamp(top, 0L, static_cast< s32 >(sTextureSize));
    u32 maxX = MR::clamp(right, 0L, static_cast< s32 >(sTextureSize));
    u32 maxY = MR::clamp(bottom, 0L, static_cast< s32 >(sTextureSize));
    f32 radiusSq = radius * radius;

    for (u32 y = minY; y < maxY; y++) {
        u32 row = y * sTextureSize;

        for (u32 x = minX; x < maxX; x++) {
            TVec2f offset(x - point.x, y - point.y);

            if (offset.squared() <= radiusSq) {
                total += mSnowDepth[row + x];
            }
        }
    }

    return total;
}

void SnowFloor::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorEnemy(pReceiver)) {
        return;
    }

    if (MR::isEqualString(pReceiver->mHost->mName, "マリオ炎球")) {
        doErase(pReceiver->mPosition, pReceiver->mRadius, cFireballDigAmount);
    } else {
        for (u32 i = 0; i < mCheckCount; i++) {
            if (mCheckList[i] == pReceiver) {
                return;
            }

            if (mCheckList[i]->mHost == pReceiver->mHost) {
                return;
            }
        }

        if (MR::sendArbitraryMsg(ACTMES_ASK_HIDDEN_BY_SNOW, pReceiver, pSender) == true) {
            addCheckList(pReceiver);
            return;
        }

        if (pReceiver->mHost->getBaseMtx()) {
            TVec3f up;
            MR::calcUpVec(&up, pReceiver->mHost);
            TVec3f position(pReceiver->mPosition - up * (pReceiver->mRadius - 20.0f));
            doErase(position, 20.0f, cFireballDigAmount);
        }
    }

    mDirtyFrames = 2;
}

void SnowFloor::draw() const {
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

void SnowFloor::drawLayer(s32 layer) const {
    mTexture->load(GX_TEXMAP0);
    TVec3f p3, p2, p1, p0;
    TVec3f axis(mAxis);
    TVec3f tangent, cross;
    f32 halfWidth = (static_cast< f32 >(sTextureSize) * sCellSize) / sStripCount;
    f32 halfLength = static_cast< f32 >(sTextureSize) * sCellSize;

    for (u32 i = 0; i < sStripCount; i++) {
        TVec3f center(mPosition + mDirections[i] * (mRadius + sLayerHeight * layer));
        const TVec3f& direction = mDirections[i];
        tangent.cross(direction, axis);
        MR::normalizeOrZero(&tangent);
        cross.cross(tangent, direction);
        MR::normalizeOrZero(&cross);
        tangent = -tangent;

        if (i != 0) {
            p0 = p1;
            p3 = p2;
        } else {
            p0 = center - tangent * halfWidth + cross * halfLength;
            p3 = center - tangent * halfWidth - cross * halfLength;
        }

        p1 = center + tangent * halfWidth + cross * halfLength;
        p2 = center + tangent * halfWidth - cross * halfLength;
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        TDDraw::sendPoint(p0);
        GXTexCoord2f32(mTexCoords[i], 0.0f);
        TDDraw::sendPoint(p1);
        GXTexCoord2f32(mTexCoords[i + 1], 0.0f);
        TDDraw::sendPoint(p2);
        GXTexCoord2f32(mTexCoords[i + 1], 1.0f);
        TDDraw::sendPoint(p3);
        GXTexCoord2f32(mTexCoords[i], 1.0f);
        GXEnd();
    }
}
