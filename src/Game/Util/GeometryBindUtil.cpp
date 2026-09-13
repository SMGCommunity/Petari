#include "Game/Util/GeometryBindUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void GeometryBindUtil_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.000003814697265625f;
    (void)0.5f;
    (void)3.0f;
}

BindSphere::BindSphere(const TVec3f& rPosition, f32 radius) : mPosition(rPosition), mRadius(radius) {
}

BindCone::BindCone(const TVec3f& rPosition, const TVec3f& rDirection, f32 height, f32 radius)
    : mPosition(rPosition), mVertex(0.0f, 0.0f, 0.0f), mDirection(rDirection), mHeight(height), mRadius(radius), mGeneratrixLength() {
    MR::normalizeOrZero(&mDirection);
    calcVertexPosition();
    calcGeneratrixLength();
}

bool BindCone::isInPoint(const TVec3f& rPoint) const {
    f32 height = mDirection.dot(rPoint - mPosition);
    f32 radius = mRadius - height * (mRadius / mHeight);
    return rPoint.squared(mPosition + mDirection * height) < radius * radius;
}

void BindCone::calcVertexPosition() {
    mVertex = mPosition + mDirection * mHeight;
}

void BindCone::calcGeneratrixLength() {
    mGeneratrixLength = MR::fastSqrtf(mHeight * mHeight + mRadius * mRadius);
}

void BindCone::setPosition(const TVec3f& rPosition) {
    mPosition = rPosition;
    calcVertexPosition();
}

void BindCone::setDirection(const TVec3f& rDirection) {
    mDirection = rDirection;
    calcVertexPosition();
}

namespace MR {
    // collision detection and position correcting for a moving sphere against a cone
    bool bindSpereToCone(BindResult* pResult, const TVec3f& rMovement, const BindSphere& rSphere, const BindCone& rCone) {
        TVec3f nextPosition;
        nextPosition.set(rSphere.mPosition + rMovement);
        f32 expandedHeight = (rCone.getGeneratrixLength() / rCone.getRadius()) * rSphere.getRadius();
        TVec3f radial(nextPosition - rCone.mPosition);
        radial.orthogonalize(rCone.mDirection);

        if (radial.isZero()) {
            radial = rMovement;
            radial.orthogonalize(rCone.mDirection);

            if (radial.isZero()) {
                f32 height = rCone.mDirection.dot(rCone.mVertex - nextPosition);
                if (-rSphere.getRadius() < height && height < 0.0f) {
                    if (pResult) {
                        pResult->mPosition = rCone.mVertex + rCone.mDirection * rSphere.getRadius();
                        pResult->mContactPosition = rCone.mVertex;
                        pResult->mNormal = rCone.mDirection;
                        pResult->mIsBound = true;
                    }

                    return true;
                }

                if (0.0f <= height && height < expandedHeight) {
                    if (pResult) {
                        pResult->mPosition = rCone.mVertex - rCone.mDirection * expandedHeight;
                        pResult->mContactPosition = rCone.mVertex - rCone.mDirection * (expandedHeight + rSphere.getRadius());
                        pResult->mNormal = -rCone.mDirection;
                        pResult->mIsBound = true;
                    }

                    return true;
                }

                if (pResult) {
                    pResult->mPosition = nextPosition;
                    pResult->mIsBound = false;
                }

                return false;
            }
        }

        radial.setLength(rCone.getRadius());
        radial.add(rCone.mPosition);
        TVec3f generatrixDirection(radial - rCone.mVertex);
        f32 generatrixLength = rCone.getGeneratrixLength();
        generatrixDirection /= generatrixLength;
        f32 along = generatrixDirection.dot(nextPosition - rCone.mVertex);
        s32 within = 0.0f < along && along < generatrixLength;
        along = MR::clamp(along, 0.0f, generatrixLength);
        TVec3f closest(rCone.mVertex + generatrixDirection * along);
        TVec3f contactDirection(nextPosition - closest);
        f32 distance;
        separateScalarAndDirection(&distance, &contactDirection, contactDirection);
        bool wasInside = rCone.isInPoint(rSphere.mPosition);
        bool isInside = rCone.isInPoint(nextPosition);

        if (wasInside) {
            f32 baseDistance = rCone.mDirection.dot(rCone.mPosition - rSphere.mPosition) - rSphere.getRadius();
            f32 motionAlong = rCone.mDirection.dot(rMovement);
            TVec3f baseProjection(rSphere.mPosition + rMovement * (motionAlong - baseDistance));
            baseProjection.killElement(baseProjection - rCone.mPosition, rCone.mDirection);

            if (baseProjection.squared() < rCone.getRadius() * rCone.getRadius()) {
                if (rCone.mDirection.dot(rCone.mVertex - nextPosition) < expandedHeight) {
                    if (pResult) {
                        pResult->mPosition = rCone.mVertex - rCone.mDirection * expandedHeight;
                        pResult->mContactPosition = rCone.mVertex - rCone.mDirection * (expandedHeight + rSphere.getRadius());
                        pResult->mNormal = -rCone.mDirection;
                        pResult->mIsBound = true;
                    }

                    return true;
                }
            }
        }

        if (within && wasInside != isInside) {
            if (pResult) {
                TVec3f correction(nextPosition - contactDirection * (distance + rSphere.getRadius()));
                pResult->mContactPosition = closest;
                pResult->mNormal = -contactDirection;
                pResult->mIsBound = true;
            }

            return true;
        }

        if (distance < rSphere.getRadius()) {
            if (pResult) {
                TVec3f pushedPosition(nextPosition + contactDirection * (rSphere.getRadius() - distance));
                pResult->mPosition = pushedPosition;
                pResult->mContactPosition = closest;
                pResult->mNormal = contactDirection;
                pResult->mIsBound = true;
            }

            return true;
        }

        if (pResult) {
            pResult->mPosition = nextPosition;
            pResult->mIsBound = false;
        }

        return false;
    }

    void updateBindPositionAndVelocity(TVec3f* pPosition, TVec3f* pVelocity, const BindResult& rResult, f32 factor) {
        pPosition->set(rResult.mPosition);
        if (rResult.mIsBound) {
            f32 dot = pVelocity->dot(rResult.mNormal);
            if (dot < 0.0f) {
                pVelocity->add((-rResult.mNormal * dot) * factor);
            }
        }
    }
}  // namespace MR
