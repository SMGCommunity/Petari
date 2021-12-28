# Gravity
| Object | Percentage | Functions Done / Total Functions |
| ------------- | ------------- | ------------- |
| ConeGravity.o | 0.0% | 0 / 6 |
| CubeGravity.o | 0.0% | 0 / 10 |
| DiskGravity.o | 0.0% | 0 / 11 |
| DiskTorusGravity.o | 0.0% | 0 / 9 |
| GlobalGravityObj.o | 48.336594911937375% | 10 / 18 |
| GraviryFollower.o | 0.0% | 0 / 4 |
| GravityCreator.o | 22.125813449023862% | 11 / 42 |
| GravityInfo.o | 100.0% | 2 / 2 |
| ParallelGravity.o | 0.0% | 0 / 13 |
| PlanetGravity.o | 100.0% | 9 / 9 |
| PlanetGravityManager.o | 21.73913043478261% | 3 / 5 |
| PointGravity.o | 100.0% | 3 / 3 |
| SegmentGravity.o | 0.0% | 0 / 8 |


# ConeGravity.o
| Symbol | Decompiled? |
| __ct__11ConeGravityFv | :x: |
| setLocalMatrix__11ConeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setEnableBottom__11ConeGravityFb | :x: |
| setTopCutRate__11ConeGravityFf | :x: |
| updateMtx__11ConeGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| calcOwnGravityVector__11ConeGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |


# CubeGravity.o
| Symbol | Decompiled? |
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
| __ct__16GlobalGravityObjFPCc | :white_check_mark: |
| init__16GlobalGravityObjFRC12JMapInfoIter | :white_check_mark: |
| makeActorAppeared__16GlobalGravityObjFv | :white_check_mark: |
| makeActorDead__16GlobalGravityObjFv | :white_check_mark: |
| updateSwitch__16GlobalGravityObjFv | :white_check_mark: |
| movement__16GlobalGravityObjFv | :white_check_mark: |
| getGravity__16GlobalGravityObjFv | :white_check_mark: |
| createGlobalCubeGravityObj__2MRFPCc | :x: |
| createGlobalConeGravityObj__2MRFPCc | :x: |
| createGlobalDiskGravityObj__2MRFPCc | :x: |
| createGlobalDiskTorusGravityObj__2MRFPCc | :x: |
| createGlobalPlaneGravityObj__2MRFPCc | :x: |
| createGlobalPlaneInBoxGravityObj__2MRFPCc | :x: |
| createGlobalPlaneInCylinderGravityObj__2MRFPCc | :x: |
| createGlobalPointGravityObj__2MRFPCc | :white_check_mark: |
| createGlobalSegmentGravityObj__2MRFPCc | :x: |
| createGlobalWireGravityObj__2MRFPCc | :white_check_mark: |
| __dt__16GlobalGravityObjFv | :white_check_mark: |


# GraviryFollower.o
| Symbol | Decompiled? |
| __ct__15GraviryFollowerFP16GlobalGravityObjRC12JMapInfoIter | :x: |
| setGravityFollowHost__15GraviryFollowerFPC7NameObj | :x: |
| update__15GraviryFollowerFv | :x: |
| addBaseMatrixFollowerGravity__2MRFP16GlobalGravityObjRC12JMapInfoIter | :x: |


