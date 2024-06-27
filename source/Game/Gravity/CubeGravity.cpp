#include "Game/Gravity.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

#define REGION_NEGATIVE_X 0
#define REGION_BETWEEN_X 1
#define REGION_POSITIVE_X 2

#define REGION_NEGATIVE_Y 0
#define REGION_BETWEEN_Y 3
#define REGION_POSITIVE_Y 6

#define REGION_NEGATIVE_Z 0
#define REGION_BETWEEN_Z 9
#define REGION_POSITIVE_Z 18

#define ENCODE_REGION(x, y, z) \
    (x + y + z)

#define REGION_X_FACE_POSITIVE ENCODE_REGION(REGION_POSITIVE_X, REGION_BETWEEN_Y, REGION_BETWEEN_Z)
#define REGION_X_FACE_NEGATIVE ENCODE_REGION(REGION_NEGATIVE_X, REGION_BETWEEN_Y, REGION_BETWEEN_Z)
#define REGION_Y_FACE_POSITIVE ENCODE_REGION(REGION_BETWEEN_X, REGION_POSITIVE_Y, REGION_BETWEEN_Z)
#define REGION_Y_FACE_NEGATIVE ENCODE_REGION(REGION_BETWEEN_X, REGION_NEGATIVE_Y, REGION_BETWEEN_Z)
#define REGION_Z_FACE_POSITIVE ENCODE_REGION(REGION_BETWEEN_X, REGION_BETWEEN_Y, REGION_POSITIVE_Z)
#define REGION_Z_FACE_NEGATIVE ENCODE_REGION(REGION_BETWEEN_X, REGION_BETWEEN_Y, REGION_NEGATIVE_Z)

#define AXIS_X 1
#define AXIS_Y 3
#define AXIS_Z 9

#define ENCODE_EDGE(axis, half1, half2) \
    (axis + half1 + half2)

#define ENCODE_CORNER(signumx, signumy, signumz) \
    ((signumx + 1) + (signumy + 1) * 3 + (signumz + 1) * 9)

template <>
bool TVec3f::isZero() const
{
    register const TVec3f *rSrc = this;
    register f32 sum;

    __asm {
		psq_l     f1, 0(rSrc), 0, 0
		lfs       sum, 8(rSrc)
		ps_mul    f1, f1, f1
		ps_madd   sum, sum, sum, f1
		ps_sum0   sum, sum, f1, f1
    };

    return sum <= 0.0000038146973f;
}

template <>
float TVec3f::normalize(const TVec3f &rSrc)
{
    x = rSrc.x;
    y = rSrc.y;
    z = rSrc.z;
    float magnitude = PSVECMag(toCVec());
    PSVECNormalize(toCVec(), toVec());
    return magnitude;
}

CubeGravity::CubeGravity() : PlanetGravity()
{

    lenX = 1.0;
    lenY = 1.0;
    lenZ = 1.0;
    mActiveFaces = 63;

    mCube.identity();
    mPosition.identity();
}

void CubeGravity::setCube(const TPos3f &rCube)
{
    mCube = rCube;

    updateIdentityMtx();
}

void CubeGravity::updateMtx(const TPos3f &rMtx)
{
    mPosition.concat(rMtx, mCube);
    TVec3f dir;
    mPosition.getXDir(dir);
    lenX = PSVECMag(dir.toCVec());
    mPosition.getYDir(dir);
    lenY = PSVECMag(dir.toCVec());
    mPosition.getZDir(dir);
    lenZ = PSVECMag(dir.toCVec());
}

bool CubeGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const
{
    int area = calcGravityArea(rPosition);
    if (area < 0) {
        return false;
    }
    TVec3f gravityForce;
    float scalar;
    if (!calcFaceGravity(rPosition, area, &gravityForce, &scalar) && !calcCornerGravity(rPosition, area, &gravityForce, &scalar) && !calcEdgeGravity(rPosition, area, &gravityForce, &scalar)) {
        return false;
    }

    if (isInRangeDistance(scalar)) {
        return false;
    }

    if (pDest != NULL) {
        *pDest = gravityForce;
    }

    if (pScalar != NULL) {
        *pScalar = scalar;
    }
    return true;
}

