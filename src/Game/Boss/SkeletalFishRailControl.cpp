#include "Game/Boss/SkeletalFishRailControl.hpp"

SkeletalFishRailControl::SkeletalFishRailControl() {
    _0 = 2;
    _4 = nullptr;
    _8 = nullptr;
    _10 = 0.0f;
    _14 = 20.0f;
    _18 = false;
}

void SkeletalFishRailControl::setRailActor(LiveActor *a1, LiveActor *a2, bool a3) {
    _4 = a2;
    _8 = a1;

    if (a2 != nullptr) {
        _0 = 0;
        MR::setRailCoord(a2, 0.0f);
        MR::setRailDirectionToEnd(_4);
    }
    else {
        _0 = 2;
    }

    if (a3) {
        MR::setRailCoord(_8, 0.0f);
        MR::setRailDirectionToEnd(_8);
    }
}

void SkeletalFishRailControl::update() {
    if (_4 != nullptr) {
        MR::setRailCoordSpeed(_4, _14);
    }

    MR::setRailCoordSpeed(_8, _14);

    switch (_0) {
        case 0:
        {
            f32 railLength = MR::getRailTotalLength(_4);
            f32 v3 = ((_14 + MR::getRailCoord(_4) - railLength));
            if (v3 >= 0.0f) {
                f32 coord = MR::calcNearestRailCoord(_8, MR::getRailPointPosEnd(_4));
                _10 = coord;
                _C = (coord - v3);
                MR::setRailCoord(_8, _C);
                _0 = 1;
            }
            break;
        }
            
        case 1:
            if (_C - _10 >= MR::getRailTotalLength(_8)) {
                _0 = 2;
            }
            break;
        case 2:
            break;
    }

    _18 = false;

    switch (_0) {
        case 0:
            MR::moveRailRider(_4);
            break;
        case 1:
            _C += _14;
            MR::getRailTotalLength(_8);
            MR::setRailCoord(_8, _C);
            break;
        case 2:
                if (MR::getRailCoord(_8) + MR::getRailCoordSpeed(_8) > MR::getRailTotalLength(_8)) {
                    _18 = true;
                }

                MR::moveRailRider(_8);
            break;
    }
}

void SkeletalFishRailControl::getRailInfo(SkeletalFishRailInfo* pOutInfo, f32 a2) {
    switch (_0) {
        case 0:
        {
            f32 coord = MR::getRailCoord(_4);
            pOutInfo->_0 = _4;
            pOutInfo->_4 = coord - a2;
            break;
        }
        case 1:
        {
            f32 dist = _C - a2;
            if (dist < _10) {
                f32 railLength = MR::getRailTotalLength(_4);
                f32 diff = _10 - dist;
                pOutInfo->_0 = _4;
                dist = railLength - diff;
            }
            else {
                pOutInfo->_0 = _8;
            }

            pOutInfo->_4 = dist;
            break;
        }
        
        case 2:
        {
            f32 coord = MR::getRailCoord(_8);
            pOutInfo->_0 = _8;
            pOutInfo->_4 = coord - a2;
            break;
        }
    }
}

void SkeletalFishRailControl::getPos(TVec3f *pOut, f32 a2) {
    SkeletalFishRailInfo info;
    getRailInfo(&info, a2);

    if (info._4 < 0.0f && !MR::isLoopRail(info._0)) {
        MR::calcRailPosAtCoord(pOut, info._0, 0.0f);
        TVec3f railDir;
        MR::calcRailDirectionAtCoord(&railDir, info._0, 0.0f);
        railDir.scale(info._4);
        pOut->add(railDir);
        
    }
    else {
        MR::calcRailPosAtCoord(pOut, info._0, info._4);
    }
}

void SkeletalFishRailControl::getMtx(TPos3f *pOut, f32 a2) {
    SkeletalFishRailInfo normalRailInfo;
    SkeletalFishRailInfo distRailInfo;
    getRailInfo(&normalRailInfo, a2);
    getRailInfo(&distRailInfo, a2 - 1000.0f);

    TVec3f railPos;
    TVec3f railDir;
    TVec3f gravityVec;
    TVec3f stack_3C;
    TVec3f v15;
    TVec3f v14;
    TVec3f negVector;

    if (normalRailInfo._4 < 0.0f && !MR::isLoopRail(normalRailInfo._0)) {
        MR::calcRailPosAtCoord(&railPos, normalRailInfo._0, 0.0f);
        MR::calcRailDirectionAtCoord(&railDir, normalRailInfo._0, 0.0f);
        stack_3C.setPS(railDir);
        stack_3C.scale(normalRailInfo._4);
        railPos.add(stack_3C);
    }
    else {
        MR::calcRailPosAtCoord(&railPos, normalRailInfo._0, normalRailInfo._4);
        MR::calcRailDirectionAtCoord(&railDir, normalRailInfo._0, normalRailInfo._4);
    }

    MR::normalizeOrZero(&railDir);
    MR::calcGravityVector(normalRailInfo._0, railPos, &gravityVec, nullptr, 0);
    negVector = -gravityVec;
    PSVECCrossProduct(&negVector, &railDir, &v15);
    PSVECCrossProduct(&railDir, &v15, &v14);

    MR::normalizeOrZero(&v15);
    MR::normalizeOrZero(&v14);
    MR::normalizeOrZero(&railDir);

    pOut->mMtx[0][0] = v15.x;
    pOut->mMtx[1][0] = v15.y;
    pOut->mMtx[2][0] = v15.z;
    pOut->mMtx[0][1] = v14.x;
    pOut->mMtx[1][1] = v14.y;
    pOut->mMtx[2][1] = v14.z;
    pOut->mMtx[0][2] = railDir.x;
    pOut->mMtx[1][2] = railDir.y;
    pOut->mMtx[2][2] = railDir.z;
    pOut->mMtx[0][3] = railPos.x;
    pOut->mMtx[1][3] = railPos.y;
    pOut->mMtx[2][3] = railPos.z;
}
