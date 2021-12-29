# Gravity
| Object | Percentage (of Bytes) | Functions Done / Total Functions | Percentage (Functions) 
| ------------- | ------------- | ------------- | ------------- |
| ConeGravity.o | 10.14851485148515% | 3 / 6 | 50.0% |
| CubeGravity.o | 0.0% | 0 / 10 | 0.0% |
| DiskGravity.o | 0.0% | 0 / 11 | 0.0% |
| DiskTorusGravity.o | 0.0% | 0 / 9 | 0.0% |
| GlobalGravityObj.o | 100.0% | 18 / 18 | 100.0% |
| GraviryFollower.o | 100.0% | 4 / 4 | 100.0% |
| GravityCreator.o | 72.55965292841648% | 35 / 42 | 83.33333333333334% |
| GravityInfo.o | 100.0% | 2 / 2 | 100.0% |
| ParallelGravity.o | 0.0% | 0 / 13 | 0.0% |
| PlanetGravity.o | 100.0% | 9 / 9 | 100.0% |
| PlanetGravityManager.o | 21.73913043478261% | 3 / 5 | 60.0% |
| PointGravity.o | 100.0% | 3 / 3 | 100.0% |
| SegmentGravity.o | 0.0% | 0 / 8 | 0.0% |
| WireGravity.o | 33.5483870967742% | 3 / 4 | 75.0% |


# ConeGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11ConeGravityFv | :white_check_mark: |
| setLocalMatrix__11ConeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setEnableBottom__11ConeGravityFb | :white_check_mark: |
| setTopCutRate__11ConeGravityFf | :white_check_mark: |
| updateMtx__11ConeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| calcOwnGravityVector__11ConeGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |


# CubeGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| isZero__Q29JGeometry8TVec3<f>CFv | :x: |
| __ct__11CubeGravityFv | :x: |
| setCube__11CubeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| calcGravityArea__11CubeGravityCFRCQ29JGeometry8TVec3<f> | :x: |
| calcFaceGravity__11CubeGravityCFRCQ29JGeometry8TVec3<f>lPQ29JGeometry8TVec3<f>Pf | :x: |
| calcEdgeGravity__11CubeGravityCFRCQ29JGeometry8TVec3<f>lPQ29JGeometry8TVec3<f>Pf | :x: |
| calcCornerGravity__11CubeGravityCFRCQ29JGeometry8TVec3<f>lPQ29JGeometry8TVec3<f>Pf | :x: |
| calcOwnGravityVector__11CubeGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__11CubeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| normalize__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f> | :x: |


# DiskGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11DiskGravityFv | :x: |
| setLocalPosition__11DiskGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setLocalDirection__11DiskGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setSideDirection__11DiskGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setRadius__11DiskGravityFf | :x: |
| setValidDegee__11DiskGravityFf | :x: |
| setBothSide__11DiskGravityFb | :x: |
| setEnableEdgeGravity__11DiskGravityFb | :x: |
| calcOwnGravityVector__11DiskGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__11DiskGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| updateLocalParam__11DiskGravityFv | :x: |


# DiskTorusGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__16DiskTorusGravityFv | :x: |
| setPosition__16DiskTorusGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setDirection__16DiskTorusGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setRadius__16DiskTorusGravityFf | :x: |
| setDiskRadius__16DiskTorusGravityFf | :x: |
| setEdgeType__16DiskTorusGravityFl | :x: |
| setBothSide__16DiskTorusGravityFb | :x: |
| updateMtx__16DiskTorusGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| calcOwnGravityVector__16DiskTorusGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |


# GlobalGravityObj.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__16GlobalGravityObjFPCc | :white_check_mark: |
| init__16GlobalGravityObjFRC12JMapInfoIter | :white_check_mark: |
| makeActorAppeared__16GlobalGravityObjFv | :white_check_mark: |
| makeActorDead__16GlobalGravityObjFv | :white_check_mark: |
| updateSwitch__16GlobalGravityObjFv | :white_check_mark: |
| movement__16GlobalGravityObjFv | :white_check_mark: |
| getGravity__16GlobalGravityObjFv | :white_check_mark: |
| createGlobalCubeGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalConeGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalDiskGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalDiskTorusGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPlaneGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPlaneInBoxGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPlaneInCylinderGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalPointGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalSegmentGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalWireGravityObj__2MRFPCc | :white_check_mark: |
| __dt__16GlobalGravityObjFv | :white_check_mark: |


# GraviryFollower.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__15GraviryFollowerFP16GlobalGravityObjRC12JMapInfoIter | :white_check_mark: |
| setGravityFollowHost__15GraviryFollowerFPC7NameObj | :white_check_mark: |
| update__15GraviryFollowerFv | :white_check_mark: |
| addBaseMatrixFollowerGravity__2MRFP16GlobalGravityObjRC12JMapInfoIter | :white_check_mark: |


