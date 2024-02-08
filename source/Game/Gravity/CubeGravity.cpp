#include "Game/Gravity.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JMath/JMath.hpp"

template<>
bool TVec3f::isZero() const {
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

template<>
float TVec3f::normalize(const TVec3f& rSrc) {
	x = rSrc.x;
	y = rSrc.y;
	z = rSrc.z;
	float magnitude = PSVECMag(toCVec());
	PSVECNormalize(toCVec(), toVec());
	return magnitude;
}

CubeGravity::CubeGravity() : PlanetGravity() {

	lenX = 1.0;
	lenY = 1.0;
	lenZ = 1.0;
	mActiveFaces = 63;

	mCube.identity();
	mPosition.identity();
}

void CubeGravity::setCube(const TPos3f & cube) {
	mCube = cube;
	
	updateIdentityMtx();
}

void CubeGravity::updateMtx(const TPos3f &rMtx) {
	mPosition.concat(rMtx, mCube);
	TVec3f dir;
	mPosition.getXDir(dir);
	lenX = VECMag(dir.toCVec());
	mPosition.getYDir(dir);
	lenY = VECMag(dir.toCVec());
	mPosition.getZDir(dir);
	lenZ = VECMag(dir.toCVec());
}

bool CubeGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
	int area = calcGravityArea(rPosition);
	if(area < 0) return false;
	TVec3f gravityForce;
	float scalar;
	if (
		!calcFaceGravity(rPosition, area, &gravityForce, &scalar)
		&& !calcCornerGravity(rPosition, area, &gravityForce, &scalar)
		&& !calcEdgeGravity(rPosition, area, &gravityForce, &scalar)
	) return false;
		
	if(isInRangeDistance(scalar)) return false;

	if(pDest != NULL)
		*pDest = gravityForce;

	if(pScalar != NULL)
		*pScalar = scalar;
	return true;
}

int CubeGravity::calcGravityArea(const TVec3f &rPosition) const { // Area as in region, not area as in Area = Base * h
	TVec3f dirX, dirY, dirZ, trans;
	mPosition.getXDir(dirX);
	mPosition.getYDir(dirY);
	mPosition.getZDir(dirZ);
	mPosition.getTrans(trans);
	TVec3f relativePosition = rPosition - trans;
	int area; // Region of the cube
	float xDirDistance = relativePosition.dot(dirX) / lenX, yDirDistance = relativePosition.dot(dirY) / lenY, zDirDistance = relativePosition.dot(dirZ) / lenZ;

	if(xDirDistance < -lenX) {
		if((mActiveFaces & 2) != 2) return -1;
		area = 0;
	}
	else {
		if(xDirDistance <= lenX) {
			area = 1;
		}
		else {
			if((mActiveFaces & 1) != 1) return -1;
			area = 2;
		}
	}

	if(yDirDistance < -lenY) {
		if((mActiveFaces & 8) != 8) return -1;
	}
	else {
		if(yDirDistance <= lenY) {
			area += 3;
		}
		else {
			if((mActiveFaces & 4) != 4) return -1;
			area += 6;
		}
	}
	
	if(zDirDistance < -lenZ) {
		if((mActiveFaces & 32) != 32) return -1;
	}
	else {
		if(zDirDistance <= lenZ) {
			area += 9;
		}
		else {
			if((mActiveFaces & 16) != 16) return -1;
			area += 18;
		}
	} 
		
	return area;
}

bool CubeGravity::calcFaceGravity(const TVec3f &rPosition, s32 area, TVec3f *pDest, f32 *pScalar) const {
	TVec3f antiFaceDir; // Negative of the normal vector of the face an object is on
	switch(area) {
		case 4:
			mPosition.getZDir(antiFaceDir);
			break;
		case 10:
			mPosition.getYDir(antiFaceDir);
			break;
		case 12:
			mPosition.getXDir(antiFaceDir);
			break;
		case 14:
			mPosition.getXDir(antiFaceDir);
			JGeometry::negateInternal(&antiFaceDir.x, &antiFaceDir.x);
			break;
		case 16:
			mPosition.getYDir(antiFaceDir);
			JGeometry::negateInternal(&antiFaceDir.x, &antiFaceDir.x);
			break;
		case 22:
			mPosition.getZDir(antiFaceDir);
			JGeometry::negateInternal(&antiFaceDir.x, &antiFaceDir.x);
			break;
		default:
			return false;
		
	}
	TVec3f trans;
	f32 distance; // Double-check what this really is
	mPosition.getTrans(trans);
	MR::separateScalarAndDirection(&distance, &antiFaceDir, antiFaceDir);
	float gravityMagnitude = antiFaceDir.dot(trans - rPosition) / distance;
	if(gravityMagnitude < 0.f) gravityMagnitude = 0.f;
	*pDest = antiFaceDir;
	*pScalar = gravityMagnitude;
	return true;
}