int CubeGravity::calcGravityArea(const TVec3f &rPosition) const
{
    TVec3f dirX, dirY, dirZ, trans;
    mPosition.getXDir(dirX);
    mPosition.getYDir(dirY);
    mPosition.getZDir(dirZ);
    mPosition.getTrans(trans);
    TVec3f relativePosition = rPosition - trans;
    int area;    // Region of the cube
    float xDirDistance = relativePosition.dot(dirX) / lenX, yDirDistance = relativePosition.dot(dirY) / lenY, zDirDistance = relativePosition.dot(dirZ) / lenZ;

    if (xDirDistance < -lenX) {
        if ((mActiveFaces & 2) != 2) {
            return -1;
        }
        area = REGION_NEGATIVE_X;
    }
    else {
        if (xDirDistance <= lenX) {
            area = REGION_BETWEEN_X;
        }
        else {
            if ((mActiveFaces & 1) != 1) {
                return -1;
            }
            area = REGION_POSITIVE_X;
        }
    }

    if (yDirDistance < -lenY) {
        if ((mActiveFaces & 8) != 8) {
            return -1;
        }
        area += REGION_NEGATIVE_Y;
    }
    else {
        if (yDirDistance <= lenY) {
            area += REGION_BETWEEN_Y;
        }
        else {
            if ((mActiveFaces & 4) != 4) {
                return -1;
            }
            area += REGION_POSITIVE_Y;
        }
    }

    if (zDirDistance < -lenZ) {
        if ((mActiveFaces & 32) != 32) {
            return -1;
        }
        area += REGION_NEGATIVE_Z;
    }
    else {
        if (zDirDistance <= lenZ) {
            area += REGION_BETWEEN_Z;
        }
        else {
            if ((mActiveFaces & 16) != 16) {
                return -1;
            }
            area += REGION_POSITIVE_Z;
        }
    }

    return area;
}

bool CubeGravity::calcFaceGravity(const TVec3f &rPosition, s32 area, TVec3f *pDest, f32 *pScalar) const
{
    TVec3f antiFaceDir;    // Negative of the normal vector of the face an object is on
    switch (area) {
        
        case REGION_Z_FACE_NEGATIVE:
            mPosition.getZDir(antiFaceDir);
            break;

        case REGION_Y_FACE_NEGATIVE:
            mPosition.getYDir(antiFaceDir);
            break;

        case REGION_X_FACE_NEGATIVE:
            mPosition.getXDir(antiFaceDir);
            break;

        case REGION_X_FACE_POSITIVE:
            mPosition.getXDir(antiFaceDir);
            JGeometry::negateInternal(&antiFaceDir.x, &antiFaceDir.x);
            break;

        case REGION_Y_FACE_POSITIVE:
            mPosition.getYDir(antiFaceDir);
            JGeometry::negateInternal(&antiFaceDir.x, &antiFaceDir.x);
            break;

        case REGION_Z_FACE_POSITIVE:
            mPosition.getZDir(antiFaceDir);
            JGeometry::negateInternal(&antiFaceDir.x, &antiFaceDir.x);
            break;

        default:
            return false;
    }
    TVec3f trans;
    f32 length;
    mPosition.getTrans(trans);
    MR::separateScalarAndDirection(&length, &antiFaceDir, antiFaceDir);
    float height = antiFaceDir.dot(trans - rPosition) - length;
    if (height < 0.0f) {
        height = 0.0f;
    }
    *pDest = antiFaceDir;
    *pScalar = height;
    return true;
}

TVec3f negate(const TVec3f &in)
{
    TVec3f tmp;
    JGeometry::negateInternal(&in.x, &tmp.x);
    return tmp;
}

