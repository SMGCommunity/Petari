#pragma once

#include "Game/Animation/AnmPlayer.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "JSystem/J3DGraphBase/J3DStruct.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "revolution/mtx.h"
#include <revolution.h>

class ActorLightCtrl;
class BckCtrlData;
class BrkCtrl;
class BtkCtrl;
class CollisionParts;
class CollisionPartsFilterBase;
class Flag;
class HitSensor;
class LiveActor;
class LiveActorGroup;
class LodCtrl;
class ModelObj;
class MirrorActor;
class MsgSharedGroup;
class Nerve;
class PartsModel;
class ProjmapEffectMtxSetter;
class ResTIMG;
class ResourceHolder;
class TexMtxCtrl;
class TriangleFilterBase;
class XjointTransform;

namespace MR {
    enum CollisionScaleType { AutoEqualScale = 0, NoScale = 1, UNKNOWN_2 = 2, UNKNOWN_3 = 3 };

    bool isExistIndirectTexture(const LiveActor*);
    void initMirrorReflection(LiveActor*);

    CollisionParts* tryCreateCollisionMoveLimit(LiveActor*, HitSensor*);
    CollisionParts* tryCreateCollisionWaterSurface(LiveActor*, HitSensor*);
    CollisionParts* tryCreateCollisionSunshade(LiveActor*, HitSensor*);
    const char* createLowModelObjName(const LiveActor*);
    const char* createMiddleModelObjName(const LiveActor*);
    PartsModel* createBloomModel(LiveActor*, MtxPtr);
    PartsModel* createWaterModel(LiveActor*, MtxPtr);
    PartsModel* createIndirectPlanetModel(LiveActor*, MtxPtr);
    MirrorActor* tryCreateMirrorActor(LiveActor*, const char*);

    void copyTransRotateScale(const LiveActor*, LiveActor*);

    bool isOnGround(const LiveActor*);
    bool isOnGroundCos(const LiveActor*, f32);
    bool isBindedGround(const LiveActor*);
    bool isBindedGround(const LiveActor*, HitSensor*);
    bool isBindedWall(const LiveActor*);
    bool isBindedWall(const LiveActor*, HitSensor*);
    bool isBindedWallOfMap(const LiveActor*);
    bool isBindedWallOfMoveLimit(const LiveActor*);
    bool isBindedWallOrSlopeGround(const LiveActor*, f32, TVec3f*);
    bool isBindedRoof(const LiveActor*);
    bool isBindedRoof(const LiveActor*, HitSensor*);
    bool isBinded(const LiveActor*);
    bool isBinded(const LiveActor*, HitSensor*);
    bool isPressedRoofAndGround(const LiveActor*);
    bool isPressedMovingWall(const LiveActor*);

    void initDefaultPos(LiveActor*, const JMapInfoIter&);
    void initDefaultPosNoRepeat(LiveActor*, const JMapInfoIter&);

    bool isValidMovement(const LiveActor*);
    bool isValidCalcAnim(const LiveActor*);
    bool isValidCalcViewAndEntry(const LiveActor*);
    bool isValidDraw(const LiveActor*);

    void calcAnimDirect(LiveActor*);

    void setClippingTypeSphere(LiveActor*, f32);
    void setClippingTypeSphere(LiveActor*, f32, const TVec3f*);
    void setClippingTypeSphereContainsModelBoundingBox(LiveActor*, f32);

    void setClippingFar50m(LiveActor*);
    void setClippingFar100m(LiveActor*);
    void setClippingFar200m(LiveActor*);
    void setClippingFar300m(LiveActor*);
    void setClippingFarMax(LiveActor*);
    void setClippingFar(LiveActor*, f32);

    void setGroupClipping(LiveActor*, const JMapInfoIter&, int);
    void validateClipping(LiveActor*);
    void invalidateClipping(LiveActor*);

    bool changeShowModelFlagSyncNearClipping(LiveActor*, f32);