# GravityCreator.o
| Symbol | Decompiled? |
| __ct__14GravityCreatorFv | :white_check_mark: |
| createFromJMap__14GravityCreatorFRC12JMapInfoIter | :white_check_mark: |
| createInstance__18CubeGravityCreatorFv | :x: |
| settingFromSRT__18CubeGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__18CubeGravityCreatorFlll | :x: |
| getGravity__18CubeGravityCreatorFv | :x: |
| createInstance__18DiskGravityCreatorFv | :x: |
| settingFromSRT__18DiskGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__18DiskGravityCreatorFlll | :x: |
| getGravity__18DiskGravityCreatorFv | :x: |
| createInstance__23DiskTorusGravityCreatorFv | :x: |
| settingFromSRT__23DiskTorusGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__23DiskTorusGravityCreatorFlll | :x: |
| getGravity__23DiskTorusGravityCreatorFv | :x: |
| createInstance__18ConeGravityCreatorFv | :x: |
| settingFromSRT__18ConeGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__18ConeGravityCreatorFlll | :x: |
| getGravity__18ConeGravityCreatorFv | :x: |
| createInstance__19PlaneGravityCreatorFv | :x: |
| settingFromSRT__19PlaneGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| getGravity__19PlaneGravityCreatorFv | :x: |
| createInstance__24PlaneInBoxGravityCreatorFv | :x: |
| settingFromSRT__24PlaneInBoxGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| getGravity__24PlaneInBoxGravityCreatorFv | :x: |
| settingFromJMapArgs__24PlaneInBoxGravityCreatorFlll | :x: |
| createInstance__29PlaneInCylinderGravityCreatorFv | :x: |
| getGravity__29PlaneInCylinderGravityCreatorFv | :x: |
| settingFromSRT__29PlaneInCylinderGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__29PlaneInCylinderGravityCreatorFlll | :x: |
| createInstance__19PointGravityCreatorFv | :white_check_mark: |
| settingFromSRT__19PointGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| getGravity__19PointGravityCreatorFv | :white_check_mark: |
| createInstance__21SegmentGravityCreatorFv | :x: |
| settingFromSRT__21SegmentGravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :x: |
| settingFromJMapArgs__21SegmentGravityCreatorFlll | :x: |
| getGravity__21SegmentGravityCreatorFv | :x: |
| createInstance__18WireGravityCreatorFv | :white_check_mark: |
| settingFromJMapOtherParam__18WireGravityCreatorFRC12JMapInfoIter | :white_check_mark: |
| getGravity__18WireGravityCreatorFv | :white_check_mark: |
| settingFromSRT__14GravityCreatorFRCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f> | :white_check_mark: |
| settingFromJMapArgs__14GravityCreatorFlll | :white_check_mark: |
| settingFromJMapOtherParam__14GravityCreatorFRC12JMapInfoIter | :white_check_mark: |


# GravityInfo.o
| Symbol | Decompiled? |
| __ct__11GravityInfoFv | :white_check_mark: |
| init__11GravityInfoFv | :white_check_mark: |


# ParallelGravity.o
| Symbol | Decompiled? |
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
| __ct__20PlanetGravityManagerFPCc | :white_check_mark: |
| init__20PlanetGravityManagerFRC12JMapInfoIter | :white_check_mark: |
| calcTotalGravityVector__20PlanetGravityManagerCFPQ29JGeometry8TVec3<f>P11GravityInfoRCQ29JGeometry8TVec3<f>UlUl | :x: |
| registerGravity__20PlanetGravityManagerFP13PlanetGravity | :x: |
| __dt__20PlanetGravityManagerFv | :white_check_mark: |


# PointGravity.o
| Symbol | Decompiled? |
| __ct__12PointGravityFv | :white_check_mark: |
| calcOwnGravityVector__12PointGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :white_check_mark: |
| updateMtx__12PointGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :white_check_mark: |


# SegmentGravity.o
| Symbol | Decompiled? |
| __ct__14SegmentGravityFv | :x: |
| calcOwnGravityVector__14SegmentGravityCFPQ29JGeometry8TVec3<f>PfRCQ29JGeometry8TVec3<f> | :x: |
| updateMtx__14SegmentGravityFRCQ29JGeometry64TPosition3<Q29JGeometry38TMatrix34<Q29JGeometry13SMatrix34C<f>>> | :x: |
| setGravityPoint__14SegmentGravityFUlRCQ29JGeometry8TVec3<f> | :x: |
| setSideVector__14SegmentGravityFRCQ29JGeometry8TVec3<f> | :x: |
| setValidSideDegree__14SegmentGravityFf | :x: |
| setEdgeValid__14SegmentGravityFUlb | :x: |
| updateLocalParam__14SegmentGravityFv | :x: |