bool CubeGravity::calcEdgeGravity(const TVec3f &rPosition, s32 area, TVec3f *pDest, f32 *pScalar) const
{
    // There is a mistake here: so long as area is not both even and negative, the function will not
    // return here. The intent is that area should be neither even nor negative, since all edges
    // are odd and positive. However, this mistake does not really matter since the switch will
    // return if this does not.
    if (!(((area & 1) ^ ((area & 0x80000000) >> 31)) - ((area & 0x80000000) >> 31))
        || area == ENCODE_REGION(REGION_BETWEEN_X, REGION_BETWEEN_Y, REGION_BETWEEN_Z)) {
        return false;
    }
    
    TVec3f edgeVector, edgeTranslation, xDir, yDir, zDir, trans, positionOppositeInOrthogonalPlane;
    
    mPosition.getXDir(xDir);
    mPosition.getYDir(yDir);
    mPosition.getZDir(zDir);

    switch (area) {

        case ENCODE_EDGE(AXIS_X, REGION_NEGATIVE_Y, REGION_NEGATIVE_Z):
            edgeVector = xDir;
            edgeTranslation = negate(yDir) - zDir;
            break;

        case ENCODE_EDGE(AXIS_Y, REGION_NEGATIVE_X, REGION_NEGATIVE_Z):
            edgeVector = yDir;
            edgeTranslation = negate(xDir) - zDir;
            break;

        case ENCODE_EDGE(AXIS_Y, REGION_POSITIVE_X, REGION_NEGATIVE_Z):
            edgeVector = yDir;
            edgeTranslation = xDir - zDir;
            break;

        case ENCODE_EDGE(AXIS_X, REGION_POSITIVE_Y, REGION_NEGATIVE_Z):
            edgeVector = xDir;
            edgeTranslation = yDir - zDir;
            break;

        case ENCODE_EDGE(AXIS_Z, REGION_NEGATIVE_X, REGION_NEGATIVE_Y):
            edgeVector = zDir;
            edgeTranslation = negate(xDir) - yDir;
            break;

        case ENCODE_EDGE(AXIS_Z, REGION_POSITIVE_X, REGION_NEGATIVE_Y):
            edgeVector = zDir;
            edgeTranslation = xDir - yDir;
            break;

        case ENCODE_EDGE(AXIS_Z, REGION_NEGATIVE_X, REGION_POSITIVE_Y):
            edgeVector = zDir;
            edgeTranslation = negate(xDir).translate(yDir);
            break;

        case ENCODE_EDGE(AXIS_Z, REGION_POSITIVE_X, REGION_POSITIVE_Y):
            edgeVector = zDir;
            edgeTranslation = xDir.translate(yDir);
            break;

        case ENCODE_EDGE(AXIS_X, REGION_NEGATIVE_Y, REGION_POSITIVE_Z):
            edgeVector = xDir;
            edgeTranslation = negate(yDir).translate(zDir);
            break;

        case ENCODE_EDGE(AXIS_Y, REGION_NEGATIVE_X, REGION_POSITIVE_Z):
            edgeVector = yDir;
            edgeTranslation = negate(xDir).translate(zDir);
            break;

        case ENCODE_EDGE(AXIS_Y, REGION_POSITIVE_X, REGION_POSITIVE_Z):
            edgeVector = yDir;
            edgeTranslation = xDir.translate(zDir);
            break;

        case ENCODE_EDGE(AXIS_X, REGION_POSITIVE_Y, REGION_POSITIVE_Z):
            edgeVector = xDir;
            edgeTranslation = yDir.translate(zDir);
            break;

        default:
            return false;

    }

    mPosition.getTrans(trans);
    edgeTranslation += trans;

    MR::normalizeOrZero(&edgeVector);

    positionOppositeInOrthogonalPlane.rejection(edgeTranslation - rPosition, edgeVector);

    if (positionOppositeInOrthogonalPlane.isZero()) {
        pDest->normalize(edgeTranslation - trans);
        *pScalar = 0.0;
    }
    else {
        *pScalar = pDest->normalize(positionOppositeInOrthogonalPlane);
    }

    return true;
}

bool CubeGravity::calcCornerGravity(const TVec3f &rPosition, s32 area, TVec3f *pDest, f32 *pScalar) const
{
    TVec3f vertex, xDir, yDir, zDir, trans;
    
    mPosition.getXDir(xDir);
    mPosition.getYDir(yDir);
    mPosition.getZDir(zDir);

    switch (area) {
        
        case ENCODE_CORNER(-1, -1, -1):
            vertex = negate(xDir) - yDir - zDir;
            break;
            
        case ENCODE_CORNER(1, -1, -1):
            vertex = xDir - yDir - zDir;
            break;
            
        case ENCODE_CORNER(-1, 1, -1):
            vertex = negate(xDir).translate(yDir) - zDir;
            break;
            
        case ENCODE_CORNER(1, 1, -1):
            vertex = xDir.translate(yDir) - zDir;
            break;
            
        case ENCODE_CORNER(-1, -1, 1):
            vertex = (negate(xDir) - yDir).translate(zDir);
            break;
            
        case ENCODE_CORNER(1, -1, 1):
            vertex = (xDir - yDir).translate(zDir);
            break;
            
        case ENCODE_CORNER(-1, 1, 1):
            vertex = negate(xDir).translate(yDir).translate(zDir);
            break;
            
        case ENCODE_CORNER(1, 1, 1):
            vertex = xDir.translate(yDir).translate(zDir);
            break;
            
        default:
            return false;
            
    }
    
    mPosition.getTrans(trans);
    vertex += trans;

    TVec3f gravity = vertex - rPosition;
    if (gravity.isZero()) {
        *pScalar = 0.0;
        pDest->normalize(vertex - trans);
    }
    else {
        *pScalar = pDest->normalize(gravity);
    }

    return true;
}
