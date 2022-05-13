#include "Game/Util/JMapUtil.h"
#include "Game/Util.h"

namespace {
    bool getJMapInfoRailArg(const JMapInfoIter &rIter, const char *pName, s32 *pOut) NO_INLINE {
        s32 val;
        bool hasValue = rIter.getValue<s32>(pName, &val);

        if (!hasValue) {
            return false;
        } 

        if (val != -1) {
            *pOut = val;
            return true;
        }

        return false;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &rIter, const char *pName, s32 *pOut) NO_INLINE {
        s32 val;
        bool hasValue = rIter.getValue<s32>(pName, &val);

        if (!hasValue) {
            return false;
        } 

        if (val != -1) {
            *pOut = val;
            return true;
        }

        return false;
    }

    bool getJMapInfoArgNoInit(const JMapInfoIter &rIter, const char *pName, f32 *pOut) NO_INLINE {
        s32 val;
        bool hasValue = ::getJMapInfoArgNoInit(rIter, pName, &val);

        if (!hasValue) {
            return false;
        }

        *pOut = val;
        return true;
    } 

    bool getJMapInfoArgNoInit(const JMapInfoIter &rIter, const char *pName, bool *pOut) NO_INLINE {
        s32 val;
        bool hasValue = rIter.getValue<s32>(pName, &val);

        if (!hasValue) {
            return false;
        } 

        if (val != -1) {
            *pOut = true;
        }
        else {
            *pOut = false;
        }

        return true;
    }
};

namespace MR {
    bool isEqualObjectName(const JMapInfoIter &rIter, const char *pOtherName) {
        const char* objName;
        getObjectName(&objName, rIter);
        return isEqualStringCase(objName, pOtherName);
    }

    s32 getDemoGroupID(const JMapInfoIter &rIter) {
        s32 groupID = -1;
        rIter.getValue<s32>("DemoGroupId", &groupID);
        return groupID;
    }

    s32 getDemoGroupLinkID(const JMapInfoIter &rIter) {
        s32 linkID = -1;
        rIter.getValue<s32>("l_id", &linkID);
        return linkID;
    }

    bool getJMapInfoRailArg0NoInit(const JMapInfoIter &rIter, s32 *pRailArg0) { 
        return ::getJMapInfoRailArg(rIter, "path_arg0", pRailArg0);
    }

    /*
    bool getRailId(const JMapInfoIter &rIter, s32 *pRailID) {
        *pRailID = -1;
        return ::getJMapInfoArgNoInit(rIter, "CommonPath_ID", pRailID); 
    } 
    */

   bool getObjectName(const char **pName, const JMapInfoIter &rIter) {
       if (!rIter.isValid()) {
           return false;
       }

       if (rIter.getValue<const char *>("type", pName)) {
           return true;
       }

       return rIter.getValue<const char*>("name", pName);
   }

   bool isExistJMapArg(const JMapInfoIter &rIter) {
       if (!rIter.isValid()) {
           return false;
       }

        s32 val;
       return rIter.getValue<s32>("Obj_arg0", &val);
   }

   bool getJMapInfoShapeIdWithInit(const JMapInfoIter &rIter, s32 *pShapeID) {
       return rIter.getValue<s32>("ShapeModelNo", pShapeID);
   }
};