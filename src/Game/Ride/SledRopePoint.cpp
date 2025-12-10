#include "Game/Ride/SledRopePoint.hpp"
#include "Game/LiveActor/Binder.hpp"

SledRopePoint::SledRopePoint(const TVec3f& rPos, f32 bindRadius) : SwingRopePoint(rPos), mBinder(nullptr), mNoBind(false) {
    mBinder = new Binder(nullptr, &mPosition, new TVec3f(0.0f, -1.0f, 0.0f), bindRadius, 0.0f, 8);
}

void SledRopePoint::updatePos(f32 vel) {
    if (!mNoBind) {
        mPosition.add(mBinder->bind(mVelocity));
    } else {
        mBinder->clear();
        mPosition.add(mVelocity);
    }
    mVelocity.scale(vel);
}

bool SledRopePoint::bindToMapCollision(f32 rebound) {
    bool bound = false;
    for (u32 plane = 0; plane < mBinder->mPlaneNum; plane++) {
        const TVec3f* normal = mBinder->getPlane(plane)->getNormal(0);
        f32 dot = mVelocity.dot(*normal);
        if (dot < 0.0f) {
            TVec3f v1(*normal);
            v1.scale(-(1.0f + rebound) * dot);
            mVelocity.add(v1);
            bound = true;
        }
    }
    return bound;
}