    bool isClipped(const LiveActor*);
    bool isInvalidClipping(const LiveActor*);

    void setBaseTRMtx(LiveActor*, MtxPtr);
    void setBaseTRMtx(LiveActor*, const TPos3f&);
    void setBaseTRMtx(LiveActor*, const TQuat4f&);
    void setBaseScale(LiveActor*, const TVec3f&);

    ResourceHolder* getResourceHolder(const LiveActor*);
    ResourceHolder* getModelResourceHolder(const LiveActor*);
    ResTIMG* getTexFromModel(const char*, const LiveActor*);
    ResTIMG* getTexFromArc(const char*, const LiveActor*);
    const char* getModelResName(const LiveActor*);

    bool isExistAnim(const LiveActor*, const char*);
    bool isExistBck(const LiveActor*, const char*);
    bool isExistBtk(const LiveActor*, const char*);
    bool isExistBrk(const LiveActor*, const char*);
    bool isExistBtp(const LiveActor*, const char*);
    bool isExistBpk(const LiveActor*, const char*);
    bool isExistBva(const LiveActor*, const char*);
    bool isExistTexture(const LiveActor*, const char*);

    void newDifferedDLBuffer(LiveActor*);
    void initDLMakerFog(LiveActor*, bool);
    void initDLMakerMatColor0(LiveActor*, const char*, const J3DGXColor*);
    void initDLMakerChangeTex(LiveActor*, const char*);
    TexMtxCtrl* initDLMakerTexMtx(LiveActor*, const char*);

    void startAction(const LiveActor*, const char*);
    bool isActionEnd(const LiveActor*);
    bool isActionStart(const LiveActor*, const char*);
    bool tryStartAction(const LiveActor*, const char*);

    void startAllAnim(const LiveActor*, const char*);
    bool tryStartAllAnim(const LiveActor*, const char*);

    ProjmapEffectMtxSetter* initDLMakerProjmapEffectMtxSetter(LiveActor*);

    void startBck(const LiveActor*, const char*, const char*);
    void startBckWithInterpole(const LiveActor*, const char*, s32);
    void startBckNoInterpole(const LiveActor*, const char*);
    void startBckAtFirstStep(const LiveActor*, const char*);
    bool tryStartBck(const LiveActor*, const char*, const char*);
    bool tryStartBckAndBtp(const LiveActor*, const char*, const char*);
    void setAllAnimFrame(const LiveActor*, const char*, f32);
    void setAllAnimFrameAndStop(const LiveActor*, const char*, f32);
    void setAllAnimFrameAtEnd(const LiveActor*, const char*);
    bool isAnyAnimStopped(const LiveActor*, const char*);
    bool isAnyAnimOneTimeAndStopped(const LiveActor*, const char*);

    bool isBckStopped(const LiveActor*);
    bool isBtkStopped(const LiveActor*);
    bool isBrkStopped(const LiveActor*);
    bool isBtpStopped(const LiveActor*);
    bool isBpkStopped(const LiveActor*);
    bool isBvaStopped(const LiveActor*);
    bool isBckOneTimeAndStopped(const LiveActor*);
    bool isBrkOneTimeAndStopped(const LiveActor*);
    bool isBckLooped(const LiveActor*);
    bool checkPassBckFrame(const LiveActor*, f32);

    void setBckFrameAtRandom(const LiveActor*);
    void setBtkFrameAtRandom(const LiveActor*);
    void setBckFrameAndStop(const LiveActor*, f32);
    void setBtkFrameAndStop(const LiveActor*, f32);
    void setBrkFrameAndStop(const LiveActor*, f32);
    void setBtpFrameAndStop(const LiveActor*, f32);
    void setBpkFrameAndStop(const LiveActor*, f32);
    void setBvaFrameAndStop(const LiveActor*, f32);
    void setBrkFrameEndAndStop(const LiveActor*);
    void startBtkAndSetFrameAndStop(const LiveActor*, const char*, f32);
    void startBrkAndSetFrameAndStop(const LiveActor*, const char*, f32);
    void startBtpAndSetFrameAndStop(const LiveActor*, const char*, f32);

    void startBtk(const LiveActor*, const char*);
    void startBrk(const LiveActor*, const char*);
    void startBtp(const LiveActor*, const char*);
    void startBpk(const LiveActor*, const char*);
    void startBva(const LiveActor*, const char*);

    bool startBckIfExist(const LiveActor*, const char*);
    bool startBtkIfExist(const LiveActor*, const char*);
    bool startBrkIfExist(const LiveActor*, const char*);
    bool startBtpIfExist(const LiveActor*, const char*);
    bool startBpkIfExist(const LiveActor*, const char*);
    bool startBvaIfExist(const LiveActor*, const char*);

    bool isBtkPlaying(const LiveActor*, const char*);
    bool isBrkPlaying(const LiveActor*, const char*);
    bool isBtpPlaying(const LiveActor*, const char*);
    bool isBpkPlaying(const LiveActor*, const char*);
    bool isBvaPlaying(const LiveActor*, const char*);
    bool isBckExist(const LiveActor*, const char*);
    bool isBtkExist(const LiveActor*, const char*);
    bool isBrkExist(const LiveActor*, const char*);
    bool isBpkExist(const LiveActor*, const char*);
    bool isBtpExist(const LiveActor*, const char*);
    bool isBvaExist(const LiveActor*, const char*);

    void stopBck(const LiveActor*);
    void stopBtk(const LiveActor*);
    void stopBrk(const LiveActor*);
    void stopBtp(const LiveActor*);
    void stopBva(const LiveActor*);

    void setBckRate(const LiveActor*, f32);
    void setBtkRate(const LiveActor*, f32);
    void setBrkRate(const LiveActor*, f32);
    void setBvaRate(const LiveActor*, f32);

    void setBckFrame(const LiveActor*, f32) NO_INLINE;
    void setBtkFrame(const LiveActor*, f32);
    void setBrkFrame(const LiveActor*, f32);
    void setBtpFrame(const LiveActor*, f32);
    void setBpkFrame(const LiveActor*, f32);
    void setBvaFrame(const LiveActor*, f32);

    bool isBckPlaying(const LiveActor*, const char*);

    J3DFrameCtrl* getBckCtrl(const LiveActor*);
    J3DFrameCtrl* getBtkCtrl(const LiveActor*);
    J3DFrameCtrl* getBrkCtrl(const LiveActor*);
    J3DFrameCtrl* getBtpCtrl(const LiveActor*);
    J3DFrameCtrl* getBpkCtrl(const LiveActor*);
    J3DFrameCtrl* getBvaCtrl(const LiveActor*);

    void updateMaterial(LiveActor*);
    void setMirrorReflectionInfoFromMtxYUp(const TPos3f&);
    void setMirrorReflectionInfoFromModel(LiveActor*);
    void changeModelDataTexAll(LiveActor*, const char*, const ResTIMG&);

    void initJointTransform(const LiveActor*);
    XjointTransform* getJointTransform(const LiveActor*, const char*);
    void setJointTransformLocalMtx(const LiveActor*, const char*, MtxPtr);

    f32 getBckFrame(const LiveActor*);
    f32 getBrkFrame(const LiveActor*);
    f32 getBtpFrame(const LiveActor*);
    f32 getBvaFrame(const LiveActor*);
    f32 getBckRate(const LiveActor*);
    f32 getBckFrameMax(const LiveActor*);
    f32 getBtkFrameMax(const LiveActor*);
    f32 getBrkFrameMax(const LiveActor*);

    const char* getPlayingBckName(const LiveActor*);

    void reflectBckCtrlData(LiveActor*, const BckCtrlData&);

    void initLightCtrl(LiveActor*);
    void initLightCtrlForPlayer(LiveActor*);
    void initLightCtrlNoDrawEnemy(LiveActor*);
    void initLightCtrlNoDrawMapObj(LiveActor*);
    void updateLightCtrl(LiveActor*);
    void updateLightCtrlDirect(LiveActor*);
    void loadActorLight(const LiveActor*);
    void calcLightPos0(TVec3f*, const LiveActor*);
    void calcLightPos1(TVec3f*, const LiveActor*);
    const GXColor* getLightAmbientColor(const LiveActor*);
    ActorLightCtrl* getLightCtrl(const LiveActor*);

    bool isStep(const LiveActor*, s32) NO_INLINE;
    bool isFirstStep(const LiveActor*);
    bool isLessStep(const LiveActor*, s32);
    bool isLessEqualStep(const LiveActor*, s32);
    bool isGreaterStep(const LiveActor*, s32);
    bool isGreaterEqualStep(const LiveActor*, s32);
    bool isIntervalStep(const LiveActor*, s32);
    bool isNewNerve(const LiveActor*);

    f32 calcNerveRate(const LiveActor*, s32);
    f32 calcNerveRate(const LiveActor*, s32, s32);
    f32 calcNerveEaseInRate(const LiveActor*, s32);
    f32 calcNerveEaseOutRate(const LiveActor*, s32);
    f32 calcNerveEaseOutRate(const LiveActor*, s32, s32);
    f32 calcNerveEaseInOutRate(const LiveActor*, s32);
    f32 calcNerveEaseInOutRate(const LiveActor*, s32, s32);
    f32 calcNerveValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveValue(const LiveActor*, s32, s32, f32, f32);
    f32 calcNerveEaseInValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveEaseInValue(const LiveActor*, s32, s32, f32, f32);
    f32 calcNerveEaseOutValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const LiveActor*, s32, f32, f32);
    f32 calcNerveEaseInOutValue(const LiveActor*, s32, s32, f32, f32);

    void setNerveAtStep(LiveActor*, const Nerve*, s32);
    void setNerveAtBckStopped(LiveActor*, const Nerve*);
    bool trySetNerve(LiveActor*, const Nerve*);

    const TVec3f* getGroundNormal(const LiveActor*);
    const TVec3f* getWallNormal(const LiveActor*);
    const TVec3f* getRoofNormal(const LiveActor*);
    const TVec3f* getBindedNormal(const LiveActor*);
    const TVec3f* getBindedHitPos(const LiveActor*);
    const TVec3f* getGroundHitPos(const LiveActor*);
    const TVec3f* getWallHitPos(const LiveActor*);
    const TVec3f* getRoofHitPos(const LiveActor*);

    void calcWallNormalHorizontal(TVec3f*, const LiveActor*);
    f32 calcHitPowerToGround(const LiveActor*);
    f32 calcHitPowerToWall(const LiveActor*);
    int getBindedPlaneNum(const LiveActor*);
    const TVec3f* getBindedPlaneNormal(const LiveActor*, int);
    HitSensor* getBindedPlaneSensor(const LiveActor*, int);
    TVec3f* getBindedFixReactionVector(const LiveActor*);

    void setBinderOffsetVec(LiveActor*, const TVec3f*, bool);
    void setBinderRadius(LiveActor*, f32);
    f32 getBinderRadius(const LiveActor*);
    void setBinderIgnoreMovingCollision(LiveActor*);
    void validateExCollisionParts(LiveActor*);
    void invalidateExCollisionParts(LiveActor*);
    void setBinderCollisionPartsFilter(LiveActor*, CollisionPartsFilterBase*);
    void setBinderExceptActor(LiveActor*, const LiveActor*);
    void setBinderExceptSensorType(LiveActor*, const TVec3f*, f32);
    void setBindTriangleFilter(LiveActor*, TriangleFilterBase*);
    bool isExistBinder(const LiveActor*);

    void onEntryDrawBuffer(LiveActor*);
    void offEntryDrawBuffer(LiveActor*);

    bool isDead(const LiveActor*);
    bool isHiddenModel(const LiveActor*);
    void showModel(LiveActor*);
    void hideModel(LiveActor*);
    void hideModelAndOnCalcAnim(LiveActor*);
    void showModelIfHidden(LiveActor*) NO_INLINE;
    void hideModelIfShown(LiveActor*);
    void hideModelAndOnCalcAnimIfShown(LiveActor*);

    void stopAnimFrame(LiveActor*);
    void releaseAnimFrame(LiveActor*);
    bool isNoCalcAnim(const LiveActor*);
    void onCalcAnim(LiveActor*);
    void offCalcAnim(LiveActor*);

    bool isNoCalcView(const LiveActor*);
    bool isNoEntryDrawBuffer(const LiveActor*);
    bool isNoBind(const LiveActor*);

    void onBind(LiveActor*);
    void offBind(LiveActor*);

    bool isCalcGravity(const LiveActor*);
    void onCalcGravity(LiveActor*);
    void offCalcGravity(LiveActor*);

    LiveActorGroup* joinToGroup(LiveActor*, const char*);
    MsgSharedGroup* joinToGroupArray(LiveActor*, const JMapInfoIter&, const char*, s32);
    LiveActorGroup* getGroupFromArray(const LiveActor*);
    LiveActor* getPairedGroupMember(const LiveActor*);

    void callMakeActorDeadAllGroupMember(const LiveActor*);
    void callKillAllGroupMember(const LiveActor*);
    void callMakeActorAppearedAllGroupMember(const LiveActor*);
    void callAppearAllGroupMember(const LiveActor*);
    void callRequestMovementOnAllGroupMember(const LiveActor*);
    void callInvalidateClippingAllGroupMember(const LiveActor*);
    void callValidateClippingAllGroupMember(const LiveActor*);

    s32 countHideGroupMember(const LiveActor*);
    s32 countShowGroupMember(const LiveActor*);

    void addToAttributeGroupSearchTurtle(const LiveActor*);
    void addToAttributeGroupReflectSpinningBox(const LiveActor*);
    bool isExistInAttributeGroupSearchTurtle(const LiveActor*);
    bool isExistInAttributeGroupReflectSpinningBox(const LiveActor*);

    void calcGravity(LiveActor*);
    void calcGravity(LiveActor*, const TVec3f&);
    void calcGravityOrZero(LiveActor*);
    void calcGravityOrZero(LiveActor*, const TVec3f&);

    void initFur(LiveActor*);
    void initFurPlanet(LiveActor*);
    LiveActor* initFurPlayer(LiveActor*);

    void initCollisionParts(LiveActor*, const char*, HitSensor*, MtxPtr);
    void initCollisionPartsAutoEqualScale(LiveActor*, const char*, HitSensor*, MtxPtr);
    void initCollisionPartsAutoEqualScaleOne(LiveActor*, const char*, HitSensor*, MtxPtr);
    void initCollisionPartsFromResourceHolder(LiveActor*, const char*, HitSensor*, ResourceHolder*, MtxPtr);

    CollisionParts* createCollisionPartsFromLiveActor(LiveActor*, const char*, HitSensor*, MR::CollisionScaleType);
    CollisionParts* createCollisionPartsFromLiveActor(LiveActor*, const char*, HitSensor*, MtxPtr, MR::CollisionScaleType);
    CollisionParts* createCollisionPartsFromResourceHolder(ResourceHolder*, const char*, HitSensor*, const TPos3f&, MR::CollisionScaleType);
    CollisionParts* tryCreateCollisionMoveLimit(LiveActor*, MtxPtr, HitSensor*);
    CollisionParts* tryCreateCollisionWaterSurface(LiveActor*, MtxPtr, HitSensor*);
    CollisionParts* tryCreateCollisionSunshade(LiveActor*, MtxPtr, HitSensor*);
    CollisionParts* tryCreateCollisionAllOtherCategory(LiveActor*, HitSensor*, CollisionParts**, CollisionParts**, CollisionParts**);
    CollisionParts* tryCreateCollisionAllOtherCategory(LiveActor*, MtxPtr, HitSensor*, CollisionParts**, CollisionParts**, CollisionParts**);

    bool isExistKcl(LiveActor*, const char*);
    f32 getCollisionBoundingSphereRange(const LiveActor*);
    bool isValidCollisionParts(LiveActor*);
    void validateCollisionParts(LiveActor*);
    void validateCollisionParts(CollisionParts*);
    void invalidateCollisionParts(LiveActor*);
    void invalidateCollisionParts(CollisionParts*);
    void onUpdateCollisionParts(LiveActor*);
    void onUpdateCollisionPartsOnetimeImmediately(LiveActor*);
    void offUpdateCollisionParts(LiveActor*);
    void resetAllCollisionMtx(LiveActor*);
    void setCollisionMtx(LiveActor*);
    void setCollisionMtx(LiveActor*, CollisionParts*);
    CollisionParts* getCollisionParts(const LiveActor*);
    bool isExistCollisionParts(const LiveActor*);
    u32 getCollisionSensorType(const CollisionParts*);

    ModelObj* createModelObjMapObj(const char*, const char*, MtxPtr);
    ModelObj* createModelObjMapObjStrongLight(const char*, const char*, MtxPtr);
    ModelObj* createModelObjNoSilhouettedMapObj(const char*, const char*, MtxPtr);
    ModelObj* createModelObjNoSilhouettedMapObjStrongLight(const char*, const char*, MtxPtr);
    ModelObj* createModelObjIndirectMapObj(const char*, const char*, MtxPtr);
    ModelObj* createModelObjPlayerDecoration(const char*, const char*, MtxPtr);
    ModelObj* createModelObjEnemy(const char*, const char*, MtxPtr);
    ModelObj* createModelObjNpc(const char*, const char*, MtxPtr);
    ModelObj* createModelObjPlanetLow(const char*, const char*, MtxPtr);
    ModelObj* createModelObjBloomModel(const char*, const char*, MtxPtr);

    PartsModel* createPartsModelMapObj(LiveActor*, const char*, const char*, MtxPtr);
    PartsModel* createPartsModelMapObjStrongLight(LiveActor*, const char*, const char*, MtxPtr);
    PartsModel* createPartsModelNoSilhouettedMapObj(LiveActor*, const char*, const char*, MtxPtr);
    PartsModel* createPartsModelEnemy(LiveActor*, const char*, const char*, MtxPtr);
    PartsModel* createPartsModelNpc(LiveActor*, const char*, const char*, MtxPtr);
    PartsModel* createPartsModelIndirectNpc(LiveActor*, const char*, const char*, MtxPtr);
    PartsModel* createPartsModelEnemyAndFix(LiveActor*, const char*, const char*, MtxPtr, const TVec3f&, const TVec3f&, const char*);
    PartsModel* createPartsModelNpcAndFix(LiveActor*, const char*, const char*, const char*);

    LodCtrl* createLodCtrlNPC(LiveActor*, const JMapInfoIter&);
    LodCtrl* createLodCtrlPlanet(LiveActor*, const JMapInfoIter&, f32, s32);
    LodCtrl* createLodCtrlMapObj(LiveActor*, const JMapInfoIter&, f32);

    Flag* createMapFlag(const char*, const char*, const TVec3f*, const TVec3f&, f32, f32, f32, s32, s32, f32);

    void stopSceneAtStep(const LiveActor*, s32, s32);

    void tryRumblePadAndCameraDistanceVeryStrong(const LiveActor*, f32, f32, f32);
    void tryRumblePadAndCameraDistanceStrong(const LiveActor*, f32, f32, f32);
    void tryRumblePadAndCameraDistanceMiddle(const LiveActor*, f32, f32, f32);
};  // namespace MR