void helperFunc1(const TVec3f& a, TVec3f& b, const TVec3f& c) {
	JMAVECScaleAdd(a.toCVec(), c.toCVec(), b.toVec(), -a.dot(c));
}


/*TVec3f translate(const TVec3f& a, const TVec3f& b) {
	TVec3f tmp = a;
	tmp += b;
	return tmp;
}*/

TVec3f negate(const TVec3f& in) {
       TVec3f tmp;
       JGeometry::negateInternal(&in.x, &tmp.x);
       return tmp;
}

bool CubeGravity::calcEdgeGravity(const TVec3f &rPosition, s32 area, TVec3f *pDest, f32 *pScalar) const {
	if(!(((area & 1) ^ ((area & 0x80000000) >> 31)) - ((area & 0x80000000) >> 31)) || area == 13) return false;
	TVec3f stack_140, stack_134, xDir, yDir, zDir, trans, stack_f8;
	mPosition.getXDir(xDir);
	mPosition.getYDir(yDir);
	mPosition.getZDir(zDir);
	switch(area) {
		case 1:
			stack_140 = xDir;
			stack_134 = negate(yDir) - zDir;
			break;
		case 3:
			stack_140 = yDir;
			stack_134 = negate(xDir) - zDir;
			break;
		case 5:
			stack_140 = yDir;
			stack_134 = xDir - zDir;
			break;
		case 7:
			stack_140 = xDir;
			stack_134 = yDir - zDir;
			break;
		case 9:
			stack_140 = zDir;
			stack_134 = negate(xDir) - yDir;
			break;
		case 11:
			stack_140 = zDir;
			stack_134 = xDir + yDir;
			break;
		case 15:
			stack_140 = zDir;
			stack_134 = negate(xDir).translate(yDir);
			break;
		case 17:
			stack_140 = zDir;
			stack_134 = xDir.translate(yDir);
			break;
		case 19:
			stack_140 = xDir;
			stack_134 = negate(yDir).translate(zDir);
			break;
		case 21:
			stack_140 = yDir;
			stack_134 = negate(xDir).translate(zDir);
			break;
		case 23:
			stack_140 = yDir;
			stack_134 = xDir.translate(zDir);
			break;
		case 25:
			stack_140 = xDir;
			stack_134 = yDir.translate(zDir);
			break;
		default:
			return false;
	}
	mPosition.getTrans(trans);
	stack_134 += trans;
	MR::normalizeOrZero(&stack_140);
	helperFunc1(stack_140, stack_f8, stack_134 - rPosition);
	if(stack_f8.isZero()) {
		pDest -> normalize(stack_134 - trans);
		*pScalar = 0.0;
	}
	else {
		*pScalar = pDest -> normalize(stack_f8);
	}
	return true;
	
}

bool CubeGravity::calcCornerGravity(const TVec3f &rPosition, s32 area, TVec3f *pDest, f32 *pScalar) const {
	TVec3f stack_140, xDir, yDir, zDir, trans;
	mPosition.getXDir(xDir);
	mPosition.getYDir(yDir);
	mPosition.getZDir(zDir);
	switch(area) {
		case 0:
			stack_140 = negate(xDir) - yDir - zDir;
			break;
		case 2:
			stack_140 = xDir - yDir - zDir;
			break;
		case 6:
			stack_140 = negate(xDir).translate(yDir) - zDir;
			break;
		case 8:
			stack_140 = xDir.translate(yDir) - zDir;
			break;
		case 18:
			stack_140 = (negate(xDir) - yDir).translate(zDir);
			break;
		case 20:
			stack_140 = (xDir - yDir).translate(zDir);
			break;
		case 24:
			stack_140 = negate(xDir).translate(yDir).translate(zDir);
			break;
		case 26:
			stack_140 = xDir.translate(yDir).translate(zDir);
			break;
		default:
			return false;
	}
	mPosition.getTrans(trans);
	stack_140 += trans;
	TVec3f stack_104 = stack_140 - rPosition;
	if(stack_104.isZero()) {
		*pScalar = 0.0;
		pDest -> normalize(stack_140 - trans);
	}
	else {
		*pScalar = pDest -> normalize(stack_104);
	}
	return true;
}
