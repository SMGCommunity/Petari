#include "Game/Boss/PoltaSensorCtrl.hpp"
#include "Game/Boss/Polta.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/types.h"
#include <JSystem/JKernel/JKRHeap.hpp>

namespace {
    const PoltaSensorListEntry sCoreSensorList[1] = {
        "Core",
        "Tongue3",
        150.0f,
        0.0f,
        0.0f,
        0.0f,
    };

    const PoltaSensorListEntry sBodySensorList[3] = {
        {
            "Body1",
            "Head1",
            320.0f,
            100.0f,
            -20.0f,
            0.0f,
        },
        {
            "Body2",
            "Head2",
            260.0f,
            180.0f,
            0.0f,
            0.0f,
        },
        {
            "Body3",
            "Head3",
            290.0f,
            320.0f,
            0.0f,
            0.0f,
        }
    };

    const PoltaSensorListEntry sLeftArmSensorList[11] = {
        {
            "ArmL1",
            "ArmL1",
            140.0f,
            90.0f,
            0.0f,
            0.0f,
        },
        {
            "ArmL2",
            "ArmL2",
            140.0f,
            120.0f,
            0.0f,
            0.0f,
        },
        {
            "HandL",
            "HandL",
            240.0f,
            220.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLA1",
            "FingerLA1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLA2",
            "FingerLA2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLB1",
            "FingerLB1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLB2",
            "FingerLB2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLC1",
            "FingerLC1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLC2",
            "FingerLC2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLD1",
            "FingerLD1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerLD2",
            "FingerLD2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
    };

    const PoltaSensorListEntry sRightArmSensorList[11] = {
        {
            "ArmR1",
            "ArmR1",
            140.0f,
            90.0f,
            0.0f,
            0.0f,
        },
        {
            "ArmR2",
            "ArmR2",
            140.0f,
            120.0f,
            0.0f,
            0.0f,
        },
        {
            "HandR",
            "HandR",
            240.0f,
            220.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRA1",
            "FingerRA1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRA2",
            "FingerRA2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRB1",
            "FingerRB1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRB2",
            "FingerRB2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRC1",
            "FingerRC1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRC2",
            "FingerRC2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRD1",
            "FingerRD1",
            140.0f,
            130.0f,
            0.0f,
            0.0f,
        },
        {
            "FingerRD2",
            "FingerRD2",
            120.0f,
            140.0f,
            0.0f,
            0.0f,
        },
    };
};

PoltaSensorCtrl::PoltaSensorCtrl(Polta* pPolta) : mPoltaPtr(pPolta),
    mScreamSensor(nullptr),
    mHeadSensor(nullptr),
    mCoreSensors(nullptr),
    mBodySensors(nullptr),
    mLeftArmSensors(nullptr),
    mRightArmSensors(nullptr)
{    
}

s32 PoltaSensorCtrl::getSensorCount() const {
    return 28;
}

void PoltaSensorCtrl::setScreamSensorSize(f32 screamSensorSize) {
    if (mScreamSensor) {
        mScreamSensor->mRadius = screamSensorSize;
    }
}

void PoltaSensorCtrl::initSensor() {
    mCoreSensors = new HitSensor*[1];
    
    mCoreSensors[0] = MR::addHitSensorAtJointEnemy(mPoltaPtr, sCoreSensorList[0].mName, 
        sCoreSensorList[0].mJointName, 8, sCoreSensorList[0].mRadius,  
        TVec3f(sCoreSensorList[0].mOffsetX,sCoreSensorList[0].mOffsetY, sCoreSensorList[0].mOffsetZ));
    
    mBodySensors = new HitSensor*[3];
    
    for (int i = 0; i< getBodySensorCount(); i++) {

        mBodySensors[i] = MR::addHitSensorAtJointEnemy(mPoltaPtr, sBodySensorList[i].mName, 
        sBodySensorList[i].mJointName, 8, sBodySensorList[i].mRadius,  
        TVec3f(sBodySensorList[i].mOffsetX,sBodySensorList[i].mOffsetY, sBodySensorList[i].mOffsetZ));
    }

    mLeftArmSensors = new HitSensor*[11];
    
    for (int i = 0; i< getArmSensorCount(); i++) {
        mLeftArmSensors[i] = MR::addHitSensorMtxEnemy(mPoltaPtr, sLeftArmSensorList[i].mName, 8, 
            sLeftArmSensorList[i].mRadius,  MR::getJointMtx(mPoltaPtr, sLeftArmSensorList[i].mJointName), 
            TVec3f(sLeftArmSensorList[i].mOffsetX,sLeftArmSensorList[i].mOffsetY, sLeftArmSensorList[i].mOffsetZ));
    }
    
    mRightArmSensors = new HitSensor*[11];
    
    for (int i = 0; i< getArmSensorCount(); i++) {

        mRightArmSensors[i] = MR::addHitSensorMtxEnemy(mPoltaPtr, sRightArmSensorList[i].mName, 8, 
            sRightArmSensorList[i].mRadius,  MR::getJointMtx(mPoltaPtr, sRightArmSensorList[i].mJointName), 
            TVec3f(sRightArmSensorList[i].mOffsetX,sRightArmSensorList[i].mOffsetY, sRightArmSensorList[i].mOffsetZ));
    }

    mScreamSensor = MR::addHitSensorAtJointEnemy(mPoltaPtr, "Scream", "Tongue3", 
        64, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    mHeadSensor = MR::addHitSensorAtJointEnemy(mPoltaPtr, "Head", "Head5", 
        32, 200.0f, TVec3f(-50.0f, 0.0f, 0.0f));
}

bool PoltaSensorCtrl::isCoreSensor(const HitSensor* pSensor) const {
    for(int i = 0; i < getCoreSensorCount(); i++) {
        if(mCoreSensors[i] == pSensor) {
            return true;
        }
    }
    return false;
}

bool PoltaSensorCtrl::isBodySensor(const HitSensor* pSensor) const {
    for(int i = 0; i < getBodySensorCount(); i++) {
        if(mBodySensors[i] == pSensor) {
            return true;
        }
    }
    return false;
}

bool PoltaSensorCtrl::isLeftArmSensor(const HitSensor* pSensor) const {
    for(int i = 0; i < getArmSensorCount(); i++) {
        if(mLeftArmSensors[i] == pSensor) {
            return true;
        }
    }
    return false;
}

bool PoltaSensorCtrl::isRightArmSensor(const HitSensor* pSensor) const {
    for(int i = 0; i < getArmSensorCount(); i++) {
        if(mRightArmSensors[i] == pSensor) {
            return true;
        }
    }
    return false;
}

bool PoltaSensorCtrl::isHeadSensor(const HitSensor* pSensor) const {
    return mHeadSensor == pSensor;
}

bool PoltaSensorCtrl::isScreamSensor(const HitSensor* pSensor) const {
    return mScreamSensor == pSensor;
}