# GravityCreator.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__14GravityCreatorFv | :white_check_mark: |
| createFromJMap__14GravityCreatorFRC12JMapInfoIter | :white_check_mark: |
| createInstance__18CubeGravityCreatorFv | :white_check_mark: |
| settingFromSRT__18CubeGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__18CubeGravityCreatorFlll | :white_check_mark: |
| getGravity__18CubeGravityCreatorFv | :white_check_mark: |
| createInstance__18DiskGravityCreatorFv | :white_check_mark: |
| settingFromSRT__18DiskGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__18DiskGravityCreatorFlll | :white_check_mark: |
| getGravity__18DiskGravityCreatorFv | :white_check_mark: |
| createInstance__23DiskTorusGravityCreatorFv | :white_check_mark: |
| settingFromSRT__23DiskTorusGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__23DiskTorusGravityCreatorFlll | :white_check_mark: |
| getGravity__23DiskTorusGravityCreatorFv | :white_check_mark: |
| createInstance__18ConeGravityCreatorFv | :white_check_mark: |
| settingFromSRT__18ConeGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__18ConeGravityCreatorFlll | :white_check_mark: |
| getGravity__18ConeGravityCreatorFv | :white_check_mark: |
| createInstance__19PlaneGravityCreatorFv | :white_check_mark: |
| settingFromSRT__19PlaneGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| getGravity__19PlaneGravityCreatorFv | :white_check_mark: |
| createInstance__24PlaneInBoxGravityCreatorFv | :white_check_mark: |
| settingFromSRT__24PlaneInBoxGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| getGravity__24PlaneInBoxGravityCreatorFv | :white_check_mark: |
| settingFromJMapArgs__24PlaneInBoxGravityCreatorFlll | :white_check_mark: |
| createInstance__29PlaneInCylinderGravityCreatorFv | :white_check_mark: |
| getGravity__29PlaneInCylinderGravityCreatorFv | :white_check_mark: |
| settingFromSRT__29PlaneInCylinderGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__29PlaneInCylinderGravityCreatorFlll | :white_check_mark: |
| createInstance__19PointGravityCreatorFv | :white_check_mark: |
| settingFromSRT__19PointGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| getGravity__19PointGravityCreatorFv | :white_check_mark: |
| createInstance__21SegmentGravityCreatorFv | :white_check_mark: |
| settingFromSRT__21SegmentGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__21SegmentGravityCreatorFlll | :white_check_mark: |
| getGravity__21SegmentGravityCreatorFv | :white_check_mark: |
| createInstance__18WireGravityCreatorFv | :white_check_mark: |
| settingFromJMapOtherParam__18WireGravityCreatorFRC12JMapInfoIter | :white_check_mark: |
| getGravity__18WireGravityCreatorFv | :white_check_mark: |
| settingFromSRT__14GravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__14GravityCreatorFlll | :white_check_mark: |
| settingFromJMapOtherParam__14GravityCreatorFRC12JMapInfoIter | :white_check_mark: |


# GravityInfo.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11GravityInfoFv | :white_check_mark: |
| init__11GravityInfoFv | :white_check_mark: |


# ParallelGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__15ParallelGravityFv | :x: |
| calcOwnGravityVector__15ParallelGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__15ParallelGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setPlane__15ParallelGravityFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| setRangeBox__15ParallelGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setRangeCylinder__15ParallelGravityFff | :x: |
| setRangeType__15ParallelGravityFQ215ParallelGravity10RANGE_TYPE | :x: |
| setBaseDistance__15ParallelGravityFf | :x: |
| setDistanceCalcType__15ParallelGravityFQ215ParallelGravity18DISTANCE_CALC_TYPE | :x: |
| isInSphereRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :x: |
| isInBoxRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :x: |
| isInCylinderRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :x: |
| isInRange__15ParallelGravityCFRCQ29JGeometry8TVec3<f>Pf | :x: |


# PlanetGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__13PlanetGravityFv | :white_check_mark: |
| setPriority__13PlanetGravityFl | :white_check_mark: |
| calcGravity__13PlanetGravityCFPQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| calcOwnGravityVector__13PlanetGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| isInRangeSquare__13PlanetGravityCFf | :white_check_mark: |
| isInRangeDistance__13PlanetGravityCFf | :white_check_mark: |
| calcGravityFromMassPosition__13PlanetGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| updateMtx__13PlanetGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |
| updateIdentityMtx__13PlanetGravityFv | :white_check_mark: |


# PlanetGravityManager.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__20PlanetGravityManagerFPCc | :white_check_mark: |
| init__20PlanetGravityManagerFRC12JMapInfoIter | :white_check_mark: |
| calcTotalGravityVector__20PlanetGravityManagerCFPQ29JGeometry8TVec3<f>P11GravityInfoRCQ29JGeometry8TVec3<f>UlUl | :x: |
| registerGravity__20PlanetGravityManagerFP13PlanetGravity | :x: |
| __dt__20PlanetGravityManagerFv | :white_check_mark: |


# PointGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__12PointGravityFv | :white_check_mark: |
| calcOwnGravityVector__12PointGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| updateMtx__12PointGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |


# SegmentGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__14SegmentGravityFv | :x: |
| calcOwnGravityVector__14SegmentGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__14SegmentGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setGravityPoint__14SegmentGravityFUlRCQ29JGeometry8TVec3<f> | :x: |
| setSideVector__14SegmentGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setValidSideDegree__14SegmentGravityFf | :x: |
| setEdgeValid__14SegmentGravityFUlb | :x: |
| updateLocalParam__14SegmentGravityFv | :x: |


# WireGravity.o
| Symbol | Decompiled? |
| ------------- | ------------- |
| __ct__11WireGravityFv | :white_check_mark: |
| setPointListSize__11WireGravityFUl | :white_check_mark: |
| addPoint__11WireGravityFRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| calcOwnGravityVector__11WireGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |


