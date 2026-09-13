#include "Game/MapObj/SpinDriverUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"

void SpinDriverUtil::setShadowAndClipping(LiveActor* pActor, TVec3f* pClippingCenter, f32 dropLength, f32 heightMargin, f32* pClippingRadius) {
    TVec3f dropPosition;
    TVec3f position(pActor->mPosition);
    TVec3f gravity(pActor->mGravity);
    TVec3f dropEnd;
    dropPosition.set(position - gravity * heightMargin);

    if (dropLength < 0.0f) {
        if (MR::getFirstPolyOnLineToMap(&dropEnd, nullptr, dropPosition, gravity * (1500.0f + heightMargin))) {
            dropLength = dropEnd.distance(position);
            dropLength += heightMargin;
            heightMargin = 0.5f * dropLength + heightMargin;
        } else {
            dropEnd.set(position);
            dropLength = 0.0f;
        }
    } else {
        dropEnd.set(position + gravity * dropLength);
        heightMargin = 0.5f * dropLength + heightMargin;
    }

    pClippingCenter->set((dropEnd + position) * 0.5f);
    MR::setClippingTypeSphere(pActor, heightMargin, pClippingCenter);

    if (pClippingRadius) {
        *pClippingRadius = heightMargin;
    }

    MR::setShadowDropPosition(pActor, nullptr, dropPosition);
    MR::setShadowDropDirection(pActor, nullptr, gravity);
    MR::setShadowDropLength(pActor, nullptr, dropLength);
}
