#include "Game/Util/JMapLinkInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    static const char* sFollowInfoTagName[3] = {"MapParts_ID", "Obj_ID", "ChildObjId"};
    static const char* sInfoNameTable[3] = {"mappartsinfo", "objinfo", "childobjinfo"};
};  // namespace

JMapLinkInfo::JMapLinkInfo(const JMapInfoIter& rIter, bool a2) {
    _0 = -1;
    _4 = -1;
    _8 = -1;

    if (a2) {
        setLinkedInfo(rIter);
    } else {
        setLinkInfo(rIter);
    }
}

bool JMapLinkInfo::isValid() const {
    bool ret = false;
    if (_0 >= 0 && _4 >= 0) {
        if (_8 >= 0) {
            ret = true;
        }
    }

    return ret;
}

void JMapLinkInfo::invalidate() {
    _0 = -1;
    _4 = -1;
    _8 = -1;
}

// https://decomp.me/scratch/ELpNv
void JMapLinkInfo::setLinkedInfo(const JMapInfoIter& rIter) {
    invalidate();

    if (rIter.isValid()) {
        s32 id = -1;
        if (rIter.getValue< s32 >("l_id", &id)) {
            _0 = id;
            _4 = MR::getPlacedZoneId(rIter);

            const char* name = rIter.mInfo->getName();

            s32 idx;
            for (idx = 0; idx < 3; idx++) {
                if (MR::isEqualString(name, sInfoNameTable[idx])) {
                    break;
                }
            }

            _8 = (idx < 3) ? idx : -1;
        }
    }
}

void JMapLinkInfo::setLinkInfo(const JMapInfoIter& rIter) {
    invalidate();

    if (rIter.isValid()) {
        _4 = MR::getPlacedZoneId(rIter);

        s32 v4 = -1;
        s32 v5 = -1;
        bool v6 = false;

        for (s32 i = 0; i < 3; i++) {
            s32 val = -1;
            rIter.getValue< s32 >(::sFollowInfoTagName[i], &val);

            if (val >= 0 && !v6) {
                v4 = val;
                v5 = i;
                v6 = true;
            }
        }

        if (v6) {
            _0 = v4;
            _8 = v5;
            _4 = MR::getPlacedZoneId(rIter);
        }
    }
}
