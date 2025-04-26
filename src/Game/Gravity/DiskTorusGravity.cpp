#include "Game/Gravity.hpp"
#include "Game/Util.hpp"
#include "Inline.hpp"


#define DISK_TORUS_DISABLE_BOTH_EDGE_GRAVITY 0
#define DISK_TORUS_DISABLE_OUTER_EDGE_GRAVITY 1
#define DISK_TORUS_DISABLE_INNER_EDGE_GRAVITY 2

DiskTorusGravity::DiskTorusGravity() :
	PlanetGravity(),
	mLocalDirection(0, 1, 0),
	mRotation(0, 1, 0)
{
	mRadius = 2000.0f;
	mWorldRadius = 2000.0f;
	mDiskRadius = 0.0f;
	mEdgeType = 3;
	mEnableBothSide = true;
}

void DiskTorusGravity::setPosition(const TVec3f &rPosition) {
	mLocalPosition = rPosition;
}

void DiskTorusGravity::setDirection(const TVec3f &rDirection) {
	mLocalDirection = rDirection;
}

void DiskTorusGravity::setRadius(f32 val) {
	mRadius = val;
}

void DiskTorusGravity::setDiskRadius(f32 val) {
	if (val < 0.0f) {
		mDiskRadius = 0.0f;
	}
	else {
		mDiskRadius = val;
	}
}

void DiskTorusGravity::setEdgeType(s32 val) {
	mEdgeType = val;
}

void DiskTorusGravity::setBothSide(bool val) {
	mEnableBothSide = val;
}

inline TVec3f multInline2(const TVec3f &v, f32 s) {
    TVec3f ret(v);
    ret.x *= s;
    ret.y *= s;
    ret.z *= s;
    return ret;
}

        inline void negateInline_2(TVec3f *self, register const TVec3f &rSrc)
        {
            register TVec3f *dst = self;
            register f32 xy;
            register f32 z;
 
            __asm {
                psq_l xy, 0(rSrc), 0, 0
                ps_neg xy, xy
                psq_st xy, 0(dst), 0, 0
                lfs z, 8(rSrc)
                fneg z, z
                stfs z, 8(dst)
            };
        }
        inline TVec3f negateInline_2(const TVec3f &self) {
            TVec3f ret;
            negateInline_2(&ret, self);
            return ret;
        }
        
bool DiskTorusGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPos) const {
    
    TVec3f relativePosition;
    relativePosition = rPos - mTranslation;
    
    f32 centralAxisY = relativePosition.dot(mRotation);
    
    if(!mEnableBothSide && centralAxisY < 0.0f) {
        return false;
    }
    
    TVec3f dirOnTorusPlane = relativePosition - multInline2(mRotation, centralAxisY);
    f32 distanceToCentralAxis;
    MR::separateScalarAndDirection(&distanceToCentralAxis, &dirOnTorusPlane, dirOnTorusPlane);
    if(MR::isNearZero(distanceToCentralAxis, 0.00100000005f)) {
        
        // Just choose a direction in-plane for gravity if the object is sitting
        // on the central axis (otherwise dirOnTorusPlane is zero)
        MR::makeAxisVerticalZX(&dirOnTorusPlane, mRotation);
        
    }

    
    f32 innerRadius = mWorldRadius - mDiskRadius;
    f32 worldRadius = mWorldRadius;
    TVec3f gravity(0, 0, 0);
    f32 distance = 0.0f;

    if(distanceToCentralAxis < innerRadius) {

        if(mEdgeType == DISK_TORUS_DISABLE_BOTH_EDGE_GRAVITY || mEdgeType == DISK_TORUS_DISABLE_INNER_EDGE_GRAVITY) {
            return false;
        }

        TVec3f nearestInnerEdgePoint;
        JMAVECScaleAdd(&dirOnTorusPlane, &mTranslation, &nearestInnerEdgePoint, innerRadius);

        gravity = nearestInnerEdgePoint - rPos;
        MR::separateScalarAndDirection(&distance, &gravity, gravity);
    }
    else if(distanceToCentralAxis > worldRadius) {
        
        if(mEdgeType == DISK_TORUS_DISABLE_BOTH_EDGE_GRAVITY || mEdgeType == DISK_TORUS_DISABLE_OUTER_EDGE_GRAVITY) {
            return false;
        }

        TVec3f nearestOuterEdgePoint;
        JMAVECScaleAdd(&dirOnTorusPlane, &mTranslation, &nearestOuterEdgePoint, worldRadius);
        gravity = nearestOuterEdgePoint - rPos;
        MR::separateScalarAndDirection(&distance, &gravity, gravity);
    }
    else {
        gravity = centralAxisY >= 0.0f ? negateInline_2(mRotation) : mRotation;
        distance = __fabsf(centralAxisY);
    }
    
    if(!isInRangeDistance(distance)) {
        return false;
    }
    if(pDest) {
        *pDest = gravity;
    }
    if(pScalar) {
        *pScalar = distance;
    }

    return true;
    
}


void DiskTorusGravity::updateMtx(const TPos3f &rMtx) {
    rMtx.mult(mLocalPosition, mTranslation);
    rMtx.mult33Inline(mLocalDirection, mRotation);
    
    f32 worldScale;
    MR::separateScalarAndDirection(&worldScale, &mRotation, mRotation);
    mWorldRadius = mRadius * worldScale;
}
